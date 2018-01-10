// vault-stub.c starts the implementation of vault.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "vault.h"
#include "file_tools.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

//Static variables to store file size, current undex, and file ID
static Vault VID = -1;
static int curIndex;
static struct stat *buf;

//ftruncate prototyep
int ftruncate(int fildes, off_t length);

/// open the vault file.
int v_open( const char * filename ) {
	//Check to see if file already open
	if (VID == -1)
	{
		VID = open(filename, O_RDWR);
		//Make sure file load was valid
		if (VID == -1)
		{
			perror("badFileName: No such file or directory\n");
			return -1;
		}
		//Fill file data struct
		buf = malloc(sizeof(struct stat));
		fstat(VID, buf);
		curIndex = lseek(VID, 0, SEEK_SET);
	}
	else 
	{
		return -1;
	}
	return VID;
}

/// close the vault file.
int v_close( Vault vid ) {
	//Makes sure file is loaded
	if (VID == -1)
	{
		perror("No File to Close\n");
		return -1;
	}
	//CLoses file and makes sure it closes properly
	else if (close(vid) == -1)
	{
		perror("Close file failed\n");
		return -1;
	}
	else
	{
		return 0;
	}
}

/// create an entry for the vault.
VaultEntry_T * v_create( char * title, char * artist, int value, int cost ) {
	//Make sure memory is allocated properly
	VaultEntry_T * toReturn = malloc(sizeof(VaultEntry_T));
	if (toReturn == NULL) exit(EXIT_FAILURE);
	//Populate title field, making sure to add illipsis if too long
	if (strlen(title) < TITLE_LEN)
	{
		strcpy(toReturn->title, title);
	}
	else 
	{
		for (int i = 0; i < TITLE_LEN - 4; ++i)
		{
			toReturn->title[i] = title[i];
		}
		toReturn->title[TITLE_LEN - 4] = '.';
		toReturn->title[TITLE_LEN - 3] = '.';
		toReturn->title[TITLE_LEN - 2] = '.';
		toReturn->title[TITLE_LEN - 1] = '\0';
	}
	//Populate artist field, making sure to add illipsis too long
	if (strlen(artist) < ARTIST_LEN)
	{
		strcpy(toReturn->artist, artist);
	}
	else 
	{	
		for (int i = 0; i < ARTIST_LEN - 4; ++i)
		{
			toReturn->artist[i] = artist[i];
		}	
		toReturn->artist[ARTIST_LEN - 4] = '.';
		toReturn->artist[ARTIST_LEN - 3] = '.';
		toReturn->artist[ARTIST_LEN - 2] = '.';
		toReturn->artist[ARTIST_LEN - 1] = '\0';
	}
	//Populate vlue and cost fields
	toReturn->value = value;
	toReturn->cost = cost;
	return toReturn;
}

/// add an entry to the vault file.
int v_add( Vault vid, VaultEntry_T * entry ) {
	//Make sure file is loaded and entry is valide
	if (VID == -1) return -1;
	if (entry == NULL) return -1;
	//Write entry data to end of file
	curIndex = lseek(vid, 0, SEEK_END);
	write(vid, entry, 76);
	//Recalculate the file size
	fstat(vid, buf);
	//Return the last index
	curIndex = lseek(vid, 0, SEEK_END);
	return (curIndex / 76) - 1;
}

/// get the entry at the index.
VaultEntry_T * v_get( Vault vid, size_t index ) {
	//Make sure file is loaded and index is valid
	if (VID == -1) exit(EXIT_FAILURE);
	if ((unsigned)index * 76 > buf->st_size) return NULL;
	//Allocate new vault entry stuct, fill it with data at specified index
	curIndex = lseek(vid, index * 76, SEEK_SET);
	VaultEntry_T* toReturn = malloc(sizeof(VaultEntry_T));
	read(vid, toReturn, 76);
	//Return next index 
	curIndex = lseek(vid, 0, SEEK_CUR);
	return toReturn;
}

/// update the entry at the index with new information.
int v_update( Vault vid, size_t index, VaultEntry_T * entry ) {
	//Make sure file is loaded and index is valid and entry is valid
	if (VID == -1) return -1;
	if ((unsigned)index * 76 > buf->st_size) return -1;
	if (entry == NULL) return -1;
	//Overwrite given index with new data
	curIndex = lseek(vid, index * 76, SEEK_SET);
	write(vid, entry, 76);
	//Return next index
	curIndex = lseek(vid, 0, SEEK_CUR);
	return (curIndex / 76);
}

/// delete the entry at the index, replacing it with the last entry's content.
int v_delete( Vault vid, size_t index ) {
	//Check if file is loaded and index is valid
	if (VID == -1) return -1;
	if ((unsigned)index * 76 > buf->st_size) return -1;
	//Allocate temporary entry, read from last entry into it
	VaultEntry_T* temp = malloc(sizeof(VaultEntry_T));
	curIndex = lseek(vid, -76, SEEK_END);
	read(vid, temp, 76);	
	//Write from temporary entry into specified entry to be deleted
	curIndex = lseek(vid, index * 76, SEEK_SET);
	write(vid, temp, 76);
	//Truncate the file, removing excess entry
	ftruncate(vid, buf->st_size - 76);
	curIndex = lseek(vid, ((index + 1) * 76), SEEK_SET);
	//Free the temp and re calculate size
	free(temp);
	fstat(vid, buf);
	//Return next index
	return (curIndex / 76);
}

/// return the number of entries in the vault file.
size_t v_size( Vault vid ) {
	curIndex = lseek(vid, 0, SEEK_CUR);	
	return (buf->st_size / 76);
}

/// get the index associated with the current file offset into the vault.
size_t v_index( Vault vid ) {
	curIndex = lseek(vid, 0, SEEK_CUR);
	return (curIndex / 76);
}

/// set the file offset so it refers to the index-th record in the vault file.
size_t v_set_index( Vault vid, size_t index ) {
	curIndex = lseek(vid, index * 76, SEEK_SET);
	return (curIndex / 76);
}

/// print the entry.
void v_print_entry( VaultEntry_T * entry ) {
	printf("\"%s\", by %s\n\tis worth $%i and cost $%i dollars.\n", entry->title, entry->artist, entry->value, entry->cost);
	return ;
}

// end file
