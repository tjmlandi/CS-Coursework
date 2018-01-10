//
//  Textures
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Contributor:  Thomas Landi
//
//  Simple class for setting up texture mapping parameters.
//
//  This code can be compiled as either C or C++.
//

#ifdef __cplusplus
#include <iostream>
#else
#include <stdio.h>
#endif

#include "Textures.h"

// this is here in case you are using SOIL;
// if you're not, it can be deleted.
#include <SOIL.h>

#ifdef __cplusplus
using namespace std;
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Add any global definitions and/or variables you need here.

GLuint texture;

///
// This function loads texture data for the GPU.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the various shaders.
///
void loadTexture( void )
{
	glEnable(GL_TEXTURE_2D);
    // Add your code here.
	int w;
	int h;
	int comp;
	stbi_set_flip_vertically_on_load(true);
	//Load the image with stb
	unsigned char* image = stbi_load("smiley2.png",&w, &h, &comp, STBI_rgb_alpha);

	//Only proceed if we loaded an image
	if (image == NULL)
		return

	//Bind the texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Set the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Define the texture properties
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);	
}

///
// This function sets up the parameters for texture use.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the various shaders.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpTexture( GLuint program )
{
    // Add your code here.
	//Set the active texture
	glActiveTexture(GL_TEXTURE0);
	//Bind the saved texture id
	glBindTexture(GL_TEXTURE_2D, texture);

	glUseProgram(program);

	//Pass the texture to the uniform in the shader
	glUniform1i(glGetUniformLocation(program, "myTexture"), GL_TEXTURE0);
}
