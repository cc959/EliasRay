#version 330 core

out vec4 fragColor;
#define PI 3.14159265359

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

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

float noise(vec2 fc) {
	vec2 st = fc / u_resolution.xy;
	st.x *= u_resolution.x / u_resolution.y;

	vec2 i = floor(st);
	vec2 f = fract(st);

	vec2 u = f * f * (3.0 - 2.0 * f);

	return mix(mix(dot(random2(i + vec2(0.0, 0.0)), f - vec2(0.0, 0.0)), dot(random2(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0)), u.x), mix(dot(random2(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0)), dot(random2(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0)), u.x), u.y);
}

float getVal(vec2 fc) {
	return voronoi(voronoi(fc) * fc + fc);
}

vec2 trans(vec2 fc) {
	vec2 st = fc / u_resolution.xy;
	st.x *= u_resolution.x / u_resolution.y;
	return st;
}

vec3 hsv2rgb(in vec3 c) {
	vec3 rgb = clamp(abs(mod(c.x * 6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
	rgb = rgb * rgb * (3.0 - 2.0 * rgb);
	return c.z * mix(vec3(1.0), rgb, c.y);
}

void main() {
	vec2 fc = gl_FragCoord.xy;
	vec3 color = vec3(fc / u_resolution.xy, 0.5) * 2;//vec3(0.88, 0, 1);

	float pos = getVal(fc);
	float right = getVal(fc + vec2(3, 0));
	float front = getVal(fc + vec2(0, 3));

    // Visualize the distance field
	vec3 c = color;

	vec3 lightColor = vec3(1);//hsv2rgb(vec3(pow(sin(u_time / 10.f), 2), 1, 1));
	vec3 tangent = normalize(vec3(0.1, 0, right - pos));
	vec3 bitangent = normalize(vec3(0, 0.1, front - pos));
	vec3 normal = cross(tangent, bitangent);

	vec2 mousePos = vec2(trans(u_mouse.xy));

	vec3 lightPos = vec3(mousePos, 0.1);
	vec3 lightDir = normalize(lightPos - vec3(trans(fc), 0));

	c *= lightColor * clamp(dot(normal, lightDir), 0.1, 1);

	fragColor = vec4(c, 1);
}
