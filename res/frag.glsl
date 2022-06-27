#version 420 core

// Hash Functions https://www.shadertoy.com/view/ttc3zr

uint murmurHash1(uint src) {
	const uint M = 0x5bd1e995u;
	uint h = 1190494759u;
	src *= M;
	src ^= src >> 24u;
	src *= M;
	h *= M;
	h ^= src;
	h ^= h >> 13u;
	h *= M;
	h ^= h >> 15u;
	return h;
}

// 1 output, 1 input
float random(float src) {
	uint h = murmurHash1(floatBitsToUint(src));
	return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint murmurHash1(uvec2 src) {
	const uint M = 0x5bd1e995u;
	uint h = 1190494759u;
	src *= M;
	src ^= src >> 24u;
	src *= M;
	h *= M;
	h ^= src.x;
	h *= M;
	h ^= src.y;
	h ^= h >> 13u;
	h *= M;
	h ^= h >> 15u;
	return h;
}

// 1 output, 2 inputs
float random(vec2 src) {
	uint h = murmurHash1(floatBitsToUint(src));
	return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint murmurHash1(uvec3 src) {
	const uint M = 0x5bd1e995u;
	uint h = 1190494759u;
	src *= M;
	src ^= src >> 24u;
	src *= M;
	h *= M;
	h ^= src.x;
	h *= M;
	h ^= src.y;
	h *= M;
	h ^= src.z;
	h ^= h >> 13u;
	h *= M;
	h ^= h >> 15u;
	return h;
}

// 1 output, 3 inputs
float random(vec3 src) {
	uint h = murmurHash1(floatBitsToUint(src));
	return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uint murmurHash1(uvec4 src) {
	const uint M = 0x5bd1e995u;
	uint h = 1190494759u;
	src *= M;
	src ^= src >> 24u;
	src *= M;
	h *= M;
	h ^= src.x;
	h *= M;
	h ^= src.y;
	h *= M;
	h ^= src.z;
	h *= M;
	h ^= src.w;
	h ^= h >> 13u;
	h *= M;
	h ^= h >> 15u;
	return h;
}

// 1 output, 4 inputs
float random(vec4 src) {
	uint h = murmurHash1(floatBitsToUint(src));
	return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uvec2 murmurHash2(uint src) {
	const uint M = 0x5bd1e995u;
	uvec2 h = uvec2(1190494759u, 2147483647u);
	src *= M;
	src ^= src >> 24u;
	src *= M;
	h *= M;
	h ^= src;
	h ^= h >> 13u;
	h *= M;
	h ^= h >> 15u;
	return h;
}

// 2 outputs, 1 input
vec2 random2(float src) {
	uvec2 h = murmurHash2(floatBitsToUint(src));
	return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uvec2 murmurHash2(uvec2 src) {
	const uint M = 0x5bd1e995u;
	uvec2 h = uvec2(1190494759u, 2147483647u);
	src *= M;
	src ^= src >> 24u;
	src *= M;
	h *= M;
	h ^= src.x;
	h *= M;
	h ^= src.y;
	h ^= h >> 13u;
	h *= M;
	h ^= h >> 15u;
	return h;
}

// 2 outputs, 2 inputs
vec2 random2(vec2 src) {
	uvec2 h = murmurHash2(floatBitsToUint(src));
	return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uvec2 murmurHash2(uvec3 src) {
	const uint M = 0x5bd1e995u;
	uvec2 h = uvec2(1190494759u, 2147483647u);
	src *= M;
	src ^= src >> 24u;
	src *= M;
	h *= M;
	h ^= src.x;
	h *= M;
	h ^= src.y;
	h *= M;
	h ^= src.z;
	h ^= h >> 13u;
	h *= M;
	h ^= h >> 15u;
	return h;
}

// 2 outputs, 3 inputs
vec2 random2(vec3 src) {
	uvec2 h = murmurHash2(floatBitsToUint(src));
	return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uvec2 murmurHash2(uvec4 src) {
	const uint M = 0x5bd1e995u;
	uvec2 h = uvec2(1190494759u, 2147483647u);
	src *= M;
	src ^= src >> 24u;
	src *= M;
	h *= M;
	h ^= src.x;
	h *= M;
	h ^= src.y;
	h *= M;
	h ^= src.z;
	h *= M;
	h ^= src.w;
	h ^= h >> 13u;
	h *= M;
	h ^= h >> 15u;
	return h;
}

// 2 outputs, 4 inputs
vec2 random2(vec4 src) {
	uvec2 h = murmurHash2(floatBitsToUint(src));
	return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uvec3 murmurHash3(uint src) {
	const uint M = 0x5bd1e995u;
	uvec3 h = uvec3(1190494759u, 2147483647u, 3559788179u);
	src *= M;
	src ^= src >> 24u;
	src *= M;
	h *= M;
	h ^= src;
	h ^= h >> 13u;
	h *= M;
	h ^= h >> 15u;
	return h;
}

// 3 outputs, 1 input
vec3 random3(float src) {
	uvec3 h = murmurHash3(floatBitsToUint(src));
	return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uvec3 murmurHash3(uvec2 src) {
	const uint M = 0x5bd1e995u;
	uvec3 h = uvec3(1190494759u, 2147483647u, 3559788179u);
	src *= M;
	src ^= src >> 24u;
	src *= M;
	h *= M;
	h ^= src.x;
	h *= M;
	h ^= src.y;
	h ^= h >> 13u;
	h *= M;
	h ^= h >> 15u;
	return h;
}

// 3 outputs, 2 inputs
vec3 random3(vec2 src) {
	uvec3 h = murmurHash3(floatBitsToUint(src));
	return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uvec3 murmurHash3(uvec3 src) {
	const uint M = 0x5bd1e995u;
	uvec3 h = uvec3(1190494759u, 2147483647u, 3559788179u);
	src *= M;
	src ^= src >> 24u;
	src *= M;
	h *= M;
	h ^= src.x;
	h *= M;
	h ^= src.y;
	h *= M;
	h ^= src.z;
	h ^= h >> 13u;
	h *= M;
	h ^= h >> 15u;
	return h;
}

// 3 outputs, 3 inputs
vec3 random3(vec3 src) {
	uvec3 h = murmurHash3(floatBitsToUint(src));
	return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uvec3 murmurHash3(uvec4 src) {
	const uint M = 0x5bd1e995u;
	uvec3 h = uvec3(1190494759u, 2147483647u, 3559788179u);
	src *= M;
	src ^= src >> 24u;
	src *= M;
	h *= M;
	h ^= src.x;
	h *= M;
	h ^= src.y;
	h *= M;
	h ^= src.z;
	h *= M;
	h ^= src.w;
	h ^= h >> 13u;
	h *= M;
	h ^= h >> 15u;
	return h;
}

// 3 outputs, 4 inputs
vec3 random3(vec4 src) {
	uvec3 h = murmurHash3(floatBitsToUint(src));
	return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uvec4 murmurHash4(uint src) {
	const uint M = 0x5bd1e995u;
	uvec4 h = uvec4(1190494759u, 2147483647u, 3559788179u, 179424673u);
	src *= M;
	src ^= src >> 24u;
	src *= M;
	h *= M;
	h ^= src;
	h ^= h >> 13u;
	h *= M;
	h ^= h >> 15u;
	return h;
}

// 4 outputs, 1 input
vec4 random4(float src) {
	uvec4 h = murmurHash4(floatBitsToUint(src));
	return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uvec4 murmurHash4(uvec2 src) {
	const uint M = 0x5bd1e995u;
	uvec4 h = uvec4(1190494759u, 2147483647u, 3559788179u, 179424673u);
	src *= M;
	src ^= src >> 24u;
	src *= M;
	h *= M;
	h ^= src.x;
	h *= M;
	h ^= src.y;
	h ^= h >> 13u;
	h *= M;
	h ^= h >> 15u;
	return h;
}

// 4 outputs, 2 inputs
vec4 random4(vec2 src) {
	uvec4 h = murmurHash4(floatBitsToUint(src));
	return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uvec4 murmurHash4(uvec3 src) {
	const uint M = 0x5bd1e995u;
	uvec4 h = uvec4(1190494759u, 2147483647u, 3559788179u, 179424673u);
	src *= M;
	src ^= src >> 24u;
	src *= M;
	h *= M;
	h ^= src.x;
	h *= M;
	h ^= src.y;
	h *= M;
	h ^= src.z;
	h ^= h >> 13u;
	h *= M;
	h ^= h >> 15u;
	return h;
}

// 4 outputs, 3 inputs
vec4 random4(vec3 src) {
	uvec4 h = murmurHash4(floatBitsToUint(src));
	return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

//------------------------------------------------------------------------------

uvec4 murmurHash4(uvec4 src) {
	const uint M = 0x5bd1e995u;
	uvec4 h = uvec4(1190494759u, 2147483647u, 3559788179u, 179424673u);
	src *= M;
	src ^= src >> 24u;
	src *= M;
	h *= M;
	h ^= src.x;
	h *= M;
	h ^= src.y;
	h *= M;
	h ^= src.z;
	h *= M;
	h ^= src.w;
	h ^= h >> 13u;
	h *= M;
	h ^= h >> 15u;
	return h;
}

// 4 outputs, 4 inputs
vec4 random4(vec4 src) {
	uvec4 h = murmurHash4(floatBitsToUint(src));
	return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

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
	vec3 position;
	vec3 size;
	vec3 color;
	vec3 specular;
	int pad;
	uint type;
};

struct Hit {
	vec3 position;
	float dist;
	vec3 normal;
	Object object;
};

layout(std140, binding = 0) uniform object_buffer {
	Object objects[20];
};

mat3 GetTangentSpace(vec3 normal, vec3 seed) {
    // Choose a helper vector for the cross product
	vec3 helper = random3(seed);

    // Generate vectors
	vec3 tangent = normalize(cross(normal, helper));
	vec3 binormal = normalize(cross(normal, tangent));
	return mat3(tangent, binormal, normal);
}

vec3 SampleHemisphere(vec3 normal, vec3 seed) {
    // Uniformly sample hemisphere direction
	float cosTheta = random(seed);
	float sinTheta = sqrt(max(0.0f, 1.0f - cosTheta * cosTheta));
	float phi = 2 * PI * random(seed * 2.f);
	vec3 tangentSpaceDir = vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
    // Transform direction to world space
	return tangentSpaceDir * GetTangentSpace(normal, seed);
}

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

mat3 randomRot(float scatter, vec3 seed) {
	return rotationX((random(seed.xyz) - 0.5f) * 2.f * scatter) * rotationY((random(seed.yzx) - 0.5f) * 2.f * scatter) * rotationZ((random(seed.zxy) - 0.5f) * 2.f * scatter);
}

vec3 sampleSkyBox(vec3 direction) {
	// Sample the skybox and write it
	float theta = acos(direction.y) / -PI;
	float phi = atan(direction.x, -direction.z) / -PI * 0.5f;

	return texture(skybox, vec2(phi, theta)).xyz * 1.3;
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
    // Transform the direction from camera to world space and normalize0
	direction = (_CameraToWorld * vec4(direction, 0.0f)).xyz;
	direction = normalize(direction);
	return Ray(origin, direction);
}

Hit Trace(Ray ray) {
	Hit bestHit;
	bestHit.dist = 1e10;
	bestHit.object.type = -1;
	bestHit.object.color = sampleSkyBox(ray.direction);

	for(int i = 0; i < n; i++) /**/
		if(intersectObject(objects[i], ray, bestHit))
			bestHit.object = objects[i];

	return bestHit;
}

const float eps = 1e-3;

vec3 render(vec2 fc) {

	vec2 uv = fc / u_resolution * 2.f - vec2(1.f);

	Ray ray = CreateCameraRay(uv);

	Hit hit = Trace(ray);

	vec3 outColor = vec3(0);
	vec3 rayStrength = vec3(1);

	for(int i = 0; i < 5; i++) {
		if(hit.object.type == -1) {
			outColor += hit.object.color * rayStrength;
			break;
		}

		ray.origin = hit.position + hit.normal * eps;

		vec3 reflected = reflect(ray.direction, hit.normal);
		ray.direction = hit.normal * randomRot(PI, hit.position + hit.normal); //SampleHemisphere(hit.normal, hit.position + hit.normal);//

		vec3 diffuse = 2 * min(1 - hit.object.specular, hit.object.color);
		float alpha = 15.0;

		vec3 specular = hit.object.specular * (alpha + 2) * pow(clamp(dot(ray.direction, reflected), 0.f, 1.f), alpha);

		rayStrength *= (diffuse + specular) * clamp(dot(hit.normal, ray.direction), 0.f, 1.f);

		hit = Trace(ray);
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
