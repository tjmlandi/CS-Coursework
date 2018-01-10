// Author: Thomsa Landi


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/wait.h>

//Function Prototypes
char* remove_quotes(char* commandLine, int lineLength);
int execute_command(char* command);
int mysh_history(int argc, char* argv[]);
int mysh_bang(int argc,char* argv[]);
int mysh_help(int argc, char* argv[]);

//Command nodes used to store entry history
typedef struct CommandNode
{
	int numCommand;
	char* command;
} CommandNode;


//Global values
static int lineNum = 1;
static char input[256];
static int historyNum = 10;
static CommandNode** Commands;
static int numCommands = 0;
static char history[] = {'h', 'i', 's', 't', 'o', 'r', 'y', '\0'};
static char help[] = {'h', 'e', 'l', 'p', '\0'};
static char quit[] = {'q', 'u', 'i', 't', '\0'};
static char verbose[] = {'v', 'e', 'r', 'b', 'o', 's', 'e', '\0'};
static bool diagnostic = false;

int main(int argc, char* argv[])
{
	//Handles -v and -h tages if there are any
	int c;
	char* ptr;
	int hist;
	while ((c = getopt (argc, argv, "vh:")) != -1)
	{
		switch (c)
		{
		case 'v':
			diagnostic = true;
			printf("diagnostic enable\n");
			break;
		case 'h':
			hist = strtol(optarg, &ptr, 10);
			if (hist <= 0) 
			{
				fprintf(stderr, "usage: mysh [-v] [-h pos_num]\n");
				return 1;
			}	
			historyNum = hist;
		}	
	}	
	
	//Initializes the array of past commands
	Commands = malloc(sizeof(CommandNode*) * historyNum);
	for (int i = 0; i < historyNum; ++i)
	{
		Commands[i] = NULL;
	}	

	//Main input loop
	while(1)
	{
		//Displays shell and gets user input
		printf("mysh[%d]", lineNum);
		if (fgets(input, sizeof(input), stdin) == NULL)
		{
			printf("\n");
			exit(EXIT_SUCCESS);
		}
		printf("\n");
		
		//If verbose is enabled, print out command info
		if (diagnostic == true)
		{
			char* temp = malloc(sizeof(input) + 1);
			strcpy(temp, input);
			char* tokens[strlen(input)];
			int i = 0;
			while ((tokens[i] = remove_quotes(input, strlen(input))) != NULL ) i += 1;
			char delim[] = {' '};
			tokens[i] = strtok(input, delim);
			i += 1;
			while ((tokens[i] = strtok(NULL, delim)) != NULL) i += 1;
			printf("input command tokens:\n");
			for (int j = 0; j < i; ++j)
			{
				printf("\t%d: %s\n", j, tokens[j]);
			}
			free(temp);
		}
		//Calls main command exectuion
		if (execute_command(input) == 0)
		{
			//Adds commands to comands list
			if (numCommands < historyNum)
			{
				Commands[numCommands] = malloc(sizeof(CommandNode));
				Commands[numCommands]->numCommand = lineNum;
				Commands[numCommands]->command = malloc(sizeof(input) + 1);
				strcpy(Commands[numCommands]->command, input);
				numCommands += 1;
			}
			else 
			{
				free(Commands[0]->command);
				free(Commands[0]);
				for (int i = 0; i < 9; ++i)
				{
					Commands[i] = Commands[i + 1];
				}
				Commands[historyNum - 1] = malloc(sizeof(CommandNode));
				Commands[historyNum - 1]->numCommand = lineNum;
				Commands[historyNum - 1]->command = malloc(sizeof(input) + 1);
				strcpy(Commands[historyNum - 1]->command, input);
				numCommands += 1;
			}
		lineNum += 1;
		}
	}
	return 0;
}

//Prints out the recent shell history
int mysh_history(int argc, char* argv[])
{
	//Warning hanndling
	//-------------------------------------
	int nothing = argc;
	char* whatever = argv[0];
	nothing = strcmp(whatever, argv[0]);
	nothing += 1;
	//-------------------------------------

	//For loop through all commands to print them
	for (int i = 0; i < numCommands; ++i)
	{
		printf("\t%d: ", Commands[i]->numCommand);
		for (unsigned int j = 0; j < strlen(Commands[i]->command); ++j)
		{
			printf("%c", Commands[i]->command[j]);
		}
		printf("\n");
	}
	return 0;
}

