#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H 1

#include <netinet/in.h>
#include <stdio.h>

// Handle error with sys msg.
void DieWithSystemMessage(const char *msg);

// Handle error with sys msg. (Can specify errno.)
void DieWithSystemMessage2(const char *msg, int no);

// Make directory. (Path need '/' at end.)
void MakeDirectory(const char *dirpath);

// Get address from sockaddr_in by string.
void GetAddressFromSockaddr_in(struct sockaddr_in *addr, char *buf, size_t len);

#endif  // MY_LIBRARY_H
