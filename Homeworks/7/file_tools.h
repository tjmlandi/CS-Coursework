// 
// $Id: file_tools.h,v 2.1 2014/11/13 02:45:57 csci243 Exp $ 
// File: file_tools.h 
// file_tools.h declares the interface to file tools used by the vault.
// 
// @author bks: ben k steele
//
// // // // // // // // // // // // // // // // // // // // // // // // 


#ifndef FILE_TOOLS_H
#define FILE_TOOLS_H

// Make sure we have all the data type declarations we need
#include <sys/types.h>
#include <unistd.h>

/// MoveType specifies the spot from which moves are calculated.
typedef enum {
    START,     /// start of the file
    HERE,      /// relative to the offset of the file pointer 
    END        /// end of the file
} MoveType;

/// move to the offset relative to the specified spot within the file.
/// precondition: file is open.
/// postcondition: file offset is valid. function aborts on invalid move.
/// @param fd the open file descriptor
/// @param offset the number of bytes to move relative to the spot
/// @param spot the spot is a value of MoveType
/// @return the resulting file pointer offset in bytes from start of file
/// 
off_t move_to( int fd, off_t offset, MoveType spot );

/// read_entry fetches count bytes to fill entry with content from the file.
/// @param fd the open file descriptor
/// @param entry the buffer to fill must be > count in size
/// @param count number of bytes to read
/// @return 0 on success or negative value on failure
/// 
int read_entry( int fd, void * entry, size_t count );

/// write_entry writes count bytes from entry to the file at current offset.
/// @param fd the open file descriptor
/// @param entry the buffer to empty must be >= count in size
/// @param count number of bytes 
/// @return 0 on success or negative value on failure
/// 
int write_entry( int fd, void * entry, size_t count );

#endif // FILE_TOOLS_H

// // // // // // // // // // // // // // // // // // // // // // // // 
// Revisions: 
// $Log: file_tools.h,v $
// Revision 2.1  2014/11/13 02:45:57  csci243
// (wrc) updated to use off_t for file offsets
//
// Revision 2.0  2014/04/13 16:55:26  csci243
// documentation wording clarifications.
//
