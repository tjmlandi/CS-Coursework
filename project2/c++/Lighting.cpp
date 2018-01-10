//
//  Lighting
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Contributor:  Thomas Landi
//
//  Simple class for setting up Phong illumination/shading.
//
//  This code can be compiled as either C or C++.
//

#include "Lighting.h"

// Add any global definitions and/or variables you need here.

///
// This function sets up the lighting, material, and shading parameters
// for the Phong shader.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpPhong( GLuint program, int color)
{
    // Add your code here
	glUseProgram(program);

	//Get the locations of the uniforms of the lighting parameters
	GLint dColorLoc = glGetUniformLocation(program, "diffuseColor");
	GLint sColorLoc = glGetUniformLocation(program, "specularColor");
	GLint lPosLoc = glGetUniformLocation(program, "lightPosition");

	float dColor[] = {1.0,0.0,0.0};
	if (color == 0) {
		dColor[0] = 0.0;
		dColor[1] = 1.0;
		dColor[2] = 0.0;
	}
	else if (color == 2) {
		dColor[0] = 1.0;
		dColor[1] = 1.0;
		dColor[2] = 0.0;
	}
	else if (color == 3) {
		dColor[0] = 0.0;
		dColor[1] = 0.0;
		dColor[2] = 1.0;
	}
	float sColor[] = {1.0,1.0,1.0};
	float lPos[] = {0.0,1000.0,1000.0};

	//Pass the values into the shader
	glUniform3fv(dColorLoc, 1, dColor);
	glUniform3fv(sColorLoc, 1, sColor);
	glUniform3fv(lPosLoc, 1, lPos);
}

void setUpDiffuse( GLuint program ) {
        glUseProgram(program);

        //Get the locations of the uniforms of the lighting parameters
        GLint colorLoc = glGetUniformLocation(program, "lightColor");
        GLint lightDirLoc = glGetUniformLocation(program, "lightDirection");
        GLint ambLoc = glGetUniformLocation(program, "ambience");
	GLint minBrightLoc = glGetUniformLocation(program, "minBrightness");

        float color[] = {0.0, 1.0, 0.0};
        float lightDir[] = {1.0,-1.0,1.0};
        float amb = 0.8;
	float minBright = 0.15;

        //Pass the values into the shader
        glUniform3fv(colorLoc, 1, color);
        glUniform3fv(lightDirLoc, 1, lightDir);
        glUniform1f(ambLoc, amb);
	glUniform1f(minBrightLoc, minBright);
}
