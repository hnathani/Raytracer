/*
 * Object.cpp
 *
 *  Created on: Nov 15, 2011
 *      Author: hussein
 */

#include "Object.h"

Material::Material() {

}

Material::Material(float d, float s, float a, float p_c, float rl,
		float t, float rr) {
	diff = d;
	spec = s;
	ambi = a;
	phong_cosine = p_c;
	refl = rl;
	trans = t;
	refr = rr;
	hasTex = false;
}

void Material::setColor(vec3 color) {
	this->color = color;
}

void Material::setTexture(Texture *tex) {
	this->tex = tex;
	hasTex = true;
}

vec3 Material::getColor(Intersection i) {
	if (hasTex) {
		int x,y,w,h;
		w = tex->getWidth();
		h = tex->getHeight();
		x = i.texCoord.x * (w - 1);
		y = i.texCoord.y * (h - 1);
		x = abs(x);
		y = abs(y);
		return (tex->getPixel(y % h , x %  w));
	}
	return color;
}

Intersection::Intersection() {

}
Intersection::Intersection(vec3 normal, float distance) {
	this->normal = normal;
	this->distance = distance;
}

Object::Object() {
	motionBlurOn = false;
	this->t = mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0),
			vec4(0, 0, 0, 1));
}

Object::~Object() {
}

void Object::setMotionBlur() {
	motionBlurOn = true;
	mbPoints.push_back(vec3(0,0,0));
}

void Object::addMotionBlur(vec3 point) {
	mbPoints.push_back(point);
}

void Object::setMaterial(Material m) {
	this->mat = m;
}

void Object::completeMotionBlur() {

}

Material Object::getMaterial() {
	return mat;
}

