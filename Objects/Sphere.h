	/*
 * Sphere.h
 *
 *  Created on: Nov 16, 2011
 *      Author: hussein
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "Object.h"
#include "stdio.h"
class Sphere: public Object {
public:
	Sphere(float r, vec3 pos);
	virtual ~Sphere();

	//Check if the ray intersects
	Intersection rayIntersection(Ray ray);
	void printInfo();

	//Set matrix transformation
	void setTransform(mat4 t);

	//Apply matrix transformation
	void applyTransforms();

	//Get sphere's normal
	vec3 getNormal(vec3 p);

	//Get sphere's radius
	float getRadius();

private:
	float radius;
	float scaleX;
	float scaleY;
	float scaleZ;
	vec3 pos;

	Intersection intersectSphere(Ray ray);
	Intersection intersectEllipisoid(Ray ray);
};

#endif /* SPHERE_H_ */
