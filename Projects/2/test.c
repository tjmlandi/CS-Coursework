#include <stddef.h>
#include "amigonet.h"
#include <stdio.h>


int main()
{
	create_amigonet();
	char chad[] = {'c', 'h', 'a', 'd'};
	char brad[] = {'b', 'r', 'a', 'd'};
	addUser(chad);
	addUser(brad);
	//addAmigo(findUser(chad), findUser(brad));
	dump_data();
	destroy_amigonet();
}
