/*
 * Scene.cpp
 *
 *  Created on: Nov 15, 2011
 *      Author: hussein
 */

#include "Scene.h"

Scene::Scene(const char *file) {
	antialiasing = false;
	motionblur = false;
	readFile(file);
}

Scene::~Scene() {
}

vec3 Scene::traceRay(Ray ray, int depth) {
	bool intersection = false, intoObject = true;
	Object *object = NULL;
	vec3 objPos, objNormal, color;
	unsigned int i, j, index;
	Intersection objDist, nextDist;
	objDist.distance = -1;
	for (i = 0; i < objects.size(); i++) {

		nextDist = objects[i]->rayIntersection(ray);

		if (nextDist.distance > 0.0001
				&& (nextDist.distance < objDist.distance || objDist.distance < 0)) {
			objDist = nextDist;
			object = objects[i];
			objPos = ray.getStart() + objDist.distance * ray.getDir();
			index = i;
			intersection = true;
		}
	}

	if (!intersection) {
		return bgColor;
	}
	Material mat = object->getMaterial();
	vec3 matColor = mat.getColor(objDist);
	objNormal = normalize(objDist.normal);
	color = matColor * mat.ambi * globalAmbient;

	if (dot(objNormal, ray.getDir()) > 0) {
		intoObject = false;
		objNormal = normalize(-1 * objNormal);
	}

	for (i = 0; i < lights.size(); i++) {
		bool shadow = false;
		vec3 lDir = normalize(lights[i]->getDirectionToLight(objPos));
		Ray rayToLight(lDir, objPos, ray.getT());

		objDist.distance = -1;
		float maxDist = lights[i]->getDistanceToLight(objPos);
		for (j = 0; j < objects.size(); j++) {
			nextDist = objects[j]->rayIntersection(rayToLight);

			if (nextDist.distance > 0.0001
					&& (nextDist.distance <= objDist.distance
							|| objDist.distance < 0)
					&& nextDist.distance <= maxDist) {
				shadow = true;
				break;
			}
		}

		if (!shadow || motionblur) {
			float ct = dot(objNormal, lDir);
			vec3 r = 2 * ct * objNormal - lDir;
			vec3 e = normalize(-1 * ray.getDir());

			if (ct > 0) {
				color =
						color
								+ lights[i]->getColor(objPos) * mat.diff * ct
										* matColor;
				vec3 r = 2 * ct * objNormal - lDir;
				float e_r = dot(e, r);
				e_r = pow(e_r, mat.phong_cosine);
				if (e_r > 0) {
					color = color
							+ matColor * lights[i]->getColor(objPos) * mat.spec
									* 2.5 * e_r;
				}
			}
		}
	}

	vec3 refl_color(0, 0, 0), refr_color(0, 0, 0);
	if (depth < camera.getDepth()) {
		vec3 refl_dir = (ray.getDir()
				- objNormal * 2 * (dot(ray.getDir(), objNormal)));

		if (mat.refl > 0.01) {
			Ray refl_ray(normalize(refl_dir), objPos, rand() % 10);
			refl_color = mat.refl * traceRay(refl_ray, depth + 1);
		}

		if (mat.refr > 0.01) {
			float n, nt;

			if (intoObject) {
				n = 1;
				nt = mat.refr;
			} else {
				n = mat.refr;
				nt = 1;
			}

			float cos = dot(objNormal, ray.getDir());
			float sin = 1 - ((n * n) / (nt * nt) * (1 - cos * cos));

			vec3 refr_dir;

			if (sin < 0) {
				refr_dir = normalize(refl_dir);
			} else {
				refr_dir = (n / nt) * ray.getDir()
						+ (n / nt * cos - sqrt(sin)) * objNormal;
			}

			Ray refr_ray(normalize(refr_dir), objPos, rand() % 10);
			refr_color = mat.trans * traceRay(refr_ray, depth + 1);
		}
	}
	color = color + refl_color + refr_color;
	return color;
}

