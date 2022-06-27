#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
using namespace glm;

class Object
{
public:
    vec4 position = vec4(0, 0, 0, 0);
    vec4 size = vec4(1, 1, 1, 0);
    vec4 color = vec4(1, 1, 1, 0);
    vec4 specular = vec4(0, 0, 0, 0);
    uint type = 0;
    int pad[3];
};
