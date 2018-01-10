//
//  Viewing
//
//  Created by Warren R. Carithers 2016/11/11.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Simple class for setting up the viewing and projection transforms.
//
//  This code can be compiled as either C or C++.
//

#include "Viewing.h"

// current values for transformations
GLfloat rotateDefault[3]    = { 0.0f, 50.0f, 90.0f };
GLfloat translateDefault[3] = { 1.0f, 0.0f, -1.0f };
GLfloat scaleDefault[3]     = { 1.0f, 4.0f, 1.0f };

// current view values
GLfloat eyeDefault[3]  = { 0.0f, 3.0f, 3.0f };
GLfloat lookDefault[3] = { 1.0f, 0.0f, 0.0f };
GLfloat upDefault[3]   = { 0.0f, 1.0f, 0.0f };

// clipping window boundaries
GLfloat cwLeft   = -1.0f;
GLfloat cwRight  = 1.0f;
GLfloat cwTop    = 1.0f;
GLfloat cwBottom = -1.0f;
GLfloat cwNear   = 3.0f;
GLfloat cwFar    = 100.5f;

///
// This function sets up the view and projection parameter for a frustum
// projection of the scene.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpFrustum( GLuint program )
{
    GLint leftLoc = glGetUniformLocation( program, "left" );
    GLint rightLoc = glGetUniformLocation( program, "right" );
    GLint topLoc = glGetUniformLocation( program, "top" );
    GLint bottomLoc = glGetUniformLocation( program, "bottom" );
    GLint nearLoc = glGetUniformLocation( program, "near" );
    GLint farLoc = glGetUniformLocation( program, "far" );

    glUniform1f( leftLoc,   cwLeft );
    glUniform1f( rightLoc,  cwRight );
    glUniform1f( topLoc,    cwTop );
    glUniform1f( bottomLoc, cwBottom );
    glUniform1f( nearLoc,   cwNear );
    glUniform1f( farLoc,    cwFar );
}

///
// This function clears any transformations, setting the values to the
// defaults: scale by 4 in Y, rotate by 50 in Y and 90 in Z, and
// translate by 1 in X and -1 in Z.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void clearTransforms( GLuint program )
{
    // reset the shader using global data
    GLint thetaLoc = glGetUniformLocation( program, "theta" );
    GLint transLoc = glGetUniformLocation( program, "trans" );
    GLint scaleLoc = glGetUniformLocation( program, "scale" );

    glUniform3fv( thetaLoc, 1, rotateDefault );
    glUniform3fv( transLoc, 1, translateDefault );
    glUniform3fv( scaleLoc, 1, scaleDefault );
}

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
    GLfloat translateX, GLfloat translateY, GLfloat translateZ )
{
    GLfloat scaleVec[]     = { scaleX, scaleY, scaleZ };
    GLfloat rotateVec[]    = { rotateX, rotateY, rotateZ };
    GLfloat translateVec[] = { translateX, translateY, translateZ };

    GLint thetaLoc = glGetUniformLocation( program, "theta" );
    GLint transLoc = glGetUniformLocation( program, "trans" );
    GLint scaleLoc = glGetUniformLocation( program, "scale" );

    // send down to the shader
    glUniform3fv( thetaLoc, 1, rotateVec );
    glUniform3fv( transLoc, 1, translateVec );
    glUniform3fv( scaleLoc, 1, scaleVec );
}

///
// This function clears any changes made to camera parameters, setting the
// values to the defaults: eyepoint (0.0,3.0,3.0), lookat (1,0,0.0,0.0),
// and up vector (0.0,1.0,0.0).
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void clearCamera( GLuint program )
{
    GLint posLoc = glGetUniformLocation( program, "cPosition" );
    GLint lookLoc = glGetUniformLocation( program, "cLookAt" );
    GLint upVecLoc = glGetUniformLocation( program, "cUp" );

    glUniform3fv( posLoc, 1, eyeDefault );
    glUniform3fv( lookLoc, 1, lookDefault );
    glUniform3fv( upVecLoc, 1, upDefault );
}

///
// This function sets up the camera parameters controlling the viewing
// transformation.
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
    GLfloat eyeVec[]    = { eyeX, eyeY, eyeZ };
    GLfloat lookatVec[] = { lookatX, lookatY, lookatZ };
    GLfloat upVec[]     = { upX, upY, upZ };

    GLint posLoc = glGetUniformLocation( program, "cPosition" );
    GLint lookLoc = glGetUniformLocation( program, "cLookAt" );
    GLint upVecLoc = glGetUniformLocation( program, "cUp" );

    // send down to the shader
    glUniform3fv( posLoc, 1, eyeVec );
    glUniform3fv( lookLoc, 1, lookatVec );
    glUniform3fv( upVecLoc, 1, upVec );
}
