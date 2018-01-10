//
//  helloOpenGL
//
//  Main program for the "Hello, OpenGL" assignment.
//
//  This code can be compiled as either C or C++.
//
//  Students should not be modifying this file.
//

#ifdef __cplusplus
#include <cstdlib>
#include <iostream>
#else
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

//
// GLEW and GLFW header files also pull in the OpenGL definitions
//

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "ShaderSetup.h"

#ifdef __cplusplus
using namespace std;
#endif

// Calculate offset into vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// window dimensions
int w_width  = 512;
int w_height = 512;

// buffer information
bool bufferInit = false;
GLuint buffer1, buffer2, ebuffer;
GLuint program;

// geometry data
float datapoints[] = {
     0.25, -0.75, 0.0, 1.0,   0.50, -0.75, 0.0, 1.0,   0.25,  0.15, 0.0, 1.0,
     0.50, -0.75, 0.0, 1.0,   0.50,  0.15, 0.0, 1.0,   0.25,  0.15, 0.0, 1.0,
     0.25,  0.25, 0.0, 1.0,   0.50,  0.25, 0.0, 1.0,   0.25,  0.50, 0.0, 1.0,
     0.50,  0.25, 0.0, 1.0,   0.50,  0.50, 0.0, 1.0,   0.25,  0.50, 0.0, 1.0,
    -0.25, -0.75, 0.0, 1.0,   0.00, -0.75, 0.0, 1.0,  -0.25,  0.75, 0.0, 1.0,
     0.00, -0.75, 0.0, 1.0,   0.00,  0.75, 0.0, 1.0,  -0.25,  0.75, 0.0, 1.0,
    -0.75, -0.75, 0.0, 1.0,  -0.50, -0.75, 0.0, 1.0,  -0.75,  0.75, 0.0, 1.0,
    -0.50, -0.75, 0.0, 1.0,  -0.50,  0.75, 0.0, 1.0,  -0.75,  0.75, 0.0, 1.0,
    -0.50, -0.12, 0.0, 1.0,  -0.25, -0.12, 0.0, 1.0,  -0.50,  0.12, 0.0, 1.0,
    -0.25, -0.12, 0.0, 1.0,  -0.25,  0.12, 0.0, 1.0,  -0.50,  0.12, 0.0, 1.0,
};
int dataSize = sizeof(datapoints);

GLushort elements[] = {
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
     15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29
};
int elemSize = sizeof(elements);

GLfloat colors1[] = {
    0.00, 1.00, 0.00, 1.0,   0.00, 1.00, 0.00, 1.0,   0.00, 0.28, 0.72, 1.0,
    0.00, 1.00, 0.00, 1.0,   0.00, 0.28, 0.72, 1.0,   0.00, 0.28, 0.72, 1.0,
    0.00, 0.20, 0.80, 1.0,   0.00, 0.20, 0.80, 1.0,   0.00, 0.00, 1.00, 1.0,
    0.00, 0.20, 0.80, 1.0,   0.00, 0.00, 1.00, 1.0,   0.00, 0.00, 1.00, 1.0,
    1.00, 0.00, 0.00, 1.0,   1.00, 0.00, 0.00, 1.0,   1.00, 0.00, 0.00, 1.0,
    1.00, 0.00, 0.00, 1.0,   1.00, 0.00, 0.00, 1.0,   1.00, 0.00, 0.00, 1.0,
    1.00, 1.00, 0.00, 1.0,   1.00, 1.00, 0.00, 1.0,   1.00, 1.00, 0.00, 1.0,
    1.00, 1.00, 0.00, 1.0,   1.00, 1.00, 0.00, 1.0,   1.00, 1.00, 0.00, 1.0,
    1.00, 1.00, 0.00, 1.0,   1.00, 0.00, 0.00, 1.0,   1.00, 1.00, 0.00, 1.0,
    1.00, 0.00, 0.00, 1.0,   1.00, 0.00, 0.00, 1.0,   1.00, 1.00, 0.00, 1.0
};
int colorSize1 = sizeof(colors1);

GLfloat colors2[] = {
    1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,
    1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,
    1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,
    1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,
    1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,
    1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,
    1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,
    1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,
    1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,
    1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0,   1.00, 1.00, 1.00, 1.0
};
int colorSize2 = sizeof(colors2);

// number of vertices in the model
int nverts = 30;

// click counter for color selection
int counter = 0;

// fall back to GLSL 1.20 shaders?
bool useAltShaders = false;

///
// create a buffer
///
GLuint makeBuffer( GLenum target, const void *data, GLsizei size )
{
    GLuint buffer;

    glGenBuffers( 1, &buffer );
    glBindBuffer( target, buffer );
    glBufferData( target, size, data, GL_STATIC_DRAW );

    return( buffer );
}


///
// create the shapes we'll display
///
void createShapes()
{

    // setup vertex buffer #1
    buffer1 = makeBuffer( GL_ARRAY_BUFFER, NULL, dataSize + colorSize1 );

    glBufferSubData( GL_ARRAY_BUFFER, 0, dataSize, datapoints );
    glBufferSubData( GL_ARRAY_BUFFER, dataSize, colorSize1, colors1 );

    // setup vertex buffer #2
    buffer2 = makeBuffer( GL_ARRAY_BUFFER, NULL, dataSize + colorSize2 );

    glBufferSubData( GL_ARRAY_BUFFER, 0, dataSize, datapoints );
    glBufferSubData( GL_ARRAY_BUFFER, dataSize, colorSize2, colors2 );

    // setup the element buffer
    ebuffer = makeBuffer( GL_ELEMENT_ARRAY_BUFFER, elements, elemSize );

    // note that the buffers have all been created
    bufferInit = true;

}


