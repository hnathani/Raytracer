/*
 * ParallelLight.h
 *
 *  Created on: Nov 18, 2011
 *      Author: hussein
 */

#ifndef PARALLELLIGHT_H_
#define PARALLELLIGHT_H_

#include "Light.h"
#include "limits.h"

class ParallelLight : public Light {
public:
	ParallelLight(vec3 color, vec3 dir);
	virtual ~ParallelLight();

	//Get color intensity at the specified point
	vec3 getColor(vec3 pos);

	//Get direction to the light at the specified point
	vec3 getDirectionToLight(vec3 pos);

	//Get distance to the light
	float getDistanceToLight(vec3 pos);

private:
	//Light direction
	vec3 dir;
};

#endif /* PARALLELLIGHT_H_ */
