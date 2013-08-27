/*
 * PointLight.cpp
 *
 *  Created on: Nov 18, 2011
 *      Author: hussein
 */

#include "SpotLight.h"
#include "stdio.h"
SpotLight::SpotLight(vec3 color, vec3 pos, vec3 dir, float a1, float a2) : Light(color) {
	this->pos = pos;
	this->direction = normalize(dir);
	this->a1 = a1;
	this->a2 = a2;
}

SpotLight::~SpotLight() {
}

vec3 SpotLight::getColor(vec3 p) {
	float lightPower;
	vec3 vToObject = normalize(p - pos);
	float v = dot(direction, vToObject);
	float angleToObject = 180 * acos(v);


	if (angleToObject < a1) {
		lightPower = 1;
	} else if (angleToObject < a2 && a2 != a1) {
		lightPower = 1 - (angleToObject - a1) / (a2 - a1);
	} else {
		lightPower = 0;
	}

	//printf("%f\n", v);
	return (lightPower * color);
}

vec3 SpotLight::getDirectionToLight(vec3 pos) {
	vec3 p = normalize(this->pos - pos);
	return p;
}

float SpotLight::getDistanceToLight(vec3 pos) {
	float l = length(this->pos - pos);
	return l;
}
