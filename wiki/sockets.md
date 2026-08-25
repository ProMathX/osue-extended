## Introduction

The socket mechanism is used for _interprocess communication_ not only across processes on the same, but also on different hosts (like PCs, a router or an embedded device) on a network. Sockets follow the _client-server_-model: A connection from a client can be established over a _Client Socket_ to a _Server Socket_. Unlike Unnamed Pipes a communication channel established between two sockets supports _full-duplex_ data transfer: both the client and the server can send and receive simultaneously over the same channel. Sockets are endpoints that have a _domain_, a _type_ and a _protocol_.

The two most often used socket domains are: AF_UNIX for processes that share a common filesystem and AF_INET for processes on different hosts on a network (e.g. the Internet). Both have different addressing modes, but for the course only AF_INET (IPv4) is of interest (AF_INET6 is for IPv6). An AF_INET address consists of:

* the address of the host, an IPv4 address that uniquely identifies the host on the network - also known as the "IP address".
* the port: a 16-bit wide integer that uniquely identifies the service on the host. Port numbers below 1024 are privileged and cannot be used by normal users on a Unix host.

The _type_ of a socket actually defines the properties of the communication channel. Most widely used are SOCK_STREAM (stream sockets, connection-aware) and SOCK_DGRAM (unix datagram sockets, connection-less) sockets.

* Stream sockets resemble pipes: data is reliably transmitted and received in order by use of the Transmission Control Protocol (TCP).
* Unix datagram sockets do not establish _controlled_ connections: messages are sent individually without acknowledgement of the receiver. There are no mechanisms to ensure reliable reception or even in-order reception.

In the course we only consider stream sockets.

Lastly, the _protocol_ of a socket is restricted by the choice of domain and type. As we only want to use AF_INET SOCK_STREAM sockets, the only valid type is IPPROTO_TCP .

From the application programmer's point of view, a socket, once connected, can be used just like any file descriptor: system calls for I/O that work with file descriptors will work with sockets too (e.g. read(2), write(2), fdopen(3), close(2) ...).

## Server Sockets

1. Create a socket (_socket(2)_)
2. Bind the socket to an address (_bind(2))._
3. Listen for connections (_listen(2)_).
4. Accept connections (_accept(2)_). This call blocks by default until a client connects to the server. Be aware that each accepted connection creates a new socket and other clients can be served on the same server socket again (see example code).
5. From now on you can use the file descriptor returned by _accept(2)_ for sending and receiving data (e.g. _read(2)_, _write(2)_, _fdopen(3)_, ...)

### Example Code

Copy/paste may not be wise here - read what's going on, then try for yourself on your own. During the exam you do not have this tutorial available!

```
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
 
#define LISTEN_BACKLOG		5
#define PORT		        4000
.
.
.
 
/* Please go through the code, read the text before and also read the man-pages!
The macro _XOPEN_SOURCE must be set to 500 for some of the used structures: e.g. with the help of a CFLAG: -D_XOPEN_SOURCE=500
*/ 
static int create_server_socket(struct sockaddr_in* saddr, int* saddr_size, int port)
{
	int sockfd;
 
	/* 1.) create socket */
	if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		(void) fprintf(stderr, "Socket creation failed\n");
		return -1;
	}
 
	saddr->sin_addr.s_addr=INADDR_ANY;
	saddr->sin_port=htons(port);
	saddr->sin_family=AF_INET;
 
	/* 2.) bind to a port */
	if(bind(sockfd, (struct sockaddr*)saddr, *saddr_size) < 0)
	{
		(void) fprintf(stderr, "Could not bind to port!\n");
		(void) close(sockfd);
		return -1;
	}
 
	/* 3.) listen for incoming connections, set socket to passive */
	if(listen(sockfd, LISTEN_BACKLOG) < 0)
	{
		(void) fprintf(stderr, "Could not set socket to passive\n");
		(void) close(sockfd);
		return -1;
	}
 
 
	return sockfd;
}
 
.
.
.
 
/* someplace where you want to accept connections; Warning: error conditions are not checked */
 
	struct sockaddr_in saddr;
	int saddr_size = sizeof(saddr);
	int fd = create_server_socket(&saddr, &saddr_size, PORT);
	/* the following call blocks until a client connects */ 
	int cfd = accept(fd, (struct sockaddr*)&saddr,(socklen_t *)&saddr_size);
	/* cfd is the file descriptor of the client that just connected.
            an accept(2) on fd will block until the next client connects */
```

