Thomas Landi
Mechanics of Programming
Project 3


Design:

I mainly went with a top-down design style.
I started the project by working on handling input properly, and then proceeded to work my way through internal commands first, moving on to external commands secondly.
I had the "environment" running first so as I implemented each feature I could test it individually.


Known Problems:

Currently, I have not working around either fgets or strtok giving data randomly at the end of input, and as such have to manually set a null character at the end of each command entered.
This means that a character is removed each time, so if the bang command is called, it does not currently work properly as it will try to call the command short one character.

The way the commands are parsed, all quoted tokens are removed before the rest is separated by spaces.
This means if verbose mode is active, and quotes were in the command, they will be listed firs.
