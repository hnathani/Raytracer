/*
 * Sphere.cpp
 *
 *  Created on: Nov 16, 2011
 *      Author: hussein
 */

#include "Sphere.h"

Sphere::Sphere(float r, vec3 pos) :
		Object() {
	this->radius = r;
	this->pos = pos;
	this->scaleX = radius * 1;
	this->scaleY = radius * 1;
	this->scaleZ = radius * 1;
}

Sphere::~Sphere() {
}

Intersection Sphere::rayIntersection(Ray ray) {

	if (scaleX != radius || scaleY != radius || scaleZ != radius) {
		return intersectEllipisoid(ray);
	} else {
		return intersectSphere(ray);
	}
}

void Sphere::setTransform(mat4 t) {
	if (t[0][1] == 0 && t[0][2] == 0 && t[0][3] == 0 && t[1][0] == 0
			&& t[1][2] == 0 && t[1][3] == 0 && t[2][0] == 0 && t[2][1] == 0
			&& t[2][3] == 0) {
		scaleX = radius * t[0][0];
		scaleY = radius * t[1][1];
		scaleZ = radius * t[2][2];
		return;
	}
	this->t = t * this->t;
	/*
	 float x, y, z;
	 x = t[0][3];
	 y = t[1][3];
	 z = t[2][3];
	 pos = pos + vec3(x, y, z);

	 if (t[0][1] == 0 && t[0][2] == 0 && t[0][3] == 0 && t[1][0] == 0
	 && t[1][2] == 0 && t[1][3] == 0 && t[2][0] == 0 && t[2][1] == 0
	 && t[2][3] == 0) {
	 scaleX = radius * t[0][0];
	 scaleY = radius * t[1][1];
	 scaleZ = radius * t[2][2];
	 }
	 */
}

void Sphere::applyTransforms() {

}

vec3 Sphere::getNormal(vec3 p) {
	vec3 n = normalize(p - pos);
	return n;
}

float Sphere::getRadius() {
	return radius;
}

void Sphere::printInfo() {
	printf("Radius %f Point %f %f %f\n", this->radius, this->pos[0],
			this->pos[1], this->pos[2]);
}

Intersection Sphere::intersectSphere(Ray ray) {
	vec4 newPos = t * vec4(ray.start, 1);
	ray.start = vec3(newPos.x, newPos.y, newPos.z);
	Intersection i;
	vec3 pos = this->pos;
	if (motionBlurOn) {
		pos = this->pos + mbPoints[ray.getT() % mbPoints.size()];
	}

	float b, c, det, t1, t2, dist;

	b = dot(2 * (ray.getStart() - pos), ray.getDir());
	c = dot(ray.getStart() - pos, ray.getStart() - pos) - (radius * radius);

	det = sqrt(b * b - 4 * c);
	if (det < 0) {
		i.distance = -1;
		return i;
	} else {
		t1 = ((-b) + det) / 2;
		t2 = ((-b) - det) / 2;
		if (t1 >= 0 && t1 <= t2) {
			dist = t1;
		} else if (t2 >= 0 && t2 <= t1) {
			dist = t2;
		} else {
			i.distance = -1;
			return i;
		}
	}
	i.distance = dist;
	i.normal = getNormal(ray.getStart() + dist * ray.getDir());
	return i;
}

float getScale(vec3 a, vec3 b, vec3 s) {
	float ret = (a.x * b.x) / s.x + (a.x * b.x) / s.y + (a.z * b.z) / s.z;
	return ret;
}

Intersection Sphere::intersectEllipisoid(Ray ray) {
	vec4 newPos = t * vec4(ray.start, 1);
		ray.start = vec3(newPos.x, newPos.y, newPos.z);
	Intersection i;

	vec3 pos = this->pos;
	if (motionBlurOn) {
		pos = this->pos + mbPoints[ray.getT() % mbPoints.size()];
	}
	vec3 dir = ray.getDir();
	pos = ray.getStart() - pos;
	vec3 sX(scaleX * scaleX, scaleY * scaleY, scaleZ * scaleZ);

	float a = ((dir.x * dir.x) / (sX.x)) + ((dir.y * dir.y) / (sX.y))
			+ ((dir.z * dir.z) / (sX.z));

	float b = ((2 * pos.x * dir.x) / (sX.x)) + ((2 * pos.y * dir.y) / (sX.y))
			+ ((2 * pos.z * dir.z) / (sX.z));

	float c = ((pos.x * pos.x) / (sX.x)) + ((pos.y * pos.y) / (sX.y))
			+ ((pos.z * pos.z) / (sX.z)) - 1;

	float disc = ((b * b) - (4.0 * a * c));

	if (disc < 0) {
		i.distance = -1;
		return i;
	}
	disc = sqrt(disc);

	float sol1 = (-b + disc) / (2.0 * a);
	float sol2 = (-b - disc) / (2.0 * a);

	if (sol1 < sol2) {
		i.distance = sol1;
	} else {
		i.distance = sol2;
	}
	vec3 n = ray.getStart() + i.distance * ray.getDir();
	i.normal.x = n.x / (sX[0]);
	i.normal.y = n.y / (sX[1]);
	i.normal.z = n.z / (sX[2]);
	i.normal = normalize(i.normal);
	return i;
}
