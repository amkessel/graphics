/***************************************************
 * lorenz.h
 *
 * Andrew Kessel
 * Created 9/9/2011
 *
 * Definitions/implementations (in one package for easy portability) of functions to
 * calculate the values of a lorenz attractor. Based off of the sample code given on
 * the Moodle for CSCI 5229 at http://www.prinmath.com/csci5229/misc/lorenz.c
 *
 ***************************************************/

#ifndef _LORENZ_H_
#define _LORENZ_H_

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "kutils.h"

using namespace kutils;

/***********************************
 ***********************************
 * DECLARATIONS
 ***********************************
 ***********************************/

namespace lorenz
{
	/* An input structure to make passing stuff into the generate_lorenz method cleaner */
	typedef struct lorenz_input_s
	{
		double sigma, beta, rho;
		double x, y, z;
		double dt;
		int nPoints;
		point3_t *points;
		double *distancesSq;
		double *gradients;
		double *minDistSq, *maxDistSq;
		double *minGrad, *maxGrad;
	} lorenz_input_t;

	/* Generates the set of lorenz points based on the values given in the input struct */
	void generate_lorenz(lorenz_input_t input);
}

/***********************************
 ***********************************
 * IMPLEMENTATIONS
 ***********************************
 ***********************************/

/* Calculates the minimum squared distance between a point at index i and its neighbors at i-1,i+1 */
double calculate_min_distSq(int i, int nPoints, point3_t *points)
{
	double minDistSq = DBL_MAX;
	point3_t p = points[i];

	if(i > 0) // then there is a point to the "left"
	{
		point3_t pleft = points[i-1];
		double distSq = p.x*pleft.x + p.y*pleft.y + p.z*pleft.z;
		
		if(distSq < minDistSq)
			minDistSq = distSq;
	}

	if(i < nPoints-1) // then there is a point to the "right"
	{
		point3_t pright = points[i+1];
		double distSq = p.x*pright.x + p.y*pright.y + p.z*pright.z;
		
		if(distSq < minDistSq)
			minDistSq = distSq;
	}
	
	return minDistSq;
}

/* Calculates the gradient of the section of line on which a point at index i resides */
double calculate_gradient(int i, int nPoints, point3_t *points)
{
	// if the point is on either end of the list, just use their neighboring points as proxies
	if(i == 0)
		i = 1;		
	else if(i == nPoints-1)
		i = nPoints-2;
			
	point3_t p1 = points[i-1];
	point3_t p2 = points[i];
	point3_t p3 = points[i+1];
	
	Vector3 v1 = Vector3(p2,p1);
	Vector3 v2 = Vector3(p2,p3);

	return v1.Angle(&v2, false, false);
}

/*
*  Explicit Euler integration
*/
void lorenz::generate_lorenz(lorenz_input_t input)
{
	input.points[0].x = input.x;
	input.points[0].y = input.y;
	input.points[0].z = input.z;
	
	*input.minDistSq = DBL_MAX;
	*input.maxDistSq = -1.0 * DBL_MAX;
	
	*input.minGrad = DBL_MAX;
	*input.maxGrad = -1.0 * DBL_MAX;
	
	// calculate the points
	for(int i = 1; i < input.nPoints; i++)
	{
		double dx = input.sigma*(input.y-input.x);
		double dy = input.x*(input.rho-input.z) - input.y;
		double dz = input.x*input.y - input.beta*input.z;
		
		input.x += input.dt*dx;
		input.y += input.dt*dy;
		input.z += input.dt*dz;
		
		input.points[i].x = input.x;
		input.points[i].y = input.y;
		input.points[i].z = input.z;
	}
	
	// calculate the min distances between neighbors and gradients
	for(int i = 0; i < input.nPoints; i++)
	{
		input.distancesSq[i] = calculate_min_distSq(i, input.nPoints, input.points);
		input.gradients[i] = calculate_gradient(i, input.nPoints, input.points);
		
		if(input.distancesSq[i] < *input.minDistSq)
			*input.minDistSq = input.distancesSq[i];
		if(input.distancesSq[i] > *input.maxDistSq)
			*input.maxDistSq = input.distancesSq[i];
			
		if(input.gradients[i] < *input.minGrad)
		{
			*input.minGrad = input.gradients[i];
		}
		if(input.gradients[i] > *input.maxGrad)
		{
			*input.maxGrad = input.gradients[i];
		}
	}
}

#endif
