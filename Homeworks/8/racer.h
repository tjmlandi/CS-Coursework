/*
 * File:$Id: racer.h,v 1.5 2014/12/06 03:02:41 csci243 Exp $
 * Author: James Heliotis
 *
 * Description: a thread controlling a small figure that races across
 *    a character window
 * Revisions:
 *    $Log: racer.h,v $
 *    Revision 1.5  2014/12/06 03:02:41  csci243
 *    Fixed comment about coordinate system.
 *
 *    Revision 1.4  2014/11/22 16:00:02  csci243
 *    Improved comments.
 *
 *    Revision 1.3  2014/11/22 15:13:41  csci243
 *    Changed file names to lower case.
 *
 *    Revision 1.2  2014/11/21 20:33:58  csci243
 *    Changed to use display directly.
 *    Managed memory.
 *
 *    Revision 1.1  2014/11/20 06:08:07  jeh
 *    Initial revision
 *
 *    Revision 1.1  2013/03/14 19:45:17  jeh
 *    Initial revision
 *
 *
 */

#ifndef _RACER_H
#define _RACER_H

#include <stdlib.h>
#include <stdio.h>

#define FINISH_LINE 70
#define MAX_NAME_LEN 9
#define MAX_TIME 200 // msec

typedef struct Rcr {

    // current distance from starting line
    //
    int dist;

    // vertical position of the racer, i.e. "racing lane"
    //
    int row;

    // graphic: the drawable text
    //
    char *graphic;

} Racer;

// Do setup work for all racers at the start of the program.
// Arguments
//   milliseconds: length of pause between steps in animation
//
void initRacers( long milliseconds );

// Create a new racer.
//
// Arguments
//   name: the string name to show on the display for this racer
//         (The string gets copied.)
//   position: the row in which to race
//
// Returns: a dynamically allocated Racer object
//
// Pre: strlen( name ) < MAX_NAME_LEN, for display reasons.
//
Racer *makeRacer( char *name, int position );

// Destroy all dynamically allocated storage for a racer.
//
// Arguments:
//   racer: the object to be de-allocated
//
void destroyRacer( Racer *racer );

// Run one racer in the race.
// Initialize the display of the racer*:
//   The racer starts at the start position, column 1.
//   The racer's graphic (text name ) is displayed.
// This action happens repetitively, until its position is at FINISH_LINE:
//   Randomly calculate a waiting period, no mare than MAX_TIME (rand(3))
//   Sleep for that length of time.
//   Change the display position of this racer by +1 column*:
//     Erase the racer's name from the display.
//     Update the racer's dist field by +1.
//     Display the racer's name at the new position.
//
// The intention is to execute this function many times simultaneously,
// each in its own thread.
//
// *Care is taken to keep the update of the display by one racer "atomic".
//
// Parameter
//   racer: a Racer, declared as void* to be compatable with the
//          pthread interface (see pthread_create)
//
// Pre: racer cannot be NULL.
//
void *run( void *racer );

#endif
