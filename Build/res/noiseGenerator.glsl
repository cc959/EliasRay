#version 430 core

layout (local_size_x = 1, local_size_y = 1) in;

layout(r32f, binding = 0) uniform image3D perlin_output;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 randVector(vec2 co) {
    float random = rand(co);
    return vec2(cos(random), sin(random));
}

float perlin(vec2 point) {


  vec2 coord = floor(point);

  point = fract(point);

  vec2 rand1 = randVector(coord);
  vec2 rand2 = randVector(coord + vec2(1, 0));
  vec2 rand3 = randVector(coord + vec2(0, 1));
  vec2 rand4 = randVector(coord + vec2(1, 1));

  float randDot1 = dot(point, rand1);
  float randDot2 = dot(point - vec2(1, 0), rand2);
  float randDot3 = dot(point - vec2(0, 1), rand3);
  float randDot4 = dot(point - vec2(1, 1), rand4);

  float left = mix(randDot1, randDot3, smoothstep(0.f, 1.f, point.y));
  float right = mix(randDot2, randDot4, smoothstep(0.f, 1.f, point.y));

  float val = mix(left, right, smoothstep(0.f, 1.f, point.x));

  //make sure it's closer to zero and ond
  return clamp(val * 1.3 / 2 + 0.5, 0, 1);

}

float perlin3d(vec3 point) {
  float XY = perlin(point.xy);
  float YZ = perlin(point.yz);
  float ZX = perlin(point.zx);
  
  float YX = perlin(point.yz);
  float ZY = perlin(point.zy);
  float XZ = perlin(point.xz);
  
  float val = (XY + YZ + ZX + YX + ZY + XZ)/6;

  return val;
}

layout(location = 0) uniform vec3 offset;
layout(location = 1) uniform vec3 scale;
layout(location = 2) uniform float magnitude; 

void main() {
    
    ivec3 coords = ivec3(gl_GlobalInvocationID);
    ivec3 global_size = ivec3(gl_NumWorkGroups.xyz * gl_WorkGroupSize.xyz);

    float val = perlin3d((coords + offset) / scale) * magnitude;

    // output to a specific pixel in the image
    imageStore(perlin_output, coords, vec4(val, 0, 0, 0));
}