//
//  Viewing
//
//  Created by Warren R. Carithers 2016/11/11.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Simple class for setting up the viewing and projection transforms
//  for the Transformation Assignment.
//
//  This code can be compiled as either C or C++.
//
//  Contributor:  Thomas Landi
//

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "Viewing.h"


///
// This function sets up the view and projection parameter for a frustum
// projection of the scene. See the assignment description for the values
// for the projection parameters.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpFrustum( GLuint program )
{
    // Add your code here
	glUseProgram(program);
	//Get the location of the projection matrix in the shader
	GLint matrixLoc = glGetUniformLocation(program, "projection");
	//Create frustum matrix
	glm::mat4 proj = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 0.9f, 4.5f);
	//Save the matrix in the shader
	glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(proj));
}


///
// This function sets up the view and projection parameter for an
// orthographic projection of the scene. See the assignment description
// for the values for the projection parameters.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpOrtho( GLuint program )
{
    // Add your code here.
	glUseProgram(program);
	//Get the location of the projection matrix in the shader
	GLint matrixLoc = glGetUniformLocation(program, "projection");
	//Create an orthographic matrix
	glm::mat4 proj = glm::ortho(-1.0f,1.0f,-1.0f,1.0f,0.9f,4.5f);
	//Save the matrix in the shader
	glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(proj));
}


///
// This function clears any model transformations, setting the values
// to the defaults: no scaling (scale factor of 1), no rotation (degree
// of rotation = 0), and no translation (0 translation in each direction).
//
// You will need to write this function, and maintain all of the values
// which must be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void clearTransforms( GLuint program )
{
    // Add your code here.
	glUseProgram(program);
	//Get the location of the model matrix in the shader
	GLint matrixLoc = glGetUniformLocation(program, "model");
	//Create an identity matrix
	glm::mat4 model = glm::mat4();
	//Save the matrix in the shader
	glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(model));
}


///
// This function sets up the transformation parameters for the vertices
// of the teapot.  The order of application is specified in the driver
// program.
//
// You will need to write this function, and maintain all of the values
// which must be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param scaleX - amount of scaling along the x-axis
// @param scaleY - amount of scaling along the y-axis
// @param scaleZ - amount of scaling along the z-axis
// @param rotateX - angle of rotation around the x-axis, in degrees
// @param rotateY - angle of rotation around the y-axis, in degrees
// @param rotateZ - angle of rotation around the z-axis, in degrees
// @param translateX - amount of translation along the x axis
// @param translateY - amount of translation along the y axis
// @param translateZ - amount of translation along the z axis
///
void setUpTransforms( GLuint program,
    GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ,
    GLfloat rotateX, GLfloat rotateY, GLfloat rotateZ,
    GLfloat translateX, GLfloat translateY, GLfloat translateZ )
{
    // Add your code here.
	glUseProgram(program);
	//Get the location of the model matrix in the shader
	GLint matrixLoc = glGetUniformLocation(program, "model");
	
	//Create a scale matrix, a matrix for the rotations about each axis,
	//and a translation matrix
	glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(scaleX, scaleY, scaleZ));

	//Rotations commented out as I couldn't get them to work like scale/translate
	glm::mat4 rotateXmat = glm::mat4();//glm::rotate(rotateX, glm::vec3(1.0f,0.0f,0.0f));
	glm::mat4 rotateYmat = glm::mat4();//glm::rotate(rotateY, glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 rotateZmat = glm::mat4();//glm::rotate(rotateZ, glm::vec3(0.0f,0.0f,1.0f));


	glm::mat4 translate = glm::translate(glm::mat4(),
				glm::vec3(translateX,
					translateY,
					translateZ));
	
	//Create a model matrix with the previous matrices
	glm::mat4 model = translate * rotateXmat * rotateYmat * rotateZmat * scale * glm::mat4();
	//Save the matrix in the shader
	glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(model));
}


///
// This function clears any changes made to camera parameters, setting the
// values to the defaults: eye (0.0,0.0,0.0), lookat (0,0,0.0,-1.0),
// and up vector (0.0,1.0,0.0).
//
// You will need to write this function, and maintain all of the values
// which must be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void clearCamera( GLuint program )
{
    // Add your code here.
	glUseProgram(program);
	//Get the location of the camera matrix
	GLint matrixLoc = glGetUniformLocation(program, "camera");
	//Create a look at matrix for the default values
	glm::mat4 camera = glm::lookAt(
				glm::vec3(0.0f,0.0f,0.0f),
				glm::vec3(0.0f,0.0f,-1.0f),
				glm::vec3(0.0f,1.0f,0.0f));
	//Save the matrix in the shader
	glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(camera));
}

///
// This function sets up the camera parameters controlling the viewing
// transformation.
//
// You will need to write this function, and maintain all of the values
// which must be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param eyeX - x coordinate of the camera location
// @param eyeY - y coordinate of the camera location
// @param eyeZ - z coordinate of the camera location
// @param lookatX - x coordinate of the lookat point
// @param lookatY - y coordinate of the lookat point
// @param lookatZ - z coordinate of the lookat point
// @param upX - x coordinate of the up vector
// @param upY - y coordinate of the up vector
// @param upZ - z coordinate of the up vector
///
void setUpCamera( GLuint program,
    GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ,
    GLfloat lookatX, GLfloat lookatY, GLfloat lookatZ,
    GLfloat upX, GLfloat upY, GLfloat upZ )
{
    // Add your code here.
	glUseProgram(program);
	//Get the location of the camera matrix in the shader
	GLint matrixLoc = glGetUniformLocation(program, "camera");
	//Create a look at matrix for the provided values
	glm::mat4 camera = glm::lookAt(
				glm::vec3(eyeX, eyeY, eyeZ),
				glm::vec3(lookatX,lookatY,lookatZ),
				glm::vec3(upX,upY,upZ));
	//Save the matrix in the shader
	glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(camera));
}
