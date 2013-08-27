/*
 * main.cpp
 *
 *  Created on: Nov 16, 2011
 *      Author: hussein
 */

#include "SceneObjects/Scene.h"
#include "Objects/Texture.h"
#include "string.h"
int main(int argc, char *argv[]) {
	srand ( time(NULL) );
	Scene s (argv[1]);
	if(argc == 3) {
		if(strncmp(argv[2], "anti", 4) == 0) {
			s.setAA(true);
		}
	}
	s.renderImage();
}
