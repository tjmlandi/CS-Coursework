//
// Alternate vertex shader for the "Hello, OpenGL!" assignment.
//
// This will be used if the OpenGL implementation does not
// support OpenGL 3.2 and GLSL 1.50.
//

#version 120

// incoming vertex attributes
attribute vec4 vPosition;
attribute vec4 vColor;

// outgoing color sent to the fragment shader
varying vec4 rescolor;

void main()
{
    gl_Position = vPosition;
    rescolor = vColor;
}
