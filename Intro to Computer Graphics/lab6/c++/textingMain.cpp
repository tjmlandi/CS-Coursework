//
//  textingMain.cpp
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Main program for lighting/shading/texturing assignment
//
//  This file should not be modified by students.
//

#include <cstdlib>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "Buffers.h"
#include "ShaderSetup.h"
#include "Canvas.h"
#include "Shapes.h"
#include "Viewing.h"
#include "Lighting.h"
#include "Textures.h"

using namespace std;

// do we need to do a display() call?
bool updateDisplay = true;

// How to calculate an offset into the vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// our drawing canvas
Canvas *canvas;

// dimensions of the drawing window
int w_width  = 600;
int w_height = 600;

//
// We need two vertex buffers and two element buffers:
// one set for the quad (texture mapped), and one set
// for the teapot (Phong shaded)
//
BufferSet quadBuffers;
BufferSet teapotBuffers;

// Animation flag
bool animating = false;

// Initial animation rotation angles
float angles[2] = { 0.0f, 0.0f };

// program IDs...for shader programs
GLuint pshader, tshader;

//
// createShape() - create vertex and element buffers for a shape
//
// @param obj - which shape to create
// @param C   - the Canvas to use
//
void createShape( int obj, Canvas &C )
{
    // clear any previous shape
    C.clear();

    // make the shape
    makeShape( obj, C );

    // create the necessary buffers
    if( obj == OBJ_TEAPOT ) {
        teapotBuffers.createBuffers( C );
    } else {
        quadBuffers.createBuffers( C );
    }
}

///
// OpenGL initialization
///
void init( void )
{
    // Create our Canvas
    canvas = new Canvas( w_width, w_height );

    if( canvas == NULL ) {
        cerr << "error - cannot create Canvas" << endl;
        glfwTerminate();
        exit( 1 );
    }

    // Load texture image(s)
    loadTexture();

    // Load shaders, verifying each
    ShaderError error;
    tshader = shaderSetup( "texture.vert", "texture.frag", &error );
    if( !tshader ) {
        cerr << "Error setting up texture shader - " <<
            errorString(error) << endl;
        glfwTerminate();
        exit( 1 );
    }

    pshader = shaderSetup( "phong.vert", "phong.frag", &error );
    if( !pshader ) {
        cerr << "Error setting up Phong shader - " <<
            errorString(error) << endl;
        glfwTerminate();
        exit( 1 );
    }

    // Other OpenGL initialization
    glEnable( GL_DEPTH_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );

    // Create all our objects
    createShape( OBJ_QUAD, *canvas );
    createShape( OBJ_TEAPOT, *canvas );
}

///
// selectBuffers() - bind the correct vertex and element buffers
//
// @param program - GLSL program object
// @param B       - the BufferSet to use
///
void selectBuffers( GLuint program, BufferSet &B ) {

    // bind the buffers
    glBindBuffer( GL_ARRAY_BUFFER, B.vbuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, B.ebuffer );

    // set up the vertex attribute variables
    GLint vPosition = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );
    int offset = B.vSize;

    if( B.cSize ) {  // color data
        GLint vColor = glGetAttribLocation( program, "vColor" );
        glEnableVertexAttribArray( vColor );
        glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B.cSize;
    }

    if( B.nSize ) {  // normal data
        GLint vNormal = glGetAttribLocation( program, "vNormal" );
        glEnableVertexAttribArray( vNormal );
        glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B.nSize;
    }

    if( B.tSize ) {  // texture coordinate data
        GLint vTexCoord = glGetAttribLocation( program, "vTexCoord" );
        glEnableVertexAttribArray( vTexCoord );
        glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B.tSize;
    }
}

