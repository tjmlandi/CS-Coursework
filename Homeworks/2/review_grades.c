//Thomas Landi - txl3829 -  CS 243 Mechanics of Programming Homework 2

#include <stdio.h> //printf
#include <string.h> //strlen
#include "stats.h" //average, stddev, histogram

int isdigit(int c);


//Converts a C string to an integer, any digits after a decimal point are omitted
int str_to_int(char str[])
{
	int value = 0;
	for (int i = 0; i < (signed int)strlen(str); i++)
	{
		if (isdigit(str[i]))
		{
			value = (value * 10) + (str[i] - '0');
		}
		else
		{
			break;
		}
	}
	return value;
}

//Prints out total number of grades, mean, standard deviation, 
//and number of each letter grade
//Error message is printed if there are no arguments
int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		printf("usage: review_grades score1 [score2 ...]\n");
		printf("note: score values are non-negative.\n");
		return 1;
	}
	int grades[argc - 1];
	for (int i = 1; i <  argc; i++)
	{
		grades[i - 1] = str_to_int(argv[i]);
	}
	printf("population: %d\n", argc - 1);
	for (int i = 0; i < argc - 1; i++)
	{
		printf("%d ", grades[i]);
	}
	printf("\nmean: %f\n", average(argc - 1, grades));
	printf("std deviation: %f\n", stddev(argc -1, grades));
	histogram(argc - 1, grades);
}