## Client Sockets

The steps necessary to establish a connection from a _client socket_ to an open _server socket_ are straight forward:

1.  Resolve the host address (_getaddrinfo(3)_) - do this first, because if the host is not resolvable to a valid address, you did everything before in vain.
2.  Create a socket (_socket(2)_)
3.  Connect the socket to the address of the host (_connect(2)_)
4.  From now on you can use the file descriptor returned by _socket(2)_ for sending and receiving data (e.g. _read(2)_, _write(2)_, _fdopen(3)_, ...)

### Example Code

Copy/paste may not be wise here - read what's going on, then try for yourself on your own. During the exam you do not have this tutorial available!

```
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
 
 
.
.
.
 
/* Please go through the code, read the text before and also read the man-pages 
The macro _XOPEN_SOURCE must be set to 500 for some of the used structures: e.g. with the help of a CFLAG: -D_XOPEN_SOURCE=500
*/ 
static int open_client_socket(char* const server_hostname, const char* server_service)
{
	int sockfd = -1;
	struct addrinfo* ai,* ai_sel = NULL,* ai_head;
	struct addrinfo hints;
	int err;
 
	/* 1.) resolve hostname to IPv4 address */
 
	hints.ai_flags = 0;
	hints.ai_family = AF_INET; /* IPv4 only, IPv6: AF_INET6  */
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_addrlen = 0;
	hints.ai_addr = NULL;
	hints.ai_canonname = NULL;
	hints.ai_next = NULL;
	/* see getaddrinfo(3), returns addrinfo structures in a linked list of a given
           internet host and service; we provide that information by 'hints' */
	if((err = getaddrinfo(server_hostname, server_service, &hints, &ai)) != 0)
	{
		(void) fprintf(stderr, "ERROR:  %s\n", gai_strerror(err));
		return -1;
	}
	ai_head = ai;
	if(ai == NULL)
	{
		(void) fprintf(stderr, "Could not resolve host %s.\n", server_hostname);
		return -1;
	}
 
	/* usually we would go through the list of address informations and try each
           address until we succeed, but for now, we just try the first one. You could 
           implement your own client socket function with that feature!
	 */
	ai_sel = ai;
 
	/* 2.) create socket */
	if((sockfd = socket(ai_sel->ai_family, ai_sel->ai_socktype, ai->ai_protocol)) < 0)
	{
		(void) fprintf(stderr, "Socket creation failed\n");
		return -1;
	}
 
	/* 3.) connect */
	if(connect(sockfd, ai_sel->ai_addr, ai_sel->ai_addrlen) < 0)
	{
		(void) close(sockfd);
		freeaddrinfo(ai);
		(void) fprintf(stderr, "Connection failed.\n");
		return -1;
	}
 
	freeaddrinfo(ai); /* the dynamically created linked list of getaddrinfo must
                              be freed */
 
	return sockfd;
}
```

## Links

Other Tutorials:

* [http://www.linuxhowtos.org/C_C++/socket.htm](http://www.linuxhowtos.org/C_C++/socket.htm)
* [http://beej.us/guide/bgnet/output/html/multipage/theory.html](http://beej.us/guide/bgnet/output/html/multipage/theory.html)

Disclaimer: The information here may contain errors. Please correct them and help to improve this tutorial.

Note: This tutorial is based on multiple sources and your input. It is going to be refined and extended constantly. All of the sources we used are listed below.