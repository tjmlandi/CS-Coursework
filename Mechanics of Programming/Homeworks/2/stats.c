//Thomas Landi - CS 243 Mechanics of Programming Homework 2


#include <stdio.h> // printf
#include <math.h> // sqrt




double average(int size, int data[])
{
	double total = 0;
	//Finds the total of all the values in the array
	for (int i = 0; i < size; i++)
	{
		total += data[i];
	}
	//Divides the total by the number of items in the array
	double mean = total / size;
	return mean;
}

double stddev(int size, int data[])
{
	double summation = 0;
	double mean = average(size, data);
	//Squares the differences of all the values and the mean, and adds them up
	for (int i = 0; i < size; i++)
	{
		summation += (data[i] - mean) * (data[i] - mean);
	}
	//Takes the square root of the previous sum, divided by one minus the size
	double StandardDev = sqrt (summation / (size - 1));
	return StandardDev;
}

void histogram(int size, int data[])
{
	int a = 0;
	int aMinus = 0;
	int bPlus = 0;
	int b = 0;
	int bMinus = 0;
	int cPlus = 0;
	int c = 0;
	int cMinus = 0;
	int d = 0;
	int f = 0;
	//Finds the total amount of grades for each letter grade in the data array
	for (int i = 0; i < size; i++)
	{
		if (data[i] >= 92)
		{
			a++;
		}
		else if (data[i] >= 89)
		{
			aMinus++;
		}
		else if (data[i] >= 85)
		{
			bPlus++;
		}
		else if (data[i] >= 82)
		{
			b++;
		}
		else if (data[i] >= 79)
		{
			bMinus++;
		}
		else if (data[i] >= 75)
		{
			cPlus++;
		}
		else if (data[i] >= 72)
		{
			c++;
		}
		else if (data[i] >= 69)
		{
			cMinus++;
		}
		else if (data[i] >= 60)
		{
			d++;
		}
		else if (data[i] >= 0)
		{
			f++;
		}
	}
	//Prints out the table of grades
	printf("A:\t%d\n", a);
	printf("A-:\t%d\n", aMinus);
	printf("B+:\t%d\n", bPlus);
	printf("B:\t%d\n", b);
	printf("B-:\t%d\n", bMinus);
	printf("C+:\t%d\n", cPlus);
	printf("C:\t%d\n", c);
	printf("C-:\t%d\n", cMinus);
	printf("D:\t%d\n", d);
	printf("F:\t%d\n", f);	
}
