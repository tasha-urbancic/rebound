/**
 * @file 	problem.c
 * @brief 	Example problem: MEGNO.
 * @author 	Hanno Rein <hanno@hanno-rein.de>
 * @detail 	This example calculates the Stark problem.
 * 
 * @section 	LICENSE
 * Copyright (c) 2015 Hanno Rein, Daniel Tamayo 
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
#include "output.h"
#include "tools.h"
#include "particle.h"
#include "problem.h"
#include "integrator.h"

void additional_forces();

void problem_init(int argc, char* argv[]){
	// Setup constants
	integrator	= WHFAST;
	dt 		= 0.0020;			// initial timestep (in days)
	init_boxwidth(200); 		

	// Initial conditions
	{
		struct particle p = {.m=1.,.x=0,.y=0.,.z=0.,.vx=0,.vy=0.,.vz=0.};
		particles_add(p); 
	}
	{
		struct particle p = {.m=0.,.x=1,.y=0.,.z=0.,.vx=0,.vy=1.2,.vz=0.};
		particles_add(p); 
	}
	tools_move_to_center_of_momentum();
	problem_additional_forces 	= additional_forces;
	// Add megno particles 
	tools_megno_init(1e-16);  // N = 6 after this function call. 
	system("rm -f Y.txt");
}

void additional_forces(){
	particles[1].ax += 0.12/6.;
}

void problem_output(){
	output_timing();
	FILE* f = fopen("Y.txt","a+");
	fprintf(f,"%e %e\n",t,tools_megno());
	fclose(f);
}

void problem_finish(){
}
