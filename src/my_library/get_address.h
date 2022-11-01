#ifndef GET_ADDRESS_H
#define GET_ADDRESS_H 1

#include <netinet/in.h>
#include <stdio.h>

extern const unsigned MY_ADDRSTRLEN;

// Get address from sockaddr_in by string.
void GetAddressFromSockaddr_in(struct sockaddr_in *saddr, char *buf, size_t len);

#endif  // GET_ADDRESS_H
