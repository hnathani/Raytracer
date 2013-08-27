/*
 * Scene.h
 *
 *  Created on: Nov 15, 2011
 *      Author: hussein
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Camera.h"
#include "../Lib/vec.h"
#include "../Objects/Sphere.h"
#include "../Objects/Mesh.h"
#include "../Lights/PointLight.h"
#include "../Lights/ParallelLight.h"
#include "../Lights/SpotLight.h"

using namespace std;

class Scene {
public:
	Scene(const char *file);
	virtual ~Scene();

	//Read scene file
	void readFile(const char *file);
	void processSceneLine(string scene_line);

	//Render the scene
	void renderImage();

	void setAA(bool value);

private:
	//Trace the path of a ray and return the color
	vec3 traceRay(Ray ray, int depth);

	vec3 bgColor;
	Camera camera;
	vec3 globalAmbient;
	vector<Object*> objects;
	vector<Light*> lights;

	Object *currentObject;
	vec3 *pixelBuffer;
	bool antialiasing;
	bool motionblur;
	int oType;
	mat4 r;
	mat4 t;
	mat4 s;
	mat4 ir;
	mat4 it;
	mat4 is;
	string dir;
};

#endif /* SCENE_H_ */
