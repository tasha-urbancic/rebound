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

double energy();
double e_init;

void problem_init(int argc, char* argv[]){
	// Setup constants
	integrator	= WHFAST;
	dt 		= 1e-2;	// in year/(2*pi)
	boxsize 	= 3;	// in AU
	init_box();

	// Initial conditions
	struct particle p; // Star
	// The WH integrator assumes a heliocentric coordinate system. 
	// Therefore the star has to be at the origin. 
	p.x  = 0; p.y  = 0; p.z  = 0; 
	p.vx = 0; p.vy = -0.5; p.vz = 0;
	p.ax = 0; p.ay = 0; p.az = 0;
	p.m  = 0.5;		// in Solar Masses
	particles_add(p); 
	
	int _N = 10; // Number of test particles
	if(argc>1){
		_N = atoi(argv[1]);
	}

	{
	struct particle p; 
	p.x  = 1; p.y  = 0; p.z  = 0; 
	p.vx = 0.; p.vy = 0.5; p.vz = 0;
	p.ax = 0; p.ay = 0; p.az = 0;
	p.m  = .5;	
	particles_add(p); 
	}
	
	{
	struct particle p; 
	p.x  = 4; p.y  = 0; p.z  = 0; 
	p.m  = .105;	
	p.vx = 0; p.vy = sqrt((1.+p.m)/3.5); p.vz = 0;
	p.ax = 0; p.ay = 0; p.az = 0;
	particles_add(p); 
	}
	
	{
	struct particle p; 
	p.x  = 6; p.y  = 0; p.z  = 0; 
	p.m  = .105;	
	p.vx = 0; p.vy = sqrt((1.105+p.m)/5.5); p.vz = 0;
	p.ax = 0; p.ay = 0; p.az = 0;
//	particles_add(p); 
	}

	//double eccentricity = 0.4;
	for(int n=0; n<_N; n++){
		//struct particle p = tools_init_orbit2d(1., 1e-3, 1., eccentricity, 0.,2.*M_PI/(double)(_N)*(double)(n));
//		particles_add(p); // Test particle
	}
//	tools_move_to_center_of_momentum();
	e_init = energy();
}
double energy(){
	double e_kin = 0.;
	double e_pot = 0.;
	for (int i=0;i<N;i++){
		struct particle pi = particles[i];
		e_kin += 0.5 * pi.m * (pi.vx*pi.vx + pi.vy*pi.vy + pi.vz*pi.vz);
		for (int j=i+1;j<N;j++){
			struct particle pj = particles[j];
			double dx = pi.x - pj.x;
			double dy = pi.y - pj.y;
			double dz = pi.z - pj.z;
			e_pot -= G*pj.m*pi.m/sqrt(dx*dx + dy*dy + dz*dz);
		}
	}
	return e_kin +e_pot;
}

void problem_output(){
	if (output_check(1000.*dt)){
		output_timing();
		FILE* f = fopen("energy.txt","a");
		double e = energy();
		fprintf(f,"%e %e\n",t, fabs((e-e_init)/e_init));
		fclose(f);
	}
}

void problem_finish(){
}