//Reruns the specified command
int mysh_bang(int argc,char* argv[])
{
	//Removes the ! from the command and turns it into an integer
	char* command = argv[0];
	memmove(command, command + 1, strlen(command));
	char* ptr;
	int num = strtol(command, &ptr, 10);
	
	//Handling index out of bounds error
	if (num < Commands[0]->numCommand || num > Commands[numCommands - 1]->numCommand)
	{
		fputs("Index out of range\n", stderr);
	}
	else
	{
		//Executes command
		printf("Command: %d\n", num);
		int toExec;
		for (int i = 0; i < numCommands; ++i)
		{
			if (Commands[i]->numCommand == num) toExec = i;
		}
		execute_command(Commands[toExec]->command);
	}

	//Warning Handling
	//------------------------------------
	int nothing = argc;
	char* whatever = argv[0];
	nothing = strcmp(whatever, argv[0]);
	nothing += 1;
	//------------------------------------

	return 0;	
}


//Prints out the list of available commands
int mysh_help(int argc, char* argv[])
{
	printf("Available Commands:\n\t!N: Re-execute the Nth command in the history list\n");
	printf("\thelp: View the list of internal commands\n");
	printf("\thistory: View the list of the last ten commands entered\n");
	printf("\tquit: Exit the shell\n");
	printf("\tVerbose on/off: Turn the shell verbose on or off\n");
	int nothing = argc;
	char* whatever = argv[0];
	nothing = strcmp(whatever, argv[0]);
	nothing += 1;
	return 0;
}


//Quits the terminal
int mysh_quit(int argc, char* argv[])
{
	int nothing = argc;
	char* whatever = argv[0];
	nothing = strcmp(whatever, argv[0]);
	nothing += 1;
	printf("Goodbye\n");
	exit(EXIT_SUCCESS);
	return 0;
}

//Turns verbose mode on or off
int mysh_verbose(int argc, char* argv[])
{
	char delim[] = {' '};
	//Checking on or off specified
	char* state = strtok(NULL, delim);
	char off[] = {'o', 'f', 'f', '\0'};
	char on[] = {'o', 'n', '\0'};
	printf("%d, %s\n", strcmp(state, off), state);
	//Sending appropriate response
	if (state == NULL)
	{
		printf("usage: verbose [on|off]\n");
	}
	else if (strcmp(state, off) == 0)
	{
		diagnostic = false;
		printf("Verbose mode: Off\n");
	}
	else if (strcmp(state, on) == 0)
	{
		diagnostic = true;
		printf("Verbose mode: On\n");
	}
	else
	{
		printf("usage: verbose [on|off]\n");
	}
	int nothing = argc;
	char* whatever = argv[0];
	nothing = strcmp(whatever, argv[0]);
	nothing += 1;
	return 0;
}


//Checks entered command and executes possible internal commands, otherwise runs separate proccess with 
//external command
int execute_command(char* input)
{
	char delim[] = {' '};
	input[strlen(input) - 1] = ' ';
	char* command = strtok(input, delim);
	//Checking for blank commands
	if (command == NULL)
	{
		return 1;
	}
	if (command[0] == EOF)
	{
		return 2;
	}
	//Check possible internals
	if (strcmp(command, history) == 0)
	{
		return mysh_history(1, &command);
	}
	else if (command[0] == '!' && isdigit(command[1]) != 0)
	{
		return mysh_bang(1, &command);
	}
	else if (strcmp(command, help) == 0)
	{
		return mysh_help(1, &command);
	}
	else if (strcmp(command, quit) == 0)
	{
		for (int i = 0; i < numCommands; ++i)
		{
			if (Commands[i] != NULL)
			{
				free(Commands[i]->command);
				free(Commands[i]);
			}
		}
		return mysh_quit(1, &command);
	}
	else if (strcmp(command, verbose) == 0)
	{
		return mysh_verbose(1, &command);
	}
	//Go to external commands 
	else 
	{
		pid_t child;
		if ((child = fork()) != 0)
		{
			int status;
			wait(&status);
		}
		else
		{
			if (system(input) != -1)
			{
				exit(EXIT_SUCCESS);
			}
			else 
			{
				exit(EXIT_FAILURE);
			}
		}
		printf("\n");
	}
	return 0;	
}


//Removes first quoted section from entered string and returns it as a string
char* remove_quotes(char* commandLine, int lineLength)
{
	char* toReturn = malloc((8 * sizeof(lineLength)) + 1);
	bool isQuotes = false;
	for (int i = 0; i < lineLength; ++i)
	{
		if (commandLine[i] == '\"')
		{
			isQuotes = true;
			int j = i + 1;
			commandLine[i] = ' ';
			while (commandLine[j] != '\"')
			{
				toReturn[j - i - 1] = commandLine[j];
				commandLine[j] = ' ';
				j += 1;
			}
			commandLine[j] = ' ';
			break;
		}
		if (commandLine[i] == '\'')
		{
			isQuotes = true;
			int j = i + 1;
			commandLine[i] = ' ';
			while (commandLine[j] != '\'')
			{
				toReturn[ j - i - 1] = commandLine[j];
				commandLine[j] = ' ';
				j += 1;
			}
			commandLine[j] = ' ';
			break;
		} 
	}
	if (isQuotes) return toReturn;
	return NULL;
}