// OpenGL initialization
void init()
{

    // Load shaders and use the resulting shader program
    if( useAltShaders ) {
#ifdef __cplusplus
        cerr << "Falling back to GLSL 1.20 shaders" << endl;
#else
        fputs( "Falling back to GLSL 1.20 shaders\n", stderr );
#endif
        program = ShaderSetup( "alt.vert", "alt.frag" );
    } else {
        program = ShaderSetup( "shader.vert", "shader.frag" );
    }
    if (!program) {
#ifdef __cplusplus
        cerr << "Error setting up shaders - "
            << errorString(shaderErrorCode) << endl;
#else
        fprintf( stderr, "Error setting up shaders - %s\n",
            errorString(shaderErrorCode) );
#endif
        exit(1);
    }

    glEnable( GL_DEPTH_TEST );
    glEnable(GL_CULL_FACE);
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glCullFace(GL_BACK);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // set up the initial scene
    createShapes();
}


void display( void )
{
    // clear your frame buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // bind the vertex and element buffers
    if( (counter & 1) == 0 ) {
        glBindBuffer( GL_ARRAY_BUFFER, buffer1 );
    } else {
        glBindBuffer( GL_ARRAY_BUFFER, buffer2 );
    }
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // set up your attribute variables
    glUseProgram( program );
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(datapoints)) );

    // draw your shape
    glDrawElements( GL_TRIANGLES, nverts, GL_UNSIGNED_SHORT, (void *)0 );
}


void keyboard( GLFWwindow *window, int key, int scan, int action, int mods )
{
    switch( key ) {
        case GLFW_KEY_1:
            counter = 0;
            break;

        case GLFW_KEY_2:
            counter = 1;
            break;

        case GLFW_KEY_ESCAPE:  // Escape key
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose( window, 1 );
            break;
    }
}

void mouse( GLFWwindow *window, int button, int action, int mods )
{
    if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS ) {
        counter++;
    }
}


void glfwError( int code, const char *desc )
{
#ifdef __cplusplus
    cerr << "GLFW error " << code << ": " << desc << endl;
#else
    fprintf( stderr, "GLFW error %d: %s\n", code, desc );
#endif
    exit( 2 );
}


int main( int argc, char **argv )
{
    glfwSetErrorCallback( glfwError );

    if( !glfwInit() ) {
#ifdef __cplusplus
        cerr << "Can't initialize GLFW!" << endl;
#else
        fputs( "Can't initialize GLFW!\n", stderr );
#endif
        exit( 1 );
    }

    ///
    // on a Mac, you will need to do something like these calls
    // to ensure you're getting OpenGL 3.2 or better
    ///

    // glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    // glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
    // glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
    // glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    GLFWwindow *window = glfwCreateWindow( w_width, w_height,
        "Hello, OpenGL!", NULL, NULL );

    if( !window ) {
#ifdef __cplusplus
        cerr << "GLFW window create failed!" << endl;
#else
        fputs( "GLFW window create failed!\n", stderr );
#endif
        glfwTerminate();
        exit( 1 );
    }

    glfwMakeContextCurrent( window );

#ifndef __APPLE__
    GLenum err = glewInit();
    if( err != GLEW_OK ) {
#ifdef __cplusplus
        cerr << "GLEW error: " << glewGetErrorString(err) << endl;
#else
        fprintf( stderr, "GLEW error: %s\n", glewGetErrorString(err) );
#endif
        glfwTerminate();
        exit( 1 );
    }

    if( !GLEW_VERSION_3_2 ) {
#ifdef __cplusplus
        cerr << "OpenGL 3.2 not available" << endl;
#else
        fputs( "OpenGL 3.2 not available\n", stderr );
#endif
        if( !GLEW_VERSION_2_1 ) {
#ifdef __cplusplus
            cerr << "OpenGL 2.1 not available, either!" << endl;
#else
            fputs( "OpenGL 2.1 not available, either!\n", stderr );
#endif
            glfwTerminate();
            exit( 1 );
        }
    }
#endif

    // determine whether or not we can use GLSL 1.50
    int maj = glfwGetWindowAttrib( window, GLFW_CONTEXT_VERSION_MAJOR );
    int min = glfwGetWindowAttrib( window, GLFW_CONTEXT_VERSION_MINOR );

#ifdef __cplusplus
    cerr << "GLFW: using " << maj << "." << min << " context" << endl;
#else
    fprintf( stderr, "GLFW: using %d.%d context\n", maj, min );
#endif
    if( maj < 3 || (maj == 3 && min < 2) ) {
        // nope!
        useAltShaders = true;
    }

    init();

    glfwSetKeyCallback( window, keyboard );
    glfwSetMouseButtonCallback( window, mouse );

    while( !glfwWindowShouldClose(window) ) {
        display();
        glfwSwapBuffers( window );
        glfwWaitEvents();
    }

    glfwDestroyWindow( window );
    glfwTerminate();

    return 0;
}

