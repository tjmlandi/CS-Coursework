//
//  Clipper.cpp
//
//  Created by Warren R. Carithers on 01/15/14.
//  Based on a version created by Joe Geigel on 11/30/11.
//  Copyright 2014 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Thomas Landi
//
#include <vector>
#include <iostream>

#include "Vertex.h"
#include "Clipper.h"

///
// Simple module that performs clipping
///

///
// Constructor
///
Clipper::Clipper() {
}

//This will return true if the provided point is 'inside' the provided side defined by the 
//lower left and upper right vertices of the edge
//where 0 is left, 1 is top, 2 is right, and 3 is bottom
bool inside(Vertex point, Vertex ll, Vertex ur, int side)
{
        if (side == 0)
        {
                if (point.x > ll.x)
                        return true;
                return false;
        }
        else if (side == 1)
        {
                if (point.y < ur.y)
                        return true;
                return false;
        }
        else if (side == 2)
        {
                if (point.x < ur.x)
                        return true;
                return false;
        }
        else
        {
                if (point.y > ll.y)
                        return true;
                return false;
        }
}

//Returns the vertex of the point of interextion between the lines defined by 
//PS and LLUR
Vertex intersect(Vertex p, Vertex s, Vertex ll, Vertex ur)
{
	//First check if the edge is a horizontal or vertical edge
        if (ll.x == ur.x)
        {
		//If vertical, check which point is more to the left
                if (p.x < s.x)
                {
			//Calculate the intersection using slope intercept form
                        float m = (p.y - s.y) / (p.x - s.x);
                        float B = p.y - (m*p.x);
                        float x = ur.x;
                        float y = m*x + B;
			//Return the intersection
                        Vertex intersection;
                        intersection.x = x;
                        intersection.y = y;
                        return intersection;
                }
                else
                {
			//Same calculation, but the case where s is more to the left
                        float m = (s.y - p.y) / (s.x - p.x);
                        float B = s.y - (m*p.x);
                        float x = ur.x;
                        float y = m*x + B;
                        Vertex intersection;
                        intersection.x = x;
                        intersection.y = y;
                        return intersection;
                }
        }
        else
        {
		//If horizontal, check which point is lower
                if (p.y < s.y)
                {
			//Calculate the intersection using slope intercept form
                        float m = (p.y - s.y) / (p.x - s.x);
                        float B = p.y - (m*p.x);
                        float y = ur.y;
                        float x = (y - B) / m;
			//Return the intersection
                        Vertex intersection;
                        intersection.x = x;
                        intersection.y = y;
                        return intersection;
                }
                else
                {
			//Same calculation, but the case where s is lower
                        float m = (s.y - p.y) / (s.y - p.y);
                        float B = s.y - (m*s.x);
                        float y = ur.y;
                        float x = (y - B) / m;
                        Vertex intersection;
                        intersection.x = x;
                        intersection.y = y;
                        return intersection;
                }
        }
}

//Main algorithm to be run for each edge of the clipping box
//The function takes the number of inputs, a vector of input vertices, a reference to 
//a vector to put the output vertices into, the lower left vertex of the edge, the 
//upper right vertex of the edge, and which side is being worked on (0 for left, 1 for right
//2 for top, and 3 for bottom) 
//The function returns the number of outputs
//The function will populate the output vector with the revised list of vertices within the provided
//edge (this includes intersections with the edge
int SHPC(int in, std::vector<Vertex> inV, std::vector<Vertex>* out, Vertex ll, Vertex ur, int side)
{
	//If no input vertices, return 0
	if (in < 1)
	{
		return 0;
	}

	//Set the previous vertex to be the last vertex
        Vertex p = inV[in - 1];

	//Initialize our output count
	int outLength = 0;	

	//Loop through each input vertex
        for (int j = 0; j < in; ++j)
        {
		//Set the current vertex we are working with
                Vertex s = inV[j];
		//Check if the current vertex is within the edge
                if (inside(s, ll, ur, side))
                {
			//If the previous is within as well, add the current to the ouput
			//and increment our count
                        if (inside(p, ll, ur, side))
                        {
                                out->push_back(s);
                                ++outLength;
                        }
			//Otherwise, we find where line between the two vertices 
			//intersects with the edge, then add the current vertex
			//and the intersection to our output, as well as increment our count
                        else
                      	{
                                Vertex i = intersect(p, s, ll, ur);
                                out->push_back(i);
                                out->push_back(s);
                                outLength += 2;
                        }
                }
		//If the current vertex is not in the area
                else
                {       
			//Check if the previous vertex was in the area
                        if (inside(p, ll, ur, side))
                        {       
				//If so, just add the interection to the output
				//and increment our count accordingly
                                Vertex i = intersect(p, s, ll, ur);
                                out->push_back(i);
                                ++outLength;
                        }
                }
         	//Set the previous to be our current vertex
                p = s;
        }
	//Return the count
        return outLength;
}


