//
//  Rasterizer.cpp
//
//  Created by Joe Geigel on 11/30/11.
//  Modifications by Warren R. Carithers.
//  Copyright 2011 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Thomas Landi
//

#include <list>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>

#include "Rasterizer.h"
#include "Canvas.h"

using namespace std;

///
// Simple class that performs rasterization algorithms
///

///
// Constructor
//
// @param n number of scanlines
// @param C The Canvas to use
///
Rasterizer::Rasterizer( int n, Canvas &canvas ) : n_scanlines(n), C(canvas)
{
}

//Struct for storing edge data
struct Edge {
        int yMax;
        int x;
        bool negativeSlope;
        int dx;
        int dy;
        int sum;
        Edge* nextEdge;
};

//Comparison function for edge structs, so they can be sorted
//by x value, then slope
bool EdgeCompare (Edge* lhs, Edge* rhs) {
        if (lhs->x != rhs->x) {
                return lhs->x < rhs->x;
        }
        return (lhs->dx / lhs->dy) < (rhs->dx / rhs->dy);
}


///
// Draw a filled polygon.
//
// Implementation should use the scan-line polygon fill algorithm
// discussed in class.
//
// The polygon has n distinct vertices.  The coordinates of the vertices
// making up the polygon are stored in the x and y arrays.  The ith
// vertex will have coordinate (x[i],y[i]).
//
// You are to add the implementation here using only calls to the
// setPixel() function.
//
// @param n - number of vertices
// @param x - array of x coordinates
// @param y - array of y coordinates
///
void Rasterizer::drawPolygon(int n, const int x[], const int y[] )
{
	vector<Edge*> ActiveEdges = vector<Edge*>();
	int lowestY = INT_MAX;
	int highestY = 0;

	//Find the lowest and highest Y values, so we know 
	//how big our edge table needs to be
	for (int i = 0; i < n; ++i) {
		if (y[i] < lowestY) {
			lowestY = y[i];
		}
		if (y[i] > highestY) {
			highestY = y[i];
		}
	}
	
	int polyHeight = highestY - lowestY;		

	//Declare and initialize the edge table
	Edge** EdgeTable = new Edge*[polyHeight + 1];
	for (int i = 0; i <= polyHeight; ++i) {
		EdgeTable[i] = NULL;
	}
	
	//Loop through each 'scanline' for the polygon
	//so we can fill the edge table
	for (int i = 0; i <= polyHeight; ++i) {
		//For a scanline, loop through each edge provided
		for (int j = 0; j < n; j += 1) {
			//Two cases, depending on which vertex (the current one, or the next one)
			//has a higher Y value
			int nextEdge = (j + 1 == n) ? 0 : j + 1;
			if (y[j] < y[nextEdge]) {
				int ymin = y[j];
				int xmin = x[j];
				int ymax = y[nextEdge];
				int xmax = x[nextEdge];
				//If the lower y is on the scanline, add the edge to the
				//edge table at the corresponding index, adding it to
				//the end of the list of edges at that index
				if (ymin == i + lowestY) {
					Edge* newEdge = new Edge();	
					newEdge->yMax = ymax;
					if ((ymax - ymin) * (xmax - xmin) < 0) {
						newEdge->negativeSlope = true;
					}
					else {
						newEdge->negativeSlope = false;
					}
					newEdge->x = xmin;
					newEdge->dx = abs(xmax - xmin);
					newEdge->dy = abs(ymax - ymin);
					newEdge->sum = 0;
					newEdge->nextEdge = NULL;
					if (EdgeTable[i] == NULL) {
						EdgeTable[i] = newEdge;
					}
					else {
						Edge* current = EdgeTable[i];
						if (newEdge->x < current->x) {
							EdgeTable[i] = newEdge;
							newEdge->nextEdge = current;
						}
						else {
							while (current->nextEdge != NULL && newEdge->x < current->nextEdge->x) {
								current = current->nextEdge;
							}
							newEdge->nextEdge = current->nextEdge;
							current->nextEdge = newEdge;
						}
					}
				}
			}
			//Same as above, but for other max y case
			else {
				int ymin = y[nextEdge];
                                int xmin = x[nextEdge];
                                int ymax = y[j];
                                int xmax = x[j];
                                if (ymin == i + lowestY) {
                                        Edge* newEdge = new Edge();     
                                        newEdge->yMax = ymax;
                                        newEdge->x = xmin;
                                        newEdge->dx = abs(xmax - xmin);
                                        newEdge->dy = abs(ymax - ymin);
                                        newEdge->sum = 0;
                                        newEdge->nextEdge = NULL;
					if (EdgeTable[i] == NULL) {
						EdgeTable[i] = newEdge;
					}
					else {
						Edge* current = EdgeTable[i];
						if (newEdge->x < current->x) {
							EdgeTable[i] = newEdge;
							newEdge->nextEdge = current;
						}
						else {
							while (current->nextEdge != NULL && newEdge->x < current->nextEdge->x) {
							current = current->nextEdge;
							}
							newEdge->nextEdge = current->nextEdge;
							current->nextEdge = newEdge;
						}
					}
                                }

			}
		}
	}
	
			
	for (int i = 0; i < polyHeight; ++i) {
		cout << "Line: " << (i + lowestY) << "\n";
		if (EdgeTable[i] != NULL) {
			Edge* current = EdgeTable[i];
			while (current != NULL) {
				cout << "yMax: " << current->yMax << ", x: " << current->x << "\n";
				current = current->nextEdge;
			}
			cout << "\n";
		}
		else {
			cout << "empty\n";
		}
	}
	
	
	int yIndex = lowestY;
	bool stillEdges = true;	
	
	//Loop while either the edge table or active table have edges
	while (stillEdges) {
		cout << "test5\n";
		vector<int> toDelete = vector<int>();
		//Find any edges we are done with in the active edges and delete it
		for (int i = 0; i < ActiveEdges.size(); ++i) {
			cout << "test5.1\n";
			if (ActiveEdges[i] != NULL) {
				Edge* edge = ActiveEdges[i];
				if (edge->yMax == yIndex) {
					ActiveEdges[i] = edge->nextEdge;
					delete edge;
					edge = ActiveEdges[i];
				}
				cout << "test5.2\n";
				while (edge != NULL && edge->nextEdge != NULL) {
					cout << "test5.3\n";
					if (edge->nextEdge->yMax == yIndex) {
						edge->nextEdge = edge->nextEdge->nextEdge;
						delete edge->nextEdge;
					}
					cout << "test5.4\n";
					edge = edge->nextEdge;
				}
				cout << "test5.5\n";
			}
		}
		
		cout << "size2: " << ActiveEdges.size() << "\n";
		for (int i = 0; i < ActiveEdges.size(); ++i) {
			if (ActiveEdges[i] == NULL) {
				toDelete.push_back(i);
				cout << "Deleting: " << i << "\n";
			}
		}

		for (int i = 0; i < toDelete.size(); ++i) {
			ActiveEdges.erase(ActiveEdges.begin(), ActiveEdges.begin() + toDelete[i]);
			cout << "Deleted: " << toDelete[i] << "\n";
		}

		for (int i = 0; i < ActiveEdges.size(); ++i) {
			if (ActiveEdges[i] == NULL) {
				cout << "WHAT" << i << "\n";
			}
		}
		
		cout << "size1: " << ActiveEdges.size() << "\n";

		cout << "test6\n";
		//Add any edges that we have reached the minY for to the active table

		cout << "what: " << EdgeTable[70]->yMax << "\n";
		if ((yIndex - lowestY) < polyHeight && (yIndex - lowestY) >= 0) {
			if (EdgeTable[yIndex - lowestY] != NULL) {
				cout << EdgeTable[yIndex - lowestY];
				ActiveEdges.push_back(EdgeTable[yIndex - lowestY]);
				EdgeTable[yIndex - lowestY] = NULL;
			}
		}
		cout << "test7\n";
		cout << "yIndex: " << yIndex << "\n";
		cout << "size: " << ActiveEdges.size() << "\n";
		for (int i = 0; i < ActiveEdges.size(); ++i) {
			Edge* edge = ActiveEdges[i];
			cout << edge << "\n";
			while (edge != NULL) {
				cout << "yMax: " << edge->yMax << ", x: " << edge->x << "\n";
				edge = edge->nextEdge;
			}
		}
		cout << "test0.1\n";	
		//Sort the Active Edges 	
		sort(ActiveEdges.begin(), ActiveEdges.end(), EdgeCompare);
		cout << "test0.2\n";
		cout << "test0.2\n";
		//Find all the x intersections for the current scanline
		//with the current active edges
		vector<int> intersections = vector<int>();		
		for (int i = 0; i < ActiveEdges.size(); ++i) {
			Edge* edge = ActiveEdges[i];
			while (edge != NULL) {
				intersections.push_back(edge->x);
				edge = edge->nextEdge;
			}
		}
		
		cout << "test\n";
		//Fill in the scanline between the pairs of intersections
		for (int i = 0; i < intersections.size() - 1; i += 2) {
			for (int j = intersections[i]; j < intersections[i + 1]; ++j) {
				C.setPixel(j, yIndex);
			}
		}
		
		cout << "test1\n";
		//Increment the scanline
		++yIndex;
		
		//Increment the x values of all the active edges,
		//using the integer slop method
		for (int i = 0; i < ActiveEdges.size(); ++i) {
			Edge* edge = ActiveEdges[i];
			while (edge != NULL) {
				if (edge->dx != 0 && edge->dy != 0) {
					edge->sum += edge->dx;
					while (edge->sum >= edge->dy) {
						edge->x += (edge->negativeSlope) ? -1 : 1;
						edge->sum -= edge->dy;
					}
				}
				edge = edge->nextEdge;
			}
		}
	
		cout << "test2\n";
		//Find out if there are any edges left
		stillEdges = false;
		if (ActiveEdges.size() > 0) {
			stillEdges = true;
		}
		for (int i = 0; i < polyHeight; ++i) { 
			if (EdgeTable[i] != NULL) {
				stillEdges = true;
			}
		}
		cout << "test4\n";
	}
}

