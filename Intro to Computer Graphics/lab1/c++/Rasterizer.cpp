//
//  Rasterizer.cpp
//
//  Created by Warren R. Carithers on 01/28/14.
//  Based on a C++ version created by Joe Geigel on 11/30/11.
//  Copyright 2014 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//

#include <cmath>

#include "Rasterizer.h"

///
//
// Simple class that performs rasterization algorithms
//
///

///
// Constructor
//
// @param n number of scanlines
// @param C the Canvas to be used
///

Rasterizer::Rasterizer( int n, Canvas &canvas ) : n_scanlines(n), C(canvas)
{
}


///
// Draw my initials
//
// Draw my own initials using calls to drawLine(), in the same
// manner that makeLines() in the driver program draws 'CG'.
///

void Rasterizer::myInitials( void ) {

    // ######## Use light blue (0.678,0.847,0.902) for initials ######## 

    C.setColor( 0.678, 0.847, 0.902 );

    //
    // add code here to draw your initials
    // with calls to your drawLine() function
    // 

}

///
// Draw a line from (x0,y0) to (x1, y1)
//
// Implementation should be using the Midpoint Line Algorithm.
//
// You are to add the implementation here using only calls to the
// setPixel() method of the Canvas.
//
// @param x0 x coord of first endpoint
// @param y0 y coord of first endpoint
// @param x1 x coord of second endpoint
// @param y1 y coord of second endpoint
///

void Rasterizer::drawLine( int x0, int y0, int x1, int y1 )
{
    //
    // add code here to implement drawLine()
    // using the midpoint line algorithm
    //

}
