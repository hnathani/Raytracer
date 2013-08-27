/*
 * Mesh.cpp
 *
 *  Created on: Nov 18, 2011
 *      Author: hussein
 */

#include "Mesh.h"

Mesh::Mesh(const char *file) :
		Object() {
	parseFile(file);
}

Mesh::~Mesh() {
}

Intersection Mesh::rayIntersection(Ray ray) {
	Intersection currentInt, nextInt;
	currentInt.distance = -1;
	int index;
	bool intersection = false;
	for (unsigned int i = 0; i < triangles.size(); i++) {
		nextInt = triangles[i]->rayIntersection(ray);
		if (nextInt.distance > 0
				&& (nextInt.distance < currentInt.distance
						|| currentInt.distance < 0)) {
			index = i;
			intersection = true;
			currentInt = nextInt;
		}
	}
	return currentInt;
}

void Mesh::setTransform(mat4 t) {
	this->t = t * this->t;
}

void Mesh::applyTransforms() {
	for (unsigned int i = 0; i < triangles.size(); i++) {
		triangles[i]->setTransform(t);
		triangles[i]->applyTransforms();
	}
}

void Mesh::printInfo() {
	for (unsigned int i = 0; i < triangles.size(); i++) {
		for (int j = 0; j < 3; j++) {
			printf("v %f %f %f\n", triangles[i]->v[j].x, triangles[i]->v[j].y,
					triangles[i]->v[j].z);
		}
	}

	for (unsigned int i = 0; i < triangles.size(); i++) {
		for (int j = 0; j < 3; j++) {
			printf("n %f %f %f\n", triangles[i]->n[j].x, triangles[i]->n[j].y,
					triangles[i]->n[j].z);
		}
	}
}

void Mesh::parseFile(const char * file) {
	ifstream obj_file;
	string line;
	vector<vec3> vertices;
	vector<vec3> normals;
	vector<vec3> texs;
	obj_file.open(file);

	if (obj_file.is_open()) {
		while (obj_file.good()) {
			getline(obj_file, line);
			processObjLine(line, vertices, normals, texs);
		}
	} else {
		cout
				<< "Unable to open obj file. Please enter a obj scene file. Exiting. \n";

	}
}

void Mesh::processObjLine(string line, vector<vec3> &vertices,
		vector<vec3> &normals, vector<vec3> &texs) {
	istringstream lineParser(line, istringstream::in);
	string obj_attribute;
	lineParser >> obj_attribute;

	if (obj_attribute.compare("v") == 0) {
		float x, y, z;
		lineParser >> x >> y >> z;
		vec3 v(x, y, z);
		vertices.push_back(v);
	} else if (obj_attribute.compare("vn") == 0) {
		float x, y, z;
		lineParser >> x >> y >> z;
		vec3 n(x, y, z);
		normals.push_back(n);
	} else if (obj_attribute.compare("vt") == 0) {
		float x, y;
		lineParser >> x >> y;
		vec3 vt(x, y, 0);
		texs.push_back(vt);
	} else if (obj_attribute.compare("f") == 0) {
		vector<int> indices;
		string m;
		while (lineParser >> m) {
			istringstream tokenizer(m, istringstream::in);
			string token;

			while (getline(tokenizer, token, '/')) {
				indices.push_back(atoi(token.c_str()));
			}
		}
		vec3 v[3];
		vec3 n[3];
		vec3 tex[3];
		bool hasTextures = false;
		for (unsigned int i = 0; i < indices.size(); i = i + 3) {
			v[i / 3] = vertices[indices[i] - 1];

			if (indices[i + 1] > 0) {
				tex[i / 3] = texs[indices[i + 1] - 1];
				hasTextures = true;
			}
			n[i / 3] = normals[indices[i + 2] - 1];
		}

		Triangle *t = new Triangle();
		t->setVertices(v[0], v[1], v[2]);

		if (hasTextures) {
			t->setTextures(tex[0], tex[1], tex[2]);
		}

		t->setNormals(n[0], n[1], n[2]);
		triangles.push_back(t);
	}
}

void Mesh::completeMotionBlur() {
	if (motionBlurOn) {
		for (unsigned int i = 0; i < this->triangles.size(); i++) {
			triangles[i]->setMotionBlur();
			for (unsigned int j = 0; j < this->mbPoints.size(); j++) {
				triangles[i]->addMotionBlur(this->mbPoints[j]);
			}
		}

	}
}

Triangle::Triangle() :
		Object() {
	transform = mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0),
			vec4(0, 0, 0, 1));
}

Triangle::~Triangle() {

}

Intersection Triangle::rayIntersection(Ray ray) {

	Intersection i;
	vec3 v[3];
	v[0] = this->v[0];
	v[1] = this->v[1];
	v[2] = this->v[2];

	if (motionBlurOn) {
		v[0] = v[0] + mbPoints[ray.getT() % mbPoints.size()];
		v[1] = v[1] + mbPoints[ray.getT() % mbPoints.size()];
		v[2] = v[2] + mbPoints[ray.getT() % mbPoints.size()];
	}
	vec3 tNormal = cross(v[1] - v[0], v[2] - v[0]);
	if (length(tNormal) < .00001) {
		i.distance = -1;
		return i;
	}

	float vt = dot(tNormal, (v[0] - ray.getStart()));
	float vd = dot(tNormal, ray.getDir());
	if (fabs(vd) < .00001) {
		i.distance = -1;
		return i;
	}

	float d = vt / vd;
	if (d < 0) {
		i.distance = -1;
		return i;
	}

	vec3 i_point = ray.getStart() + d * ray.getDir();
	float i_v1 = dot(i_point - v[0], v[1] - v[0]);
	float i_v2 = dot(i_point - v[0], v[2] - v[0]);
	float v1_v2 = dot(v[1] - v[0], v[2] - v[0]);
	float v1_v1 = dot(v[1] - v[0], v[1] - v[0]);
	float v2_v2 = dot(v[2] - v[0], v[2] - v[0]);

	float p1 = (i_v1 * v1_v2 - i_v2 * v1_v1) / (v1_v2 * v1_v2 - v2_v2 * v1_v1);

	float p2 = ((i_v1 - p1 * v1_v2) / (v1_v1)) / (1 - p1);

	if (p1 < -.000001 || p1 > 1 + .000001 || p2 < -.000001
			|| p2 > 1 + .000001) {
		i.distance = -1;
		return i;
	}

	i.distance = d;
	i.normal = n[0];
	i.texCoord = this->t[0] + p2 * (1 - p1) * (this->t[1] - this->t[0])
			+ p1 * (this->t[2] - this->t[0]);

	return i;
}

vec3 Triangle::getNormal() {
	return n[0];
}

void Triangle::printInfo() {
}

void Triangle::setVertices(vec3 a, vec3 b, vec3 c) {
	v[0] = a;
	v[1] = b;
	v[2] = c;
}

void Triangle::setNormals(vec3 a, vec3 b, vec3 c) {
	n[0] = a;
	n[1] = b;
	n[2] = c;
}

void Triangle::setTextures(vec3 a, vec3 b, vec3 c) {
	t[0] = a;
	t[1] = b;
	t[2] = c;
}

void Triangle::setTransform(mat4 t) {
	this->transform = this->transform * t;
}

void Triangle::applyTransforms() {
	for (int i = 0; i < 3; i++) {
		vec4 tv = transform * vec4(v[i], 1);
		v[i] = vec3(tv.x, tv.y, tv.z);
	}
	vec3 normal = normalize(cross(v[1] - v[0], v[2] - v[0]));
	n[0] = normal;
	n[1] = normal;
	n[2] = normal;
}
