//
//  Vertex.h
//
//  Representation of a 3D "Vertex".
//
//  Author:  Warren R. Carithers
//  Date:    2016/10/07 12:34:54
//

#ifndef _VERTEX_H_
#define _VERTEX_H_

///
// Information pertaining to a vertex
///

typedef struct Vertex {
    float x;
    float y;
    float z;
} Vertex;

///
// For this assignment, this type will also serve
// for Normals and texture coordinates
///

typedef Vertex Normal;
typedef Vertex UVcoord;   // x == u, y == v, z is ignored

#endif
