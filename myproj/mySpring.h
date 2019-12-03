#pragma once
#include "myParticle.h"


class mySpring
{
public:
	double restLength;
	double springConstant;
	myParticle *p1;
	myParticle *p2;

	mySpring();
	mySpring(myParticle *x1, myParticle *x2, double spring = DEFAULT_K);
	~mySpring();

	void mySpring::addForce();
};

