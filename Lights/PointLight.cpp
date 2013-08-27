/*
 * PointLight.cpp
 *
 *  Created on: Nov 18, 2011
 *      Author: hussein
 */

#include "PointLight.h"

PointLight::PointLight(vec3 color, vec3 pos) : Light(color) {
	this->pos = pos;
}

PointLight::~PointLight() {
}

vec3 PointLight::getColor(vec3 p) {
	return this->color;
}

vec3 PointLight::getDirectionToLight(vec3 pos) {
	vec3 p = normalize(this->pos - pos);
	return p;
}

float PointLight::getDistanceToLight(vec3 pos) {
	float l = length(this->pos - pos);
	return l;
}
