#pragma once
 
const double GRAVITY = 0.684;				// Gravity constant
const double DT = 0.001;				// Refreshing step

/*Particles*/
const double DEFAULT_MASS = 0.2;		// Mass of the particles

/*Springs*/
const double DEFAULT_K = 20;					// Stiffness hooke's constant.
const double M_KD = 6;              // Springs damping

/*Mass-spring*/
const int DEFAULT_WIDTH = 80;			// Width of the mass-spring system
const int DEFAULT_HEIGHT = 80;				// Height of the mass-spring system 
const double SPACE_P1_TO_P2 = 0.15;	// Space between masses (particles)

const int INTEGRATOR = 1;