//
// Vertex shader for the transformation assignment
//
// Created by Warren R. Carithers 2016/04/22.
//
// Contributor:  Thomas Landi
//

#version 150

// attribute:  vertex position
in vec4 vPosition;

// add other global shader variables to hold the
// parameters your application code is providing
uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

void main()
{
    // Apply the projection, camera, and model uniforms created in Viewing.cpp
    gl_Position =  projection * camera * model * vPosition;
}
