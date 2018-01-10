/*
 * File: racer.c
 * Author: YOUR NAME HERE
 *
 * Description: a thread controlling a small figure that races across
 *    a character window
 *
 */

#if __STDC_VERSION__ >= 199901L
#   define _XOPEN_SOURCE 600
#else
#   define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "display.h"
#include "racer.h"

static int timer;
//Mutex variable
static pthread_mutex_t consoleMutex;

void initRacers(long milliseconds)
{
	timer = milliseconds;
}

//Creates a racer with the given name and position
Racer *makeRacer( char *name, int position )
{
	//Allocates the space for the racer
	Racer* toReturn = malloc(sizeof(Racer));
	toReturn->dist = 1;
	toReturn->row = position;
	//Allocates the space for the graphic and copies it in
	toReturn->graphic = malloc(sizeof(name) + 1);
	strcpy(toReturn->graphic, name);
	return toReturn;
}

//Frees the memory used by the given racer
void destroyRacer( Racer *racer)
{
	free(racer->graphic);
	free(racer);
}

//Races the given racer across the screen
void *run( void *racer )
{
	Racer* racerX = racer;
	int graphLength = strlen(racerX->graphic);
	//Loops while the racer is not at the end
	while (racerX->dist < FINISH_LINE)
	{
		//Locks the mutex and draws the racer
		pthread_mutex_lock(&consoleMutex);
		for (int i = 0; i < graphLength; ++i)
		{
			set_cur_pos(racerX->row, racerX->dist + i);
			put(racerX->graphic[i]);
		}
		pthread_mutex_unlock(&consoleMutex);
		//Sleeps before locking the mutex again and erasing the racer
		unsigned int sleepTime = (rand() % MAX_TIME) * 1000;
		usleep(sleepTime);
		pthread_mutex_lock(&consoleMutex);
		for (int i = 0; i < graphLength; ++i)
		{
			set_cur_pos(racerX->row, racerX->dist + i);
			put(' ');
		}
		pthread_mutex_unlock(&consoleMutex);
		//Advances the racer
		racerX->dist += 1;
	}
	return 0;
}
