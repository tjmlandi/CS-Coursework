//
// Shapes.h
//
// Prototypes for shape-drawing functions.
//
// This file should not be modified by students.
//

#ifndef _SHAPES_H_
#define _SHAPES_H_

#include "Canvas.h"

// Macros for object and shading selection
#define OBJ_QUAD	0
#define OBJ_TEAPOT	1
#define OBJ_BOARD	2
#define OBJ_WINE_CYLINDER	3
#define OBJ_WINE_CONE	4
#define OBJ_GRAPES	5

///
// Make either a quad or teapot
//
// @param choice - 0 for quad. 1 for teapot
// @param C      - the Canvas we'll use
///
void makeShape( int choice, Canvas &C );

#endif
