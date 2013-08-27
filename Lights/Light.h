/*
 * Light.h
 *
 *  Created on: Nov 15, 2011
 *      Author: hussein
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "../Lib/vec.h"

class Light {
public:
	Light(vec3 color);
	virtual ~Light();

	//Get light intensity at the point
	virtual vec3 getColor(vec3 pos) = 0;

	//Get direction from point to light
	virtual vec3 getDirectionToLight(vec3 pos) = 0;

	//Get distance from point to light
	virtual float getDistanceToLight(vec3 pos) = 0;

protected:
	vec3 color;
};

#endif /* LIGHT_H_ */
