/*
 * Camera.h
 *
 *  Created on: Nov 15, 2011
 *      Author: hussein
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Ray.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class Camera {
public:
	Camera();
	virtual ~Camera();

	//Get the ray that passes through the image frame at x,y
	Ray getRay(float x, float y);

	//Calculate constants used to generate ray
	void calculateConstants();

	//Set camera values
	void setPoint(float x, float y, float z);
	void setLookAt(float x, float y, float z);
	void setUp(float x, float y, float z);
	void setAngle(float angle);
	void setRes(int x, int y);
	void setDepth(int depth);

	int getResX();
	int getResY();
	int getDepth();

private:
	vec3 point;
	vec3 lookat;
	vec3 up;
	float angle;
	int res_x;
	int	res_y;
	int depth;

	float distance;
	vec3 u, v;
	vec3 offset;

	//Altered width and height
	vec3 v_width, v_height;
	vec3 imagePlane;
};

#endif /* CAMERA_H_ */