void Scene::renderImage() {
	int x, y;
	FILE *ppmFile = fopen(dir.c_str(), "wb");
	fprintf(ppmFile, "P3\n%d %d\n255\n", camera.getResX(), camera.getResY());
	for (y = 0; y < camera.getResY(); y++) {
		for (x = 0; x < camera.getResX(); x++) {
			vec3 color(0, 0, 0);
			if (antialiasing) {
				vec3 color1 = .25 * traceRay(camera.getRay(x, y), 1);
				vec3 color2 = .25 * traceRay(camera.getRay(x + .5, y), 1);
				vec3 color3 = .25 * traceRay(camera.getRay(x, y + .5), 1);
				vec3 color4 = .25 * traceRay(camera.getRay(x + .5, y + .5), 1);
				color = color1 + color2 + color3 + color4;
			} else if (motionblur) {
				for (int i = 0; i < 10; i++) {
					float v = i;
					vec3 color1 = .25 * traceRay(camera.getRay(x, y), 1);
					vec3 color2 = .25
							* traceRay(camera.getRay(x + v / 10.0, y), 1);
					vec3 color3 = .25
							* traceRay(camera.getRay(x, y + v / 10.0), 1);
					vec3 color4 = .25
							* traceRay(
									camera.getRay(x + v / 10.0, y + v / 10.0),
									1);
					color = color + .10 * (color1 + color2 + color3 + color4);
				}
			} else {
				color = traceRay(camera.getRay(x, y), 1);
			}
			color[0] = std::min(color[0], 1.0f);
			color[1] = std::min(color[1], 1.0f);
			color[2] = std::min(color[2], 1.0f);
			fprintf(ppmFile, "%.0f %.0f %.0f ", color[0] * 255, color[1] * 255,
					color[2] * 255);
		}
		fprintf(ppmFile, "\n");
	}
	fclose(ppmFile);
}

void Scene::setAA(bool value) {
	antialiasing = value;
}

void Scene::readFile(const char *file) {
	ifstream scene_file;
	string line;
	scene_file.open(file);

	if (scene_file.is_open()) {
		while (scene_file.good()) {
			getline(scene_file, line);
			processSceneLine(line);
		}
	} else {
		cout << "Unable to open " << file
				<< " . Please enter a valid scene file. Exiting. \n";
		exit(0);
	}
}

