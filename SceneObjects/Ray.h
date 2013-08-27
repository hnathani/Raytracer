/*
 * Ray.h
 *
 *  Created on: Nov 15, 2011
 *      Author: hussein
 */

#ifndef RAY_H_
#define RAY_H_

#include "../Lib/vec.h"

class Ray {
public:
	//Create a ray. Each ray has a point and a vector
	Ray(vec3 dir, vec3 start, int time);
	virtual ~Ray();

	//Get ray attributes
	vec3 getDir();
	vec3 getStart();
	int getT();

	vec3 direction;
	vec3 start;
private:
	int timeValue;
};

#endif /* RAY_H_ */
