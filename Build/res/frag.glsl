#version 330 core

out vec4 fragColor;
#define PI 3.14159265359

const float AA = 3;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

struct Light {
	vec3 position;
	vec3 color;
};

vec2 random2(vec2 p) {
	return fract(sin(vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)))) * 43758.5453);
}

// 2D Random
float random(in vec2 st) {
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float voronoi(vec2 fc) {
	vec2 st = fc / u_resolution.xy;
	st.x *= u_resolution.x / u_resolution.y;

    // Scale
	st *= 5.;

    // Tile the space
	vec2 i_st = floor(st);
	vec2 f_st = fract(st);

	float m_dist = 1.;  // minimum distance

	for(int y = -1; y <= 1; y++) {
		for(int x = -1; x <= 1; x++) {
            // Neighbor place in the grid
			vec2 neighbor = vec2(float(x), float(y));

            // Random position from current + neighbor place in the grid
			vec2 point = random2(i_st + neighbor);

			// Animate the point
			point = 0.5 + 0.5 * sin(u_time + 6.2831 * point);

			// Vector between the pixel and the point
			vec2 diff = neighbor + point - f_st;

            // Distance to the point
			float dist = dot(diff, diff);

            // Keep the closer distance
			m_dist = min(m_dist, dist);
		}
	}
	return sqrt(m_dist);
}

float getVal(vec2 fc) {
	return voronoi(fc * voronoi(fc) + fc);
}

vec2 trans(vec2 fc) {
	vec2 st = fc / u_resolution.xy;
	st.x *= u_resolution.x / u_resolution.y;
	return st;
}

//const int nl = 2;

//Light lights[nl] = Light[nl] (Light(vec3(-5, 5, 0.1), vec3(1, 0.12, 0.81)), Light(vec3(5, -1, 0.1), vec3(0, 0.07, 1)));

float delta = 0.01;

vec3 render(vec2 fc) {
	float pos = getVal(fc);
	float right = getVal(fc + vec2(delta, 0));
	float front = getVal(fc + vec2(0, delta));

    // Visualize the distance field
	vec3 baseColor = vec3(1) * pos;

	vec3 tangent = normalize(vec3(0.05 * delta, 0, right - pos));
	vec3 bitangent = normalize(vec3(0, 0.05 * delta, front - pos));
	vec3 normal = cross(tangent, bitangent);

	vec2 mousePos = vec2(trans(u_mouse.xy));

	vec3 outColor = vec3(0);

	vec3 lightColor = vec3(0, 1.42, 2);//lights[i].color;
	vec3 lightPos = vec3(mousePos, 0.4);//lights[i].position;
	vec3 lightDir = normalize(lightPos - vec3(trans(fc), 0));
	outColor += baseColor * lightColor * pow(clamp(dot(normal, lightDir), 0, 1), 3);

	return outColor;
}

void main() {
	vec2 fc = gl_FragCoord.xy;

	vec3 outColor = vec3(0, 0, 0);//render(fc) + render(fc + vec2(0.5, 0)) + render(fc + vec2(0, 0.5)) + render(fc + vec2(0.5, 0.5));

	for(float x = 0.f; x < 1.f; x += 1.f / AA) {
		for(float y = 0.f; y < 1.f; y += 1.f / AA) {
			outColor += render(fc + vec2(x, y));
		}
	}

	fragColor = vec4(outColor / (AA * AA), 1);
}