///
// clipPolygon
//
// Clip the polygon with vertex count in and vertices inV against the
// rectangular clipping region specified by lower-left corner ll and
// upper-right corner ur. The resulting vertices are placed in outV.
//
// The routine should return the with the vertex count of polygon
// resulting from the clipping.
//
// @param in    the number of vertices in the polygon to be clipped
// @param inV   the incoming vertex list
// @param outV  the outgoing vertex list
// @param ll    the lower-left corner of the clipping rectangle
// @param ur    the upper-right corner of the clipping rectangle
//
// @return number of vertices in the polygon resulting after clipping
//
///

int Clipper::clipPolygon( int in, const Vertex inV[], Vertex outV[],
                          Vertex ll, Vertex ur )
{
	// YOUR CODE GOES HERE
	//Initialize our output count and output vector
	int outLength = 0;
	std::vector<Vertex>* out = new std::vector<Vertex>();	
	
	//Insert the vertices from the input array into an input vector
	std::vector<Vertex> in0;
	in0.insert(in0.begin(), inV, inV + in);
	
	//Set two vertices to be the corners of the left side
	Vertex v1 = {ll.x, ll.y};
	Vertex v2 = {ll.x, ur.y};
	//Run the main algo for the left side
	outLength = SHPC(in, in0, out, v1, v2, 0);

	//Move the output from the previous call to the input vector for the next call
	std::vector<Vertex> in1 = std::vector<Vertex>();
	for (int i = 0; i < out->size(); ++i)
	{
		in1.push_back((*out)[i]);
	}
	//Reset the output pointer to a new empty vector
	delete out;
	out = new std::vector<Vertex>();


	//Repeat the previous steps for the other three sides of the clipping area
	Vertex v3 = {ll.x, ur.y};
	Vertex v4 = {ur.x, ur.y};
	outLength = SHPC(outLength, in1, out, v3, v4, 1);

	

	std::vector<Vertex> in2 = std::vector<Vertex>();
        for (int i = 0; i < out->size(); ++i)
        {
                in2.push_back((*out)[i]);
        }
        delete out;
        out = new std::vector<Vertex>();
	Vertex v5 = {ur.x, ll.y};
	Vertex v6 = {ur.x, ur.y};
	outLength = SHPC(outLength, in2, out, v5, v6, 2);


	std::vector<Vertex> in3 = std::vector<Vertex>();
        for (int i = 0; i < out->size(); ++i)
        {
                in3.push_back((*out)[i]);
        }
        delete out;
        out = new std::vector<Vertex>();

	Vertex v7 = {ll.x, ll.y};
	Vertex v8 = {ur.x, ll.y};
	outLength = SHPC(outLength, in3, out, v7, v8, 3);

	//Copy the output vector into the ouput array
	for (int i = 0; i < outLength; ++i) 
	{
		outV[i] = (*out)[i];
	}
	
	delete out;

	return outLength;  // remember to return the outgoing vertex count!

}
