//
//  Pipeline.cpp
//
//  Created by Warren R. Carithers on 2016/10/19.
//  Based on a version created by Joe Geigel on 11/30/11.
//  Copyright 2016 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Thomas Landi
//

#include <iostream>
#include <cmath>

#include "Pipeline.h"
#include "Clipper.h"

#define PI 3.14159265
///
// Simple wrapper class for midterm assignment
//
// Only methods in the implementation file whose bodies
// contain the comment
//
//    // YOUR IMPLEMENTATION HERE
//
// are to be modified by students.
///

///
// Constructor
//
// @param w width of canvas
// @param h height of canvas
///
Pipeline::Pipeline( int w, int h ) : Canvas(w,h)
    // YOUR IMPLEMENTATION HERE if you need to add initializers
{
    // YOUR IMPLEMENTATION HERE if you need to modify the constructor
	polygons = vector<Poly>();
	//Initialize the transformation matrix to the identity matrix
	this->w[0][0] = 1;
	this->w[0][1] = 0;
	this->w[0][2] = 0;
	this->w[1][0] = 0;
	this->w[1][1] = 1;
	this->w[1][2] = 0;
	this->w[2][0] = 0;
	this->w[2][1] = 0;
	this->w[2][2] = 1;
}

///
// addPoly - Add a polygon to the canvas.  This method does not draw
//           the polygon, but merely stores it for later drawing.
//           Drawing is initiated by the drawPoly() method.
//
//           Returns a unique integer id for the polygon.
//
// @param p - Array containing the vertices of the polygon to be added.
// @param n - Number of vertices in polygon
//
// @return a unique integer identifier for the polygon
///
int Pipeline::addPoly( const Vertex p[], int n )
{
    // YOUR IMPLEMENTATION HERE
	//Add the polygon to the list of polygons, and return its index
	Poly tmp = {};	
	tmp.p = const_cast<Vertex*>(p);
	tmp.n = n;

	polygons.push_back(tmp);

    // REMEMBER TO RETURN A UNIQUE ID FOR THE POLYGON
	return polygons.size() - 1;
}

///
// drawPoly - Draw the polygon with the given id.  The polygon should
//            be drawn after applying the current transformation to
//            the vertices of the polygon.
//
// @param polyID - the ID of the polygon to be drawn.
///
void Pipeline::drawPoly( int polyID )
{
    // YOUR IMPLEMENTATION HERE
	int size = polygons[polyID].n;
	Clipper clipper = Clipper();
	Vertex tmp[50];
	size = clipper.clipPolygon(polygons[polyID].n, polygons[polyID].p, tmp, CWll, CWur);
	int x[size] = {};
	int y[size] = {};
	//Move the polygon's vertices into arrays for drawing
	for (int i = 0; i < size; ++i) {
		x[i] = tmp[i].x;
		y[i] = tmp[i].y;
	}
	//Apply the transformation matrix
	for (int i = 0; i < size; ++i) {
		x[i] = (x[i] * w[0][0]) + (y[i] * w[0][1]) + w[0][2];
		y[i] = (x[i] * w[1][0]) + (y[i] * w[1][1]) + w[1][2];	
	}
	//Apply the viewport
	for (int i = 0; i < size; ++i) {
		float tmpX = x[i] + CWll.x;
		tmpX = tmpX / (float)(CWur.x - CWll.x);
		x[i] = (tmpX * VPwidth) + VPx;
	
		float tmpY = y[i] + CWll.y;
		tmpY = tmpY / (float)(CWur.y - CWll.y);
		y[i] = (tmpY * VPheight) + VPy;
	}

	//Draw the polygon
	Canvas::drawOutline(size, x, y);
}

///
// clearTransform - Set the current transformation to the identity matrix.
///
void Pipeline::clearTransform( void )
{
    // YOUR IMPLEMENTATION HERE
	//Reset the transformation matrix to the identity matrix
	w[0][0] = 1;
        w[0][1] = 0;
        w[0][2] = 0;
        w[1][0] = 0;
        w[1][1] = 1;
        w[1][2] = 0;
        w[2][0] = 0;
        w[2][1] = 0;
        w[2][2] = 1;
}

///
// translate - Add a translation to the current transformation by
//             premultiplying the appropriate translation matrix to
//             the current transformation matrix.
//
// @param x - Amount of translation in x.
// @param y - Amount of translation in y.
///
void Pipeline::translate( float tx, float ty )
{
    // YOUR IMPLEMENTATION HERE
	//Pre multiply the matrix by the translation matrix created by the dimensions provided
	w[0][0] = w[0][0] + (w[2][0] * tx); 
	w[0][1] = w[0][1] + (w[2][1] * tx);
	w[0][2] = w[0][2] + (w[2][2] * tx);
	w[1][0] = w[1][0] + (w[2][0] * ty);
	w[1][1] = w[1][1] + (w[2][1] * ty);
	w[1][2] = w[1][2] + (w[2][2] * ty);
}

///
// rotate - Add a rotation to the current transformation by premultiplying
//          the appropriate rotation matrix to the current transformation
//          matrix.
//
// @param degrees - Amount of rotation in degrees.
///
void Pipeline::rotate( float degrees )
{
    // YOUR IMPLEMENTATION HERE
	//Pre multiply the matrix by the matrix created by the rotation angle provided
	float sinD = sin(degrees * PI / 180);
	float cosD = cos(degrees * PI / 180);
	w[0][0] = (cosD * w[0][0]) - (sinD * w[1][0]);
	w[0][1] = (cosD * w[0][1]) - (sinD * w[1][1]);
	w[0][2] = (cosD * w[0][2]) - (sinD * w[1][2]);
	w[1][0] = (cosD * w[1][0]) + (sinD * w[0][0]);
	w[1][1] = (cosD * w[1][1]) + (sinD * w[0][1]);
	w[1][2] = (cosD * w[1][2]) + (sinD * w[0][2]);
	
}

///
// scale - Add a scale to the current transformation by premultiplying
//         the appropriate scaling matrix to the current transformation
//         matrix.
//
// @param x - Amount of scaling in x.
// @param y - Amount of scaling in y.
///
void Pipeline::scale( float sx, float sy )
{
    // YOUR IMPLEMENTATION HERE
	//Pre multiply matrix by the matrix created by the scale provided
	w[0][0] *= sx;
	w[0][1] *= sx;
	w[0][2] *= sx;
	w[1][0] *= sy;
	w[1][1] *= sy;
	w[1][2] *= sy;
}

///
// setClipWindow - Define the clip window.
//
// @param bottom - y coord of bottom edge of clip window (in world coords)
// @param top - y coord of top edge of clip window (in world coords)
// @param left - x coord of left edge of clip window (in world coords)
// @param right - x coord of right edge of clip window (in world coords)
///
void Pipeline::setClipWindow( float bottom, float top, float left, float right )
{
    // YOUR IMPLEMENTATION HERE
	//Save the clip window dimensions provided
	CWll.x = left;
	CWll.y = bottom;
	CWur.x = right;
	CWur.y = top;
}

///
// setViewport - Define the viewport.
//
// @param xmin - x coord of lower left of view window (in screen coords)
// @param ymin - y coord of lower left of view window (in screen coords)
// @param width - width of view window (in world coords)
// @param height - width of view window (in world coords)
///
void Pipeline::setViewport( int x, int y, int width, int height )
{
    // YOUR IMPLEMENTATION HERE
	VPx = x;
	VPy = y;
	VPwidth = width;
	VPheight = height;
}
