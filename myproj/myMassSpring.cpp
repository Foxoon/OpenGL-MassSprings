#include "myMassSpring.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "mySpring.h"
#include <vector>
#include "mySphere.h"

myMassSpring::myMassSpring(unsigned int width, unsigned int height)
{
	unsigned int i, j;

	//Creating width*height particles.
	particles.assign(width, std::vector<myParticle>());
	for (i = 0; i < width; i++)
	{
		particles[i].assign(height, myParticle());
		for (j = 0; j < height; j++)
		{
			particles[i][j] = myParticle( static_cast<float>(i * SPACE_P1_TO_P2 - SPACE_P1_TO_P2 * width / 2.0f), 10.0f,
				static_cast<float>(j * SPACE_P1_TO_P2 - SPACE_P1_TO_P2 * height / 2.0f + 2.0f));
		}
	}

	for (i=0;i<width/4.0f; i++)
		particles[0][i].movable = false;
	for (i = (int)(3 * width / 4.0f); i < width; i++)
		particles[0][i].movable = false;


	//Creating springs.
	double k = DEFAULT_K;
	for (i = 0; i < width; ++i)
	{
		for (j = 0; j < height; ++j)
		{
			if (i > 0)
				springs.push_back(mySpring(&particles[i][j], &particles[i - 1][j], k));
			if (j > 0)
				springs.push_back(mySpring(&particles[i][j], &particles[i][j - 1], k));
			if (i > 0 && j > 0)
				springs.push_back(mySpring(&particles[i][j], &particles[i - 1][j - 1], k));
			if (i > 0 && j < height - 1)
				springs.push_back(mySpring(&particles[i][j], &particles[i - 1][j + 1], k));
			if (i < width - 2)
				springs.push_back(mySpring(&particles[i][j], &particles[i + 2][j], k));
			if (j < height - 2)
				springs.push_back(mySpring(&particles[i][j], &particles[i][j + 2], k));

		}
	}


	//Giving color values.
	kd[0] = 0.8f;		kd[1] = 0.8f;		kd[2] = 0.8f;		kd[3] = 1.0f;

	computeNormals();
}

void myMassSpring::clearForces()
{
	for (size_t i = 0; i < particles.size(); i++)
		for (size_t j = 0; j < particles[i].size(); j++)
			particles[i][j].force = glm::vec3(0.0f, 0.0f, 0.0f);
}

void myMassSpring::addForces()
{
	glm::vec3 gravity(0.0f, -DEFAULT_MASS*GRAVITY, 0.0f);

	for (size_t i = 0; i < particles.size(); i++)
		for (size_t j = 0; j < particles[i].size(); j++)
			particles[i][j].addForce(gravity);

	for (unsigned int i = 0; i < springs.size(); i++) springs[i].addForce();
}

void myMassSpring::calculateNextPosition()
{
	for (size_t i = 0; i < particles.size(); i++)
		for (size_t j = 0; j < particles[i].size(); j++)
			particles[i][j].calculateNextPosition();
}

void myMassSpring::ballCollision(mySphere *s)
{
	glm::vec3 repulsiveForce;
	for (size_t i = 0; i < particles.size(); i++)
	{
		for (size_t j = 0; j < particles[i].size(); j++)
		{
			if (glm::distance(s->center, particles[i][j].position) < s->radius)
			{
				repulsiveForce = glm::vec3(particles[i][j].position - s->center);
				particles[i][j].addForce(repulsiveForce);
			}
		}
	}
}

void myMassSpring::groundCollision(float floor_y)
{
	float l;
	for (size_t i = 0; i < particles.size(); i++)
		for (size_t j = 0; j < particles[i].size(); j++)
		{
			l = (particles[i][j].position.y - floor_y);
			if (l < 0)
				//particles[i][j].position.y = floor_y;
				particles[i][j].movable = false;
		}
}

void myMassSpring::computeNormals()
{
	for (size_t i = 0; i < particles.size(); i++)
		for (size_t j = 0; j < particles[i].size(); j++)
			particles[i][j].normal = glm::vec3(0.0f, 0.0f, 0.0f);

	for (size_t i = 0; i < particles.size() - 1; i++)
		for (size_t j = 0; j < particles[i].size() - 1; j++)
		{
			glm::vec3 face_normal = glm::cross(particles[i + 1][j].position - particles[i][j].position,
				particles[i + 1][j + 1].position - particles[i + 1][j].position);
			particles[i][j].normal += face_normal;
			particles[i + 1][j].normal += face_normal;
			particles[i + 1][j + 1].normal += face_normal;

			face_normal = glm::cross(particles[i][j + 1].position - particles[i + 1][j + 1].position,
				particles[i][j].position - particles[i][j + 1].position);
			particles[i][j].normal += face_normal;
			particles[i][j + 1].normal += face_normal;
			particles[i + 1][j + 1].normal += face_normal;
		}

	for (size_t i = 0; i < particles.size(); i++)
		for (size_t j = 0; j < particles[i].size(); j++)
			particles[i][j].normal = glm::normalize(particles[i][j].normal);
}

void myMassSpring::drawSpring()
{
	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i < particles.size() - 1; i++)
		for (size_t j = 0; j < particles[i].size() - 1; j++)
		{
			glNormal3fv(&particles[i][j].normal[0]);
			glVertex3fv(&particles[i][j].position[0]);
			glNormal3fv(&particles[i + 1][j].normal[0]);
			glVertex3fv(&particles[i + 1][j].position[0]);
			glNormal3fv(&particles[i + 1][j + 1].normal[0]);
			glVertex3fv(&particles[i + 1][j + 1].position[0]);

			glNormal3fv(&particles[i][j].normal[0]);
			glVertex3fv(&particles[i][j].position[0]);
			glNormal3fv(&particles[i + 1][j + 1].normal[0]);
			glVertex3fv(&particles[i + 1][j + 1].position[0]);
			glNormal3fv(&particles[i][j + 1].normal[0]);
			glVertex3fv(&particles[i][j + 1].position[0]);
		}
	glEnd();
}

myMassSpring::~myMassSpring()
{

}

