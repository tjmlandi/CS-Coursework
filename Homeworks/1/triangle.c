// 
// Thomas Landi Homework 1-3 CS243 
// 
// A program to print out multiple triangles "right-up" triangles.

#include <stdio.h>


void drawTriangle(int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = size; j > i; j--)
		{
			printf(" ");
		}
		for (int k = 0; k <= i; k++)
		{
			printf("*");
		}
		printf("\n");
	}
}

int main()
{
	drawTriangle(1);
	drawTriangle(4);
	drawTriangle(6);
	return 0;
}
