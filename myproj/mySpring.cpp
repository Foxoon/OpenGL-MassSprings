#include "mySpring.h"
#include <glm/glm.hpp>
#include <iostream>

mySpring::mySpring()
{

}

mySpring::mySpring(myParticle *x1, myParticle *x2, double spring)
{
	p1 = x1;
	p2 = x2;
	restLength = glm::distance(p1->position, p2->position);
	springConstant = spring;
}

void mySpring::addForce()
{
	float x = pow(p1->position.x - p2->position.x, 2);
	float y = pow(p1->position.y - p2->position.y, 2);
	float z = pow(p1->position.z - p2->position.z, 2);

	float norm = sqrt(x+y+z);

	glm::vec3 pos = p1->position - p2->position;
	glm::vec3 velocity = p1->velocity - p2->velocity;

	glm::vec3 vec = glm::vec3(pos);
	vec *= (2.0f / glm::length(pos));

	glm::vec3 formula = -(glm::vec3(DEFAULT_K * (glm::length(pos) - restLength)) + glm::vec3(M_KD * (glm::dot(pos, velocity) / glm::length(pos))));
	glm::vec3 dampingForce = vec * formula;

	glm::vec3 force = (float)DEFAULT_K * (norm - (float)restLength) * (pos / norm);

	p1->addForce(force);
	p2->addForce(-force);
	p1->addForce(dampingForce);
	p2->addForce(-dampingForce);


}




mySpring::~mySpring()
{
}
