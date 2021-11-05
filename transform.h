#include <glm/gtc/quaternion.hpp>

using namespace glm;

class Transform
{
private:
	/* data */
public:
	vec3 position;
	vec3 scale;
	quat rotation;

	Transform(/* args */);
	~Transform();
};

Transform::Transform(/* args */)
{
}

Transform::~Transform()
{
}
