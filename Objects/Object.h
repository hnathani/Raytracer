/*
 * Object.h
 *
 *  Created on: Nov 15, 2011
 *      Author: hussein
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "../Lib/vec.h"
#include "../SceneObjects/Ray.h"
#include "Texture.h"
#include "../Lib/mat.h"


enum  {
	NONE, MOTIONBLUR
};


//Data used when a ray intersect an object
struct Intersection {
	Intersection();
	Intersection(vec3 normal, float distance);
	vec3 normal;
	float distance;
	vec3 texCoord;
};


//Material of an object
struct Material {
	Material();
	Material(float d, float s, float a, float p_c, float rl, float t,
			float rr);
	vec3 getColor(Intersection i);
	void setTexture(Texture *tex);
	void setColor(vec3 color);
	vec3 color;
	float diff, spec, ambi;
	float phong_cosine;
	float refl, trans, refr;
	bool hasTex;
	Texture *tex;
};

class Object {
public:
	Object();
	virtual ~Object();

	//Attach a material to this object
	void setMaterial(Material m);
	Material getMaterial();

	//Test if the ray intersects this object
	virtual Intersection rayIntersection(Ray ray)=0;

	//Set transformation matrix for this object
	virtual void setTransform(mat4 t)=0;

	//Apply transformations for this object
	virtual void applyTransforms()=0;

	//Print object attributes
	virtual void printInfo()=0;

	void setMotionBlur();

	void addMotionBlur(vec3 point);

	virtual void completeMotionBlur();

protected:
	Material mat;
	vector<vec3> mbPoints;
	bool motionBlurOn;
	mat4 t;
};

#endif /* OBJECT_H_ */