void Scene::processSceneLine(string scene_line) {
	istringstream lineParser(scene_line, istringstream::in);
	string scene_attribute;
	lineParser >> scene_attribute;

//Camera attributes
	if (scene_attribute.compare("cam_point") == 0) {
		float x, y, z;
		lineParser >> x >> y >> z;
		camera.setPoint(x, y, z);

	} else if (scene_attribute.compare("cam_lookat") == 0) {
		float x, y, z;
		lineParser >> x >> y >> z;
		camera.setLookAt(x, y, z);
	} else if (scene_attribute.compare("cam_up") == 0) {
		float x, y, z;
		lineParser >> x >> y >> z;
		camera.setUp(x, y, z);
	} else if (scene_attribute.compare("cam_angle") == 0) {
		float angle;
		lineParser >> angle;
		camera.setAngle(angle);
	} else if (scene_attribute.compare("cam_res") == 0) {
		int x, y;
		lineParser >> x >> y;
		camera.setRes(x, y);
	} else if (scene_attribute.compare("cam_depth") == 0) {
		int depth;
		lineParser >> depth;
		camera.setDepth(depth);
	} else if (scene_attribute.compare("cam_end") == 0) {
		camera.calculateConstants();

	}

//Color / Light Attributes
	else if (scene_attribute.compare("background_color") == 0) {
		lineParser >> bgColor.x >> bgColor.y >> bgColor.z;
	} else if (scene_attribute.compare("light_ambient") == 0) {
		float r, g, b;
		lineParser >> r >> g >> b;
		globalAmbient = vec3(r, g, b);
	} else if (scene_attribute.compare("light_parallel") == 0) {
		float r, g, b, x, y, z;
		lineParser >> r >> g >> b >> x >> y >> z;
		ParallelLight *pl = new ParallelLight(vec3(r, g, b), vec3(x, y, z));
		lights.push_back(pl);

	} else if (scene_attribute.compare("light_point") == 0) {
		float r, g, b, x, y, z;
		lineParser >> r >> g >> b >> x >> y >> z;
		PointLight *pl = new PointLight(vec3(r, g, b), vec3(x, y, z));
		lights.push_back(pl);
	} else if (scene_attribute.compare("light_spot") == 0) {
		float r, g, b, x, y, z, dx, dy, dz, a1, a2;
		lineParser >> r >> g >> b >> x >> y >> z >> dx >> dy >> dz >> a1 >> a2;
		SpotLight *sl = new SpotLight(vec3(r, g, b), vec3(x, y, z),
				vec3(dx, dy, dz), a1, a2);
		lights.push_back(sl);
	}

//Materials
	else if (scene_attribute.compare("material") == 0) {
		string type;
		float r, g, b, Ka, Kd, Ks, n_s, R, T, IoF;
		lineParser >> type;
		lineParser >> r >> g >> b;
		lineParser >> Ka >> Kd >> Ks >> n_s >> R >> T >> IoF;
		Material mat(Kd, Ks, Ka, n_s, R, T, IoF);
		mat.setColor(vec3(r, g, b));
		currentObject->setMaterial(mat);

	} else if (scene_attribute.compare("material_texture") == 0) {
		string type, file;
		float Ka, Kd, Ks, n_s, R, T, IoF;
		lineParser >> type >> file;
		lineParser >> Ka >> Kd >> Ks >> n_s >> R >> T >> IoF;
		Texture *texture = new Texture(file.c_str());
		Material mat(Kd, Ks, Ka, n_s, R, T, IoF);
		mat.setTexture(texture);
		currentObject->setMaterial(mat);
	}

//Geometry

	else if (scene_attribute.compare("surface_sphere") == 0) {
		oType = 0;
		float x, y, z, r;
		lineParser >> x >> y >> z >> r;
		currentObject = new Sphere(r, vec3(x, y, z));
	} else if (scene_attribute.compare("surface_mesh") == 0) {
		oType = 1;
		string meshName;
		lineParser >> meshName;
		currentObject = new Mesh(meshName.c_str());
	}

//Transformations
	else if (scene_attribute.compare("translate") == 0) {
		float x, y, z;
		lineParser >> x >> y >> z;
		t = Translate(x, y, z);
		it = Translate(-x, -y, -z);
	} else if (scene_attribute.compare("scale") == 0) {
		float x, y, z;
		lineParser >> x >> y >> z;
		s = Scale(x, y, z);
		is = Scale(x, y, z);
	} else if (scene_attribute.compare("rotateX") == 0) {
		float theta;
		lineParser >> theta;
		r = RotateX(theta) * r;
		ir = ir * RotateX(-theta);
	} else if (scene_attribute.compare("rotateY") == 0) {
		float theta;
		lineParser >> theta;
		r = RotateY(theta) * r;
		ir = ir * RotateY(-theta);
	} else if (scene_attribute.compare("rotateZ") == 0) {
		float theta;
		lineParser >> theta;
		r = RotateZ(theta) * r;
		ir = ir * RotateZ(-theta);
	}

//Misc
	else if (scene_attribute.compare("output_file") == 0) {
		lineParser >> dir;
	} else if (scene_attribute.compare("begin_surface") == 0) {
		r = ::identity();
		s = ::identity();
		t = ::identity();
		ir = ::identity();
		is = ::identity();
		it = ::identity();
	} else if (scene_attribute.compare("end_surface") == 0) {
		if (oType) {
			currentObject->setTransform(s);
			currentObject->setTransform(r);
			currentObject->setTransform(t);
		} else {
			currentObject->setTransform(is);
			//currentObject->setTransform(ir);
			currentObject->setTransform(it);
		}
		currentObject->applyTransforms();
		currentObject->completeMotionBlur();
		objects.push_back(currentObject);
	} else if (scene_attribute.compare("motionblur") == 0) {
		motionblur = true;
		currentObject->setMotionBlur();
	} else if (scene_attribute.compare("mb") == 0) {
		float x, y, z;
		lineParser >> x >> y >> z;
		currentObject->addMotionBlur(vec3(x, y, z));
	}
}

