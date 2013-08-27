/*
 * Ray.cpp
 *
 *  Created on: Nov 15, 2011
 *      Author: hussein
 */

#include "Ray.h"
#include "stdio.h"

Ray::Ray(vec3 dir, vec3 start,int time) {
	this->direction = dir;
	this->start = start;
	timeValue = time;
}

Ray::~Ray() {
}

vec3 Ray::getDir() {
	return direction;
}
vec3 Ray::getStart() {
	return start;
}

int Ray::getT() {
	return timeValue;
}
