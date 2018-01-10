//
//  Viewing.h
//
//  Created by Warren R. Carithers on 2016/11/11.
//  Based on a version created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology. All rights reserved.
//  
//  Code for setting up all the parameters necessary to implement
//  model, camera, and projection transformations.
//  
//  This code can be compiled as either C or C++.
//

#ifndef _VIEWING_H_
#define _VIEWING_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

///
// This function sets up the view and projection parameter for a frustum
// projection of the scene.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpFrustum( GLuint program );

///
// This function clears any transformations, setting the values to the
// defaults: scale by 4 in Y, rotate by 50 in Y and 90 in Z, and
// translate by 1 in X and -1 in Z.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void clearTransforms( GLuint program );

///
// This function sets up the transformation parameters for the vertices
// of the teapot.  The order of application is specified in the driver
// program.
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
    GLfloat translateX, GLfloat translateY, GLfloat translateZ );

///
// This function clears any changes made to camera parameters, setting the
// values to the defaults: eyepoint (0.0,3.0,3.0), lookat (1,0,0.0,0.0),
// and up vector (0.0,1.0,0.0).
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void clearCamera( GLuint program );

///
// This function sets up the camera parameters controlling the viewing
// transformation.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param eyepointX - x coordinate of the camera location
// @param eyepointY - y coordinate of the camera location
// @param eyepointZ - z coordinate of the camera location
// @param lookatX - x coordinate of the lookat point
// @param lookatY - y coordinate of the lookat point
// @param lookatZ - z coordinate of the lookat point
// @param upX - x coordinate of the up vector
// @param upY - y coordinate of the up vector
// @param upZ - z coordinate of the up vector
///
void setUpCamera( GLuint program,
    GLfloat eyepointX, GLfloat eyepointY, GLfloat eyepointZ,
    GLfloat lookatX, GLfloat lookatY, GLfloat lookatZ,
    GLfloat upX, GLfloat upY, GLfloat upZ );

#endif
