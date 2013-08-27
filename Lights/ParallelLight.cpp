/*
 * ParallelLight.cpp
 *
 *  Created on: Nov 18, 2011
 *      Author: hussein
 */

#include "ParallelLight.h"

ParallelLight::ParallelLight(vec3 color, vec3 dir) : Light(color) {
	this->dir = normalize(dir);
}

ParallelLight::~ParallelLight() {
}

vec3 ParallelLight::getColor(vec3 p) {
	return this->color;
}

vec3 ParallelLight::getDirectionToLight(vec3 pos) {
	vec3 d = normalize(-1 * dir);
	return d;
}

float ParallelLight::getDistanceToLight(vec3 pos) {
	return INT_MAX;
}
