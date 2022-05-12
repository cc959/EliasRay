#version 330 core

out vec4 fragColor;
#define PI 3.14159265359

const float AA = 2;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

struct Light {
	vec3 position;
	vec3 color;
};

struct Ray {
	vec3 origin;
	vec3 direction;
};

vec2 random2(vec2 p) {
	return fract(sin(vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)))) * 43758.5453);
}

// 2D Random
float random(in vec2 st) {
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float sdSphere(vec3 pos, float r, vec3 pos) {
	return length(pos - sp) - r;
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

vec3 render(vec2 fc) {

	vec2 uv = fc / u_resolution * 2.f - vec2(1.f);

	Ray ray = CreateCameraRay(uv);

}

void main() {
	vec2 fc = gl_FragCoord.xy;

	vec3 outColor = vec3(0);

	for(float x = 0.f; x < 1.f; x += 1.f / AA) {
		for(float y = 0.f; y < 1.f; y += 1.f / AA) {
			outColor += render(fc + vec2(x, y));
		}
	}

	fragColor = vec4(outColor / (AA * AA), 1);
}
