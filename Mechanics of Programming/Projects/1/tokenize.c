//Thomas Landi - CS243 Mechanics of Programming - Project 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "classes.h"

int isdigit(int c);
int str_to_int(char str[]);
char* strtok(char *str, const char* delim);
int sscanf(const char *str, const char *format, ...);
void matrix_creation(int tm[][12][2], int Ns, FILE *fp, char* ptr, char buf[]); 
int classify(char c);
int scanner(int S, int A, int tm[][12][2]);
int getchar(void);
int isascii(int c);

int main(int argc, char* argv[])
{
	//State machine variables
	int Ns;
	int S;
	int A;

	//Check for TM file
	if (argc <= 1)
	{
		printf("usage: ./tokenize tm_file\n");
		return 1;
	}	
	else
	{
		//Open TM file
		FILE *fp;
		fp = fopen(argv[1], "r");
		if (fp == NULL)
		{
			perror(argv[1]);
			exit(1);
		}
		char* ptr, buf[256];
		
	

		
		//Sets the number of states in the matrix
		ptr = fgets(buf, 256, fp);
		int i = 0;
		while (isdigit(ptr[i]) == 0)
		{
			i += 1;
		}
		char nsstr[2] = {ptr[i], ptr[i + 1]};
		Ns = str_to_int(nsstr);
		
		
		//Sets the number of the start state
		ptr = fgets(buf, 256, fp);
		i = 0;
		while (isdigit(ptr[i]) == 0)
		{
			i += 1;
		}
		char sstr[2] = {ptr[i], ptr[i + 1]};
		S = str_to_int(sstr);
		
		
		//Sets the number of the accepting state
		ptr = fgets(buf, 256, fp);
		i = 0;
		while (isdigit(ptr[i]) == 0)
		{
			i += 1;
		}
		char astr[2] = {ptr[i], ptr[i + 1]};
		A = str_to_int(astr);
		
		//Creates the transistion matrix
		int tm[Ns][12][2];
		matrix_creation(tm, Ns, fp, ptr, buf);
		
		fclose(fp);
		
		//Scans the text file
		while (scanner(S, A, tm) > 0);
		return 0;
	}
}


///
///Function in charge of scanning the text file
///
int scanner(int S, int A, int tm[][12][2])
{
	char token[256]; //Saved token
	int toReturn = 0; //Returns 0 when EOF is reached, otherwise returns 1
	int bufLoc = 0; //Curent location in token buffer
	int curState = S; 
	char c;
	int class;
	
	//Loops while the current character is not EOF, error code, or accepting state
	//Uses transition matrix to build token, if current token is valid
	while (1)
	{
		c = getchar();
		printf("%d ", curState);
		class = classify(c);		
		if (tm[curState][class][1] == 1)
		{
			token[bufLoc] = c;
			bufLoc += 1;
		}
		curState = tm[curState][class][0];
		if (class == 10)
		{
			toReturn = 0;
			break;
		}
		if (curState == A)
		{
			toReturn = 1;
			break;
		}
		if (curState == 99)
		{	
			toReturn = 1;
			break;
		}
	}
	printf("%d ", curState);
	//Returns zero if end of file
	if (class == 10)
	{
		printf("EOF\n");
		return 0;
	}
	//Prints rejected if invalid token
	else if (curState == 99)
	{
		printf("rejected\n");
		while((c = getchar()) != ' ');
	}
	//Prints token if valid
	else
	{
		printf("recognized '");
		for (int i = 0; i < bufLoc; ++i)
		{
			printf("%c", token[i]);
		}
		printf("'\n");
	}
	return toReturn;
}

///
///Returns the class the of character passed in
///
int classify(char c)
{
	int toReturn;
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'))
	{
		toReturn = 2;
	}
	else if (c == EOF)
	{
		toReturn = 10;
	}
	else if (isascii(c) == 0)
	{
		toReturn = 11;
	}
	
	else
	{
		switch (c)
		{
		case ' ':
			toReturn =  0;
			break;
		case '\t':
			toReturn = 0;
			break;
		case '\n':
			toReturn = 1;
			break;
		case '0':
			toReturn = 3;
			break;
		case '1':
			toReturn = 4;
			break; 
		case '2':
			toReturn = 4;
			break;
		case '3':
			toReturn = 4;
			break; 
		case '4':
			toReturn = 4;
			break; 
		case '5':
			toReturn = 4;
			break; 
		case '6':
			toReturn = 4;
			break; 
		case '7':
			toReturn = 4;
			break;
		case '8':
			toReturn = 5;
			break;
		case '/':
			toReturn = 6;
			break;
		case '*':
			toReturn = 7;
			break;
		case '+':
			toReturn = 8;
			break;
		case '-':
			toReturn = 8;
			break;
		case '%':
			toReturn = 8;
			break;
		default:
			toReturn = 9;
		}
	}
	return toReturn;
}

///
///Creates a transistion matrix based on the input file
///
void matrix_creation(int tm[][12][2], int Ns, FILE *fp, char* ptr, char buf[])
{

		//Transition matrix declration - setting all values to error values of 99 and -1
		for (int i = 0;i < Ns; ++i)
		{
			for (int j = 0; j < 12; ++j)
			{
				tm[i][j][0] = 99;
				tm[i][j][1] = -1;
			}
		} 

		
		//Populates the transition matrix based on the input file
		while ((ptr = fgets(buf, 256, fp)) != NULL)
		{
			char delim[2] = {"  "};
			char* tokState = strtok(ptr, delim);
			int state = str_to_int(tokState);
			char* token;
			while (( token = strtok(NULL, delim)) != NULL)
			{
				int class;
				int transState;
				char action;				
				char slash;
				sscanf(token, "%d %c %d %c", &class, &slash, &transState, &action);
				tm[state][class][0] =  transState;
				if (action == 's')
				{
					tm[state][class][1] = 1;
				}
				else
				{
					tm[state][class][1] = 0;
				}
			}
		}

		//Prints out the saved matrix
		printf("Scanning using the following matrix:\n");
		printf("     0");
		printf("    1");
		printf("    2");
		printf("    3");
		printf("    4");
		printf("    5");
		printf("    6");
		printf("    7");
		printf("    8");
		printf("    9");
		printf("   10");
		printf("   11\n");
		for (int i = 0; i < Ns; ++i)
		{
			printf("%2d", i);
			for (int j = 0; j < 12; ++j)
			{
				printf("  %2d", tm[i][j][0]);
				if (tm[i][j][1] == 1)
				{
					printf("s");
				}
				else
				{
					printf("d");
				}
			}
			printf("\n");
		}	
}


///
///Converts pased in strings to integers
///
int str_to_int(char str[])
{
	int value = 0;
	for (int i = 0; i < (signed int)strlen(str); ++i)
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
