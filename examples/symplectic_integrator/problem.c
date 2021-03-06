/**
 * @file 	problem.c
 * @brief 	Example problem: symplectic_integrator.
 * @author 	Hanno Rein <hanno@hanno-rein.de>
 * @detail 	This example uses the symplectic Wisdom Holman (WH) integrator
 * to integrate test particles on eccentric orbits in a fixed potential.
 * Note that the WH integrator assumes that the central object is at the origin.
 * 
 * @section 	LICENSE
 * Copyright (c) 2011 Hanno Rein, Shangfei Liu
 *
 * This file is part of rebound.
 *
 * rebound is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * rebound is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rebound.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "main.h"
#include "tools.h"
#include "output.h"
#include "particle.h"
#include "boundaries.h"
#include "integrator.h"

void problem_init(int argc, char* argv[]){
	// Setup constants
	integrator	= WH;
	dt 		= 1e-3;	// in year/(2*pi)
	boxsize 	= 3;	// in AU
	N_active 	= 1; 	// Only star has non-zero mass. If all particles have mass, delete this line.
	init_box();

	// Initial conditions
	struct particle p; // Star
	// The WH integrator assumes a heliocentric coordinate system. 
	// Therefore the star has to be at the origin. 
	p.x  = 0; p.y  = 0; p.z  = 0; 
	p.vx = 0; p.vy = 0; p.vz = 0;
	p.ax = 0; p.ay = 0; p.az = 0;
	p.m  = 1;		// in Solar Masses
	particles_add(p); 
	
	int _N = 100; // Number of test particles
	if(argc>1){
		_N = atoi(argv[1]);
	}

	double eccentricity = 0.4;
	for(int n=0; n<_N; n++){
		struct particle p = tools_init_orbit2d(1., 0., 1., eccentricity, 0.,2.*M_PI/(double)(_N)*(double)(n));
		particles_add(p); // Test particle
	}
}

void problem_output(){
	if(output_check(100.*dt)){
		output_timing();
	}
}

void problem_finish(){
}
