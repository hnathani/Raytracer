/*
 * SpotLight.h
 *
 *  Created on: Nov 18, 2011
 *      Author: hussein
 */

#ifndef SPOTLIGHT_H_
#define SPOTLIGHT_H_

#include "Light.h"

class SpotLight: public Light {
public:
	SpotLight(vec3 color,vec3 pos, vec3 dir, float a1, float a2);
	virtual ~SpotLight();

	//Get color intensity of the light
	vec3 getColor(vec3 pos);

	//Get direction to the light
	vec3 getDirectionToLight(vec3 pos);

	//Get distance to the light
	float getDistanceToLight(vec3 pos);

private:
	//Position of the light
	vec3 pos, direction;
	float a1, a2;
};

#endif /* POINTLIGHT_H_ */
