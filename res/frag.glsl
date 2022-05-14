#version 450 core

out vec4 fragColor;
#define PI 3.14159265359

const float AA = 2;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;
uniform int frames;
uniform vec3 lightDir;

uniform int n;

uniform mat4 _CameraToWorld;
uniform mat4 _CameraInverseProjection;

uniform sampler2D skybox;

// struct Light {
// 	vec3 position;
// 	vec3 color;
// };

struct Ray {
	vec3 origin;
	vec3 direction;
};

struct Object {
	vec4 position;
	vec4 size;
	vec4 color;
	float smoothness;
	uint data;
};

struct Hit {
	vec3 position;
	float dist;
	vec4 color;
	int object;
};

layout(std140, binding = 0) uniform object_buffer {
	Object objects[10];
};

mat3 rotationX(float angle) {
	float s = sin(angle);
	float c = cos(angle);

	return mat3(1.0, 0.0, 0.0, 0.0, c, s, 0.0, -s, c);
}

mat3 rotationY(float angle) {
	float s = sin(angle);
	float c = cos(angle);

	return mat3(c, 0.0, -s, 0.0, 1.0, 0.0, s, 0.0, c);
}

mat3 rotationZ(float angle) {
	float s = sin(angle);
	float c = cos(angle);

	return mat3(c, s, 0.0, -s, c, 0.0, 0.0, 0.0, 1.0);
}

vec2 random2(vec2 p) {
	return fract(sin(vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)))) * 43758.5453);
}

// 2D Random
float random(vec2 seed) {
	return fract(sin(dot(seed, vec2(12.9898, 78.233))) * 43758.5453123);
}

mat3 randomRot(float scatter, vec3 seed) {
	return rotationX((random(seed.xy) - 0.5f) * scatter) * rotationY((random(seed.yz) - 0.5f) * scatter) * rotationZ((random(seed.zx) - 0.5f) * scatter);
}

float sdSphere(vec3 pos, float r, vec3 sp) {
	vec3 d = pos - sp;
	if(max(abs(d.x), abs(d.z)) < 17.5)
		d.xz = mod(d.xz + 2.5, 5) - 2.5;
	return length(d) - r;
}

float sdRect(vec3 pos, vec3 size, vec3 sp) {
	vec3 q = abs(pos - sp) - size;
	return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float sd(Object object, vec3 sp) {
	uint type = object.data & ((1 << 30) - 1);
	if(type == 0)
		return sdSphere(object.position.xyz, object.size.x, sp);
	if(type == 1)
		return sdRect(object.position.xyz, object.size.xyz, sp);
}

vec4 sampleSkyBox(vec3 direction) {
	// Sample the skybox and write it
	float theta = acos(direction.y) / -PI;
	float phi = atan(-direction.z, direction.x) / -PI * 0.5f;
	return texture(skybox, vec2(phi, theta));
}

Hit query(vec3 sp) {
	Hit o;
	for(int i = 0; i < n; i++) {
		float dist = sd(objects[i], sp);
		if((objects[i].data & (1u << 30)) != 0)
			dist *= -1;

		Hit new = Hit(sp, dist, objects[i].color, i);

		if(i == 0)
			o = new;
		else {
			if((objects[i].data & (1u << 31)) != 0) {
				if(new.dist > o.dist)
					o = new;
			} else {
				if(new.dist < o.dist)
					o = new;
			}
		}
	}
	return o;
}

Ray CreateCameraRay(vec2 uv) {
    // Transform the camera origin to world space
	vec3 origin = (_CameraToWorld * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;

    // Invert the perspective projection of the view-space position
	vec3 direction = (_CameraInverseProjection * vec4(uv, 0.0f, 1.0f)).xyz;
    // Transform the direction from camera to world space and normalize
	direction = (_CameraToWorld * vec4(direction, 0.0f)).xyz;
	direction = normalize(direction);
	return Ray(origin, direction);
}

float eps = 1e-3;
int maxsteps = 200;

Hit march(Ray ray) {
	int cnt = 0;

	Hit hit;

	while(true) {
		Hit d = query(ray.origin);
		ray.origin += ray.direction * d.dist;

		if(d.dist < eps) {
			hit = d;
			break;
		}

		if(++cnt > maxsteps || d.dist > 1.f / eps)
			return Hit(ray.origin, d.dist, sampleSkyBox(ray.direction), -1);
	}

	return hit;

}

vec3 render(vec2 fc) {

	vec2 uv = fc / u_resolution * 2.f - vec2(1.f);

	Ray ray = CreateCameraRay(uv);

	Hit hit = march(ray);

	vec3 outColor = vec3(0);
	float colorMultiplier = 1;

	for(int i = 0; i < 5 && colorMultiplier > 0.1f; i++) {

		if(hit.object == -1) {
			outColor += hit.color.xyz * colorMultiplier;
			break;
		}

		float center = hit.dist;
		float x = query(hit.position + vec3(eps, 0, 0)).dist;
		float y = query(hit.position + vec3(0, eps, 0)).dist;
		float z = query(hit.position + vec3(0, 0, eps)).dist;

		vec3 normal = (vec3(x, y, z) - center) / eps;
		vec3 refl = reflect(ray.direction, normal);

		float scatter = PI * (1.f - objects[hit.object].smoothness);

		refl *= randomRot(scatter, normal + hit.position);

		vec3 direction = lightDir * randomRot(0.05, refl);

		Hit light = march(Ray(hit.position - direction * 5e-2, -direction));

		float lightMultiplier = 0.2f;
		if(light.object == -1)
			lightMultiplier = max(max(dot(normal, -lightDir), 0.f) + max(dot(refl, -lightDir), 0.f), lightMultiplier);

		outColor += hit.color.xyz * colorMultiplier * lightMultiplier;
		colorMultiplier *= 0.5;

		ray = Ray(hit.position + refl * 5e-2, refl);
		hit = march(ray);
	}

	return outColor;
}

void main() {
	vec2 fc = gl_FragCoord.xy;

	vec3 outColor = vec3(0);

	outColor += render(fc + random2(fc * u_time));

	fragColor = vec4(outColor, 1.f / float(frames));
}
