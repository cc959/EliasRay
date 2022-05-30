#version 420 core

out vec4 fragColor;
#define PI 3.14159265359

const float AA = 2;

uniform vec2 u_resolution;
uniform float u_time;
uniform int smple;
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
	int type;
};

struct Hit {
	vec3 position;
	float dist;
	vec3 normal;
	Object object;
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

vec4 sampleSkyBox(vec3 direction) {
	// Sample the skybox and write it
	float theta = acos(direction.y) / -PI;
	float phi = atan(direction.x, -direction.z) / -PI * 0.5f;

	return vec4(texture(skybox, vec2(phi, theta)).xyz, 0);
}

bool intersectSphere(vec3 c, float r, Ray ray, inout Hit bestHit) {
	vec3 d = ray.origin - c;
	float p1 = -dot(ray.direction, d);
	float p2sq = p1 * p1 - (dot(d, d) - r * r);

	if(p2sq < 0)
		return false;

	float p2 = sqrt(p2sq);
	float t = p1 - p2 > 0 ? p1 - p2 : p1 + p2; // use entry point unless behind you
	if(t > 0 && t < bestHit.dist) {
		bestHit.dist = t;
		bestHit.position = ray.origin + t * ray.direction;
		bestHit.normal = normalize(bestHit.position - c);
		return true;
	}
	return false;
}

bool intersectPlane(vec3 c, vec3 normal, Ray ray, inout Hit bestHit) {
	float den = dot(ray.direction, normal);

	float d = dot((c - ray.origin), normal) / den;

	if(den == 0 || d < 0)
		return false;

	vec3 position = ray.origin + ray.direction * d;

	if(d < bestHit.dist && length(position.xz) < 40.f) {
		bestHit.dist = d;
		bestHit.position = position;
		bestHit.normal = normal;
		return true;
	}
	return false;
}

bool intersectBox(vec3 c, vec3 size, Ray ray, inout Hit bestHit) {
	vec3 d = (ray.origin - c);
	float winding = all(lessThanEqual(abs(d), size)) ? -1.0 : 1.0;	// Winding direction: -1 if the ray starts inside of the box (i.e., and is leaving), +1 if it is starting outside of the box

	vec3 sgn = -sign(ray.direction); // planes the ray can intersect

	vec3 distToPlane = (size * winding * sgn - d) / ray.direction;

	#define TEST(U, VW)\
	(distToPlane.U >= 0.0) && all(lessThan(abs(d.VW + ray.direction.VW * distToPlane.U), size.VW)) /**/

	//sgn = sgn * vec3(TEST(x, yz), TEST(y, zx), TEST(z, xy));

	sgn = TEST(x, yz) ? vec3(sgn.x, 0.0, 0.0) : (TEST(y, zx) ? vec3(0.0, sgn.y, 0.0) : vec3(0.0, 0.0, TEST(z, xy) ? sgn.z : 0));

	if((sgn.x == 0) && (sgn.y == 0) && (sgn.z == 0))
		return false;

	float distance = (sgn.x != 0.0) ? distToPlane.x : ((sgn.y != 0.0) ? distToPlane.y : distToPlane.z);//dot(abs(sgn), distToPlane);

	if(distance < bestHit.dist) {
		bestHit.dist = distance;
		bestHit.position = ray.origin + ray.direction * distance;
		bestHit.normal = sgn;
		return true;
	}

	return false;
}

bool intersectObject(Object object, Ray ray, inout Hit bestHit) {
	if(object.type == 0)
		return intersectSphere(object.position.xyz, object.size.x, ray, bestHit);
	if(object.type == 1)
		return intersectBox(object.position.xyz, object.size.xyz, ray, bestHit);
	if(object.type == 2)
		return intersectPlane(object.position.xyz, object.size.xyz, ray, bestHit);

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

Hit Trace(Ray ray) {
	Hit bestHit;
	bestHit.dist = 1e10;
	bestHit.object = Object(vec4(0), vec4(0), sampleSkyBox(ray.direction), 0, -1); // skybox
	bestHit.normal = vec3(1, 0, 0);

	for(int i = 0; i < n; i++) /**/
		if(intersectObject(objects[i], ray, bestHit))
			bestHit.object = objects[i];

	return bestHit;
}

const float eps = 1e-4;

vec3 render(vec2 fc) {

	vec2 uv = fc / u_resolution * 2.f - vec2(1.f);

	Ray ray = CreateCameraRay(uv);

	Hit hit = Trace(ray);

	vec3 outColor = vec3(0);
	vec3 colorMultiplier = vec3(1);

	for(int i = 0; i < 10; i++) {

		if(hit.object.type == -1 || hit.object.color.w > 0.5) {
			outColor += hit.object.color.xyz * colorMultiplier;
			break;
		}

		vec3 refl = reflect(ray.direction, hit.normal);

		float scatter = PI * (1.f - hit.object.smoothness);
		refl *= randomRot(scatter, hit.position);

		//vec3 direction = lightDir * randomRot(0.03, refl);

		//Hit light = Trace(Ray(hit.position - direction * 5e-2, -direction));

		ray = Ray(hit.position + refl * eps, refl);
		Hit newhit = Trace(ray);

		// float diffuse = 0.f;
		// float specular = 0.f;

		// if(light.object == -1)
		// 	diffuse = max(dot(normal, -lightDir), 0.f);

		// if(newhit.object == -1)
		// 	specular = pow(max(dot(refl, -lightDir), 0.f), 8.f * objects[hit.object].smoothness);

		float lightMultiplier = 0;//min(mix(diffuse, specular, 0.1 + objects[hit.object].smoothness * 0.8), 1);

		outColor += hit.object.color.xyz * colorMultiplier * lightMultiplier;
		colorMultiplier *= 0.5 * (mix(hit.object.color.xyz, vec3(1.f), hit.object.smoothness * 0.5));

		hit = newhit;
	}

	return outColor;
}

void main() {
	vec2 fc = gl_FragCoord.xy;

	vec3 outColor = vec3(0);

	outColor += render(fc + random2(fc * u_time));

	outColor = pow(outColor, vec3(1.0 / 2.2));

	fragColor = vec4(outColor, 1.f / float(smple));
}
