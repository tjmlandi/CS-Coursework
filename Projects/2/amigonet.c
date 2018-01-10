#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "amigonet.h"


struct Friends_struct
{
    int numFriends;
    User** friends;
};

typedef struct userNode
{
    User* user;
    struct userNode* next;
} userNode;

static userNode* rootAmigo;

//Creates Amignet
void create_amigonet()
{
    //Sets the root amigo
    rootAmigo = NULL;
}

//Frees all the memory used by amigonet
void destroy_amigonet()
{
    userNode* current;
    userNode* next;
    current = rootAmigo;
    next = rootAmigo;
    while(current != NULL)
    {
        next = current->next;
        free((char*)current->user->name);
        free(current->user->amigos->friends);
        current = next;
    }
}

//Adds a user to amigonet
void addUser(const char* name)
{
    userNode* current;
    current = rootAmigo;
    //Checks to see if the user exists, if so, does nothing
    while (current != NULL)
    {
        if (strcmp(current->user->name, name) == 0)
        {
            return;
        }
        current = current->next;
    }
    //Allocates the tmemory for the new user
    current = rootAmigo;
    while (current != NULL) current = current->next;
    current = malloc(sizeof(*current));
    current->next = NULL;
    current->user = malloc(sizeof(*current->user));
    current->user->name = malloc(strlen(name) + 1);
    strcpy((char*)current->user->name, (char*)name);
    current->user->amigos = malloc(sizeof(*current->user->amigos));
    current->user->amigos->numFriends = 0;
    current->user->amigos->friends = NULL;
    rootAmigo = current;
}

//Find user by name
User *findUser(const char *name)
{
    User* toReturn = NULL;
    userNode* current = rootAmigo;
    //Finds the user, if does nto exist, return NULL
    while (current != NULL)
    {
        if (strcmp(current->user->name, name) == 0)
        {
            toReturn = current->user;
        }
        current = current->next;
    }
    return toReturn;
}

//Links the two amigos
void addAmigo(User *user, User *amigo)
{
    for (int i = 0; i < user->amigos->numFriends; ++i)
    {
        if (strcmp(user->amigos->friends[i]->name, amigo->name) == 0)
        {
            return;
        }
    }
    //Creates a friend list for either amigo if they do not have one, otherwise adds the link to the existing list(s)
    if (user->amigos->numFriends == 0)
    {
        if (amigo->amigos->numFriends == 0)
        {
            user->amigos->friends = malloc(sizeof(User*));
            user->amigos->friends[0] = amigo;
            amigo->amigos->friends = malloc(sizeof(User*));
            amigo->amigos->friends[0] = user;
        }
        else
        {
            user->amigos->friends = malloc(sizeof(User*));
            user->amigos->friends[0] = amigo;
            User** newAmigoList = malloc((amigo->amigos->numFriends + 1) * sizeof(User*));
            for (int i = 0; i < amigo->amigos->numFriends; ++i)
            {
                newAmigoList[i] = amigo->amigos->friends[i];
            }
            free(amigo->amigos->friends);
            amigo->amigos->friends = newAmigoList;
            amigo->amigos->numFriends += 1;
            amigo->amigos->friends[amigo->amigos->numFriends - 1] = user;
        }
    }
    else
    {
	if (amigo->amigos->numFriends == 0)
	{
	 amigo->amigos->friends = malloc(sizeof(User*));
         amigo->amigos->friends[0] = user;
        
	 User** newUserList = malloc((user->amigos->numFriends + 1) * sizeof(User*));
      	 for (int i = 0; i < user->amigos->numFriends; ++i)
       	 {
            newUserList[i] = user->amigos->friends[i];
         }
     	 free(user->amigos->friends);
       	 user->amigos->friends = newUserList;
       	 user->amigos->numFriends += 1;
       	 user->amigos->friends[user->amigos->numFriends - 1] = amigo;
    	}
	else 
	{
        User** newAmigoList = malloc((amigo->amigos->numFriends + 1) * sizeof(User*));
        for (int i = 0; i < amigo->amigos->numFriends; ++i)
        {
            newAmigoList[i] = amigo->amigos->friends[i];
        }
        free(amigo->amigos->friends);
        amigo->amigos->friends = newAmigoList;
        amigo->amigos->numFriends += 1;
        amigo->amigos->friends[amigo->amigos->numFriends - 1] = user;

        User** newUserList = malloc((user->amigos->numFriends + 1) * sizeof(User*));
        for (int i = 0; i < user->amigos->numFriends; ++i)
        {
            newUserList[i] = user->amigos->friends[i];
        }
        free(user->amigos->friends);
        user->amigos->friends = newUserList;
        user->amigos->numFriends += 1;
        user->amigos->friends[user->amigos->numFriends - 1] = amigo;
    	}
    }
}

//De links the given amigos
void removeAmigo(User* user, User* ex_amigo)
{
    //Checks to see if they are amigos
    bool areNotFriends = true;
    for (int i = 0; i < user->amigos->numFriends; ++i)
    {
        if (strcmp(user->amigos->friends[i]->name, ex_amigo->name) == 0)
        {
            areNotFriends = false;
        }
    }
    if (areNotFriends) return;

    //Removes one from the other's list
    User** newUserList = malloc((user->amigos->numFriends -1) * sizeof(User*));
    int i = 0;
    while (strcmp(user->amigos->friends[i]->name, ex_amigo->name) != 0)
    {
        newUserList[i] = user->amigos->friends[i];
        ++i;
    }
    for (; i < user->amigos->numFriends - 1; ++i)
    {
        newUserList[i] = user->amigos->friends[i + 1];
    }
    user->amigos->numFriends -= 1;
    free(user->amigos->friends);
    if (user->amigos->numFriends == 0)
    {
        user->amigos->friends = NULL;
        free(newUserList);
    }
    else
    {
        user->amigos->friends = newUserList;
    }

    //Removes the other from one's list
    User** newAmigoList = malloc((ex_amigo->amigos->numFriends -1) * sizeof(User*));
    int j = 0;
    while (strcmp(ex_amigo->amigos->friends[i]->name, user->name) != 0)
    {
        newAmigoList[j] = ex_amigo->amigos->friends[j];
        ++j;
    }
    for (; j < ex_amigo->amigos->numFriends - 1; ++j)
    {
        newAmigoList[i] = ex_amigo->amigos->friends[j + 1];
    }
    ex_amigo->amigos->numFriends -= 1;
    free(ex_amigo->amigos->friends);
    if (ex_amigo->amigos->numFriends == 0)
    {
        ex_amigo->amigos->friends = NULL;
        free(newAmigoList);
    }
    else
    {
        ex_amigo->amigos->friends = newAmigoList;
    }
}

//Finds the degrees of separation between two users
size_t separation (const User* user1, const User* user2)
{
	return 1;
}

//prints the data for the amigos
void dump_data()
{
    userNode* current = rootAmigo;
    printf("\n");
    while(current != NULL)
    {
        printf("User %s; friends: ", current->user->name);
        for (int i = 0; i < current->user->amigos->numFriends; ++i)
        {
            printf("%s ", current->user->amigos->friends[i]->name);
        }
        printf("\n");
        current = current->next;
    }
    printf("\n");
}
