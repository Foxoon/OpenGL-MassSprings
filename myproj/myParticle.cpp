#include "myParticle.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>   

myParticle::myParticle()
{
	movable = true;
}

myParticle::myParticle(float x1, float y1, float z1, float inputmass)
{
	force = glm::vec3(0.0f, 0.0f, 0.0f);

	position = glm::vec3(x1, y1, z1);
	old_position = position;

	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	old_velocity = velocity;

	mass = inputmass;
	movable = true;
}

void myParticle::addForce(glm::vec3 f)
{
	force += f;
}

void myParticle::calculateNextPosition()
{
	if (!movable) return;
	glm::vec3 acceleration = glm::vec3(force / mass);
	glm::vec3 tmp = position;
	position = position + (position - old_position)*0.99f + acceleration * (float)DT;
	old_position = tmp;
}

myParticle::~myParticle()
{

}
