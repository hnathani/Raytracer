/*
 * PointLight.h
 *
 *  Created on: Nov 18, 2011
 *      Author: hussein
 */

#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "Light.h"

class PointLight: public Light {
public:
	PointLight(vec3 color,vec3 pos);
	virtual ~PointLight();

	//Get color intensity of the light
	vec3 getColor(vec3 pos);

	//Get direction to the light
	vec3 getDirectionToLight(vec3 pos);

	//Get distance to the light
	float getDistanceToLight(vec3 pos);

private:
	//Positin of the light
	vec3 pos;
};

#endif /* POINTLIGHT_H_ */
