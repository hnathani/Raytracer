/*
 * Texture.h
 *
 *  Created on: Nov 19, 2011
 *      Author: hussein
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "limits.h"
#include "../Lib/vec.h"

using namespace std;
class Texture {
public:
	Texture(const char *fileName);
	virtual ~Texture();

	//Parse texture from specified file
	void parseTexture(const char *fileName);

	//Print image to file
	void drawImage();

	//Get height of texture
	int getHeight();

	//Get width of texture
	int getWidth();

	//Get texture of pixel
	vec3 getPixel(int height, int width);

private:
	vec3 *pixelBuffer;
	int w,h;
};

#endif /* TEXTURE_H_ */
