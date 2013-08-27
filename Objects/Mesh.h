/*
 * Mesh.h
 *
 *  Created on: Nov 18, 2011
 *      Author: hussein
 */

#ifndef MESH_H_
#define MESH_H_

#include "Object.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "limits.h"

using namespace std;

//Represents a triangle of mesh
class Triangle: public Object {

public:
	Triangle();
	virtual ~Triangle();

	//Calculate the intersection of the ray and a triangle
	Intersection rayIntersection(Ray ray);

	//Print object info
	void printInfo();

	//Get triangle normal
	vec3 getNormal();

	void setVertices(vec3 a, vec3 b, vec3 c);
	void setNormals(vec3 a, vec3 b, vec3 c);
	void setTextures(vec3 a, vec3 b, vec3 c);

	//Set transformation matrix
	void setTransform(mat4 t);

	//Apply transformations to normals and vertices
	void applyTransforms();

	vec3 v[3];
	vec3 n[3];
	vec3 t[3];
	mat4 transform;
};

//Represents a mesh object
class Mesh: public Object {
public:
	Mesh(const char *file);
	virtual ~Mesh();

	//Intersection between a ray and a mesh object
	Intersection rayIntersection(Ray ray);

	//Print object info
	void printInfo();

	//Set transformation matrix
	void setTransform(mat4 t);

	//Apply transformations to all triangles
	void applyTransforms();

	//Add motionblur to triangles
	void completeMotionBlur();
private:
	//Parse mesh object file
	void parseFile(const char * file);

	void processObjLine(string line, vector<vec3> &vertices,
			vector<vec3> &normals, vector<vec3> &texs);

	vector<Triangle*> triangles;
};

#endif /* MESH_H_ */
