/*
 * Camera.cpp
 *
 *  Created on: Nov 15, 2011
 *      Author: hussein
 */

#include "Camera.h"
#include "stdio.h"

Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::setPoint(float x, float y, float z) {
	this->point = vec3(x, y, z);
}

void Camera::setLookAt(float x, float y, float z) {
	this->lookat = vec3(x, y, z);
}

void Camera::setUp(float x, float y, float z) {
	this->up = normalize(vec3(x, y, z));
}

void Camera::setAngle(float angle) {
	this->angle = angle;
}

void Camera::setRes(int x, int y) {
	this->res_x = x;
	this->res_y = y;
}

void Camera::setDepth(int depth) {
	this->depth = depth;
}

int Camera::getResX() {
	return res_x;
}
int Camera::getResY() {
	return res_y;
}
int Camera::getDepth() {
	return depth;
}

void Camera::calculateConstants() {
	 distance = length(lookat - point);
	 v = normalize(lookat - point);
	 u = -1 * normalize(cross(v, up));
	 up = normalize(cross(v,u));

	 vec3 imagePlane = point + v * distance;

	 float w = res_x;
	 float h = res_y;

	 float a_width = distance * tan(angle * M_PI / 180);
	 float a_height = a_width * (h / w);

	 vec3 x = (u) * a_width;
	 vec3 y = (up) * a_height;
	 offset = imagePlane + x + y;

	 v_width = u * 2 * a_width / res_x;
	 v_height = up * 2 * a_height / res_y;
}

Ray Camera::getRay(float x, float y) {
	 vec3 dir = offset - x * v_width - y * v_height;
	 dir = normalize(dir - point);
	 Ray ray(dir, point, rand() % 10);
	 return ray;
}
