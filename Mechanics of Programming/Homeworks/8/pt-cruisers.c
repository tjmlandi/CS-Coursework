/*
 * Filename: pt-cruisers.c
 * Author: YOUR NAME HERE
 *
 * Description: A demonstration of the use of threads.
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "display.h"
#include "racer.h"

// Name: main
//
// Arguments
//   argc: int containing number of command line arguments
//   argv: array of strings containing...
//             - (optional) base 10 character representation of an
//                 int seed value for random number generator (srand(3))
//             - string names of Racers
//
// Run a race with a bunch of Racers
// Examples:
//   ./pt-cruisers Jack Jill # races Jack against Jill
//   ./pt-cruisers 90125 Manny Moe Jack # races three Racers with seed 90125
//
// If the first argument represents a non-integer numeric value, the
// decimal digits at the start of the argument string are used as an
// integer seed and the rest is ignored. (See sscanf.)
// A usage error on Standard Error results if no racer names are given. The
// format is
//   fprintf( stderr, "Usage: %s [ random-seed ] names...\n", argv[ 0 ] );
//
// Post: All memory allocated in this program has been freed.
//
int main( int argc, char *argv[] )
{
	//Checks to make sure there are arguments to make into racers	
	if (argc < 2)
	{
		printf("Error: Need racers\n");
		return 1;
	}
	int numRacers = 0;
	Racer** racers;
	//Checks to see if the first argument is a number to use as a seed
	bool isInt = true;
	for (unsigned int i = 0; i < strlen(argv[1]); ++i)
	{
		if (isdigit(argv[1][i]) == 0 || argv[1][i] == '.')
		{
			isInt = false;
		}
	}
	//If no seed is given, an array of racers is created with the arguments
	if (isInt == false)
	{
		numRacers = argc - 1;
		racers = malloc(sizeof(Racer*) * (numRacers));
		for (int i = 0; i < numRacers; ++i)
		{
			racers[i] = makeRacer(argv[i + 1], i + 1);
		}
	}
	//If there is a seed, it is used, and then the array of racers is created
	else 
	{
		numRacers = argc - 2;
		char *ptr;
		long seed = strtol(argv[1], &ptr, 10);
		srand(seed);
		racers = malloc(sizeof(Racer) * (numRacers));
		for (int i = 0; i < numRacers; ++i)
		{
			racers[i] = makeRacer(argv[i + 2], i + 1);
		}
	}
	clear();
	//Creates an array of threads
	pthread_t threads[numRacers];
	//Creates a thread for each racer, using the run function and the racer as the parameter
	for (int i = 0; i < numRacers; ++i)
	{
		if (pthread_create(&threads[i], NULL, run, racers[i])) printf("failed to create thread");
	}
	//Joins all the threads
	for (int i = 0; i < numRacers; ++i)
	{
		pthread_join(threads[i], NULL);
	}
	//Free all the racers and the array
	for (int i = 0; i < numRacers; ++i)
	{
		destroyRacer(racers[i]);
	}
	free(racers);
	return 0;
}
