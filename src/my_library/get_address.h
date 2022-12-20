#ifndef GET_ADDRESS_H
#define GET_ADDRESS_H 1

#include <netinet/in.h>
#include <stdio.h>

extern const unsigned MY_ADDRSTRLEN;

// Get address from sockaddr_in by string.
void GetAddressFromSockaddr_in(const struct sockaddr_in *src, char *dst, size_t size);

#endif  // GET_ADDRESS_H
