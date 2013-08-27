/*
 * Texture.cpp
 *
 *  Created on: Nov 19, 2011
 *      Author: hussein
 */

#include "Texture.h"

Texture::Texture(const char *file) {
	parseTexture(file);
}

Texture::~Texture() {
}

int Texture::getHeight() {
	return h;
}

int Texture::getWidth() {
	return w;
}

vec3 Texture::getPixel(int height, int width) {
	return pixelBuffer[height*w + width];
}

void Texture::drawImage() {
	int x, y;
	FILE *ppmFile = fopen("texture.ppm", "wb");
	fprintf(ppmFile, "P3\n%d %d\n255\n", w, h);
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {

			vec3 color = pixelBuffer[y * w + x];

			fprintf(ppmFile, "%.0f %.0f %.0f ", color[0] * 255, color[1] * 255,
					color[2] * 255);
		}
		fprintf(ppmFile, "\n");
	}
	fclose(ppmFile);
}

void Texture::parseTexture(const char *file) {
	ifstream tex_file;
	string line;
	tex_file.open(file, ifstream::binary);
	float max = 0.0;
	string format;
	int lineIndex = 0;
	if (tex_file.is_open()) {
		while (tex_file.good()) {
			getline(tex_file, line);
			istringstream parseLine(line, istringstream::in);
			if (line[0] == '#') {
				continue;
			} else if (lineIndex == 0) {
				parseLine >> format;
				lineIndex++;
			} else if (lineIndex == 1) {
				parseLine >> w >> h;
				pixelBuffer = new vec3[h * w];
				lineIndex++;
			} else if (lineIndex == 2) {
				parseLine >> max;
				break;
			}
		}
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				vec3 c;
				if (max <= 256) {
					unsigned char r, g, b;
					tex_file.read((char*) &r, 1);
					tex_file.read((char*) &g, 1);
					tex_file.read((char*) &b, 1);
					float fr = r, fg = g, fb = b;
					c = vec3(fr / max, fg / max, fb / max);
				} else {
					short r, g, b;
					tex_file.read((char*) &r, 2);
					tex_file.read((char*) &g, 2);
					tex_file.read((char*) &b, 2);
					float fr = r, fg = g, fb = b;
					c = vec3(fr / max, fg / max, fb / max);
				}
				pixelBuffer[i * w + j] = c;
			}
		}
	} else {
		cout
				<< "Unable to open texture file. Please enter a valid texture file. Exiting. \n";
		exit(0);
	}
}