///
// Display callback
//
// Invoked whenever the image must be redrawn
///
void display( void )
{
    // clear and draw params..
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // first, the quad
    glUseProgram( tshader );

    // set up viewing and projection parameters
    setUpFrustum( tshader );

    // set up the texture information
    setUpTexture( tshader );

    // set up the camera
    setUpCamera( tshader,
        0.2f, 3.0f, 6.5f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );

    // set up transformations for the quad
    setUpTransforms( tshader,
        1.5f, 1.5f, 1.5f,
        angles[OBJ_QUAD], angles[OBJ_QUAD], angles[OBJ_QUAD],
        -1.25f, 1.0f, -1.5f
    );

    // draw it
    selectBuffers( tshader, quadBuffers );
    glDrawElements( GL_TRIANGLES, quadBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );

    // now, draw the teapot

    glUseProgram( pshader );

    // set up viewing and projection parameters
    setUpFrustum( pshader );

    // set up the Phong shading information
    setUpPhong( pshader );

    // set up the camera
    setUpCamera( pshader,
        0.2f, 3.0f, 6.5f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );

    setUpTransforms( pshader,
        2.0f, 2.0f, 2.0f,
        angles[OBJ_TEAPOT], angles[OBJ_TEAPOT], angles[OBJ_TEAPOT],
        1.5f, 0.5f, -1.5f );

    // draw it
    selectBuffers( pshader, teapotBuffers );
    glDrawElements( GL_TRIANGLES, teapotBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );
}

///
// Keyboard callback
//
// We don't need to distinguish between (e.g.) 'a' and 'A', so
// we can use the simpler form of callback
///
void keyboard( GLFWwindow *window, int key, int scan, int action, int mods )
{
    if( action != GLFW_PRESS )    // only activate on press, not rpt/release
        return;

    switch( key ) {

        case GLFW_KEY_A:    // animate
            animating = true;
            break;

        case GLFW_KEY_S:    // stop animating
            animating = false;
            break;

        case GLFW_KEY_R:    // reset rotations
            angles[0] = 0.0f;
            angles[1] = 0.0f;
            break;

        case GLFW_KEY_ESCAPE:   // terminate the program
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose( window, 1 );
            break;
    }

    updateDisplay = true;
}

// Animate the objects (maybe)
void animate( void ) {
    if( animating ) {
        angles[OBJ_QUAD]   += 1;
        angles[OBJ_TEAPOT] += 1;
        updateDisplay = true;
    }
}

void glfwError( int code, const char *desc )
{
    cerr << "GLFW error " << code << ": " << desc << endl;
    exit( 2 );
}

///
// main program for tessellation assignment
///
int main( int argc, char **argv ) {

    glfwSetErrorCallback( glfwError );

    if( !glfwInit() ) {
        cerr << "Can't initialize GLFW!" << endl;
        exit( 1 );
    }

    // NOTE:  Mac users may need to uncomment the following four lines
    // in order to force an OpenGL 3.2 (or better) context

    // glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    // glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
    // glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    // glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    GLFWwindow *window = glfwCreateWindow( w_width, w_height,
        "Lab 6 - Shading and Texturing", NULL, NULL );

    if( !window ) {
        cerr << "GLFW window create failed!" << endl;
        glfwTerminate();
        exit( 1 );
    }

    glfwMakeContextCurrent( window );

#ifndef __APPLE__
    GLenum err = glewInit();
    if( err != GLEW_OK ) {
        cerr << "GLEW error: " << glewGetErrorString(err) << endl;
        glfwTerminate();
        exit( 1 );
    }
    if( !GLEW_VERSION_3_2 ) {
        cerr << "GLEW: OpenGL 3.2 not available" << endl;
        if( !GLEW_VERSION_2_1 ) {
            cerr << "GLEW: OpenGL 2.1 not available, either!" << endl;
            glfwTerminate();
            exit( 1 );
        }
    }
#endif

    // determine whether or not we can use GLSL 1.50
    int maj = glfwGetWindowAttrib( window, GLFW_CONTEXT_VERSION_MAJOR );
    int min = glfwGetWindowAttrib( window, GLFW_CONTEXT_VERSION_MINOR );

    cerr << "GLFW: using " << maj << "." << min << " context" << endl;
    if( maj < 3 || (maj == 3 && min < 2) ) {
        // nope!
        cerr << "*** GLSL 1.50 shaders may not compile" << endl;
    }

    init();

    glfwSetKeyCallback( window, keyboard );

    while( !glfwWindowShouldClose(window) ) {
        animate();
        if( updateDisplay ) {
            updateDisplay = false;
            display();
            glfwSwapBuffers( window );
        }
        glfwPollEvents();
    }

    glfwDestroyWindow( window );
    glfwTerminate();


    return 0;
}
