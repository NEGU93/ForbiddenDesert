/**
 * libpolonet
 * Lightweight TCP/IP network module for educational environments
 * (C) 2011 by the libpolo team.
 *     Marc S. Ressl (mressl@itba.edu.ar)
 *     Jorge Prendes (jprendes@itba.edu.ar)
 *     Agustin Perez Moreno
 * Released under the GPL
 */

#ifdef _WIN32
#include <winsock.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h> /* for FIONREAD (sigh) */
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#ifndef FIONREAD
#include <sys/filio.h> /* Solaris 2 puts it here */
#endif

#include <stdio.h>
#include <string.h>
#include "polonet.h"

static int fdserver = -1;

/*
 * Public functions
 */

static int polonetInitWSA()
{
#ifdef WIN32
	static int isWSAInitialized = 0;
	
	if (!isWSAInitialized)
	{
		WSADATA wsaData;
		WORD wsaVersion = MAKEWORD(2, 0);
		
		if (WSAStartup(wsaVersion, &wsaData))
			return 0;
		
		isWSAInitialized = 1;
	}
#endif	
	return 1;
}

static int polonetSetNonBlocking(int fd)
{
#ifdef WIN32
	unsigned long isNonBlocking = 1;
	return ioctlsocket(fd, FIONBIO, &isNonBlocking) != -1;
#else
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		return 0;
	
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK) != -1;
#endif
}

static int polonetSetReuseAddress(int fd)
{
#ifndef WIN32
	int value = 1;
	if (setsockopt(fdserver, SOL_SOCKET, SO_REUSEADDR,
	               &value, sizeof(value)) == -1)
		return 0;
#endif
	return 1;
}

static void polonetCloseSocket(int fd)
{
	if (fd == -1)
		return;
#ifdef WIN32
	closesocket(fd);
#else
	close(fd);
#endif
}

/*
 * Public functions
 */

int startListening(unsigned short port)
{
	struct sockaddr_in sin;
	
	/* Open server socket */
	polonetInitWSA();
	polonetCloseSocket(fdserver);
	
	if ((fdserver = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		return 0;
	
	polonetSetNonBlocking(fdserver);
	polonetSetReuseAddress(fdserver);
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(port);
	
	if (bind(fdserver, (struct sockaddr *) &sin, sizeof(sin)) == -1)
	{
		polonetCloseSocket(fdserver);
		return 0;
	}
	
	if (listen(fdserver, 100) == -1)
	{
		polonetCloseSocket(fdserver);
		return 0;
	}
	
	return 1;
}

void stopListening()
{
	polonetCloseSocket(fdserver);
	fdserver = -1;
}

PolonetConn getAvailableConnection()
{
	fd_set fdsRead;
	struct timeval nowait;
	int fd;	
	
	if (fdserver == -1)
		return 0;
	
	FD_ZERO(&fdsRead);
	FD_SET((unsigned int) fdserver, &fdsRead);
	
	memset((char *)&nowait, 0, sizeof(nowait));
	
	if (select(fdserver + 1, &fdsRead, NULL, NULL, &nowait) == -1)
		return 0;
	
	if (!FD_ISSET(fdserver, &fdsRead))
		return 0;
	
	fd = accept(fdserver, NULL, NULL);
	
	polonetSetNonBlocking(fd);
	
	return fd + 1;
}

PolonetConn openConnection(const char *hostname, unsigned short port)
{
	struct hostent *host;
	struct sockaddr_in sin;
	int fd;
	
	polonetInitWSA();
	
	if ((host = gethostbyname(hostname)) == 0)
		return 0;
	
	if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		return 0;
	
	polonetSetNonBlocking(fd);
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = ((struct in_addr *) (host->h_addr))->s_addr;
	sin.sin_port = htons(port);
	
	connect(fd, (struct sockaddr *) &sin, sizeof(sin));
	
	return fd + 1;
}

int isPending(PolonetConn conn)
{
	fd_set fdsRead, fdsWrite, fdsExcept;
	struct timeval nowait;
	int fd = conn - 1;
	
	if (fd == -1)
		return 0;
	
	FD_ZERO(&fdsRead);
	FD_SET((unsigned int) fd, &fdsRead);
	FD_ZERO(&fdsWrite);
	FD_SET((unsigned int) fd, &fdsWrite);
	FD_ZERO(&fdsExcept);
	FD_SET((unsigned int) fd, &fdsExcept);
	
	memset((char *)&nowait, 0, sizeof(nowait));
	
	if (select(fd + 1, &fdsRead, &fdsWrite, &fdsExcept, &nowait) == -1)
		return 0;
	
	/* Not pending on (fdsRead | fdsWrite) */
	if (FD_ISSET(fd, &fdsRead))
		return 0;
	if (FD_ISSET(fd, &fdsWrite))
		return 0;
	/* Not pending on fdsException */	
	if (FD_ISSET(fd, &fdsExcept))
		return 0;
	
	return 1;
}

int isConnected(PolonetConn conn)
{
	fd_set fdsRead, fdsWrite, fdsExcept;
	struct timeval nowait;
	int fd = conn - 1;
	
	if (fd == -1)
		return 0;
	
	FD_ZERO(&fdsRead);
	FD_SET((unsigned int) fd, &fdsRead);
	FD_ZERO(&fdsWrite);
	FD_SET((unsigned int) fd, &fdsWrite);
	FD_ZERO(&fdsExcept);
	FD_SET((unsigned int) fd, &fdsExcept);
	
	memset((char *)&nowait, 0, sizeof(nowait));
	
	if (select(fd + 1, &fdsRead, &fdsWrite, &fdsExcept, &nowait) == -1)
		return 0;
	
	/* Disconnected if fdsRead but no bytes available for reading */
	if (FD_ISSET(fd, &fdsRead))
	{
		int bytesAvailable;
#ifdef WIN32
		if (ioctlsocket(fd, FIONREAD, &bytesAvailable) == -1)
#else
		if (ioctl(fd, FIONREAD, &bytesAvailable) == -1)
#endif
			return 0;
		if (bytesAvailable <= 0)
			return 0;
	}
	/* Disconnected on !fdsWrite */
	if (!FD_ISSET(fd, &fdsWrite))
		return 0;
	/* Disconnected on fdsException */
	if (FD_ISSET(fd, &fdsExcept))
		return 0;
	
	return 1;
}

int sendData(PolonetConn conn, char *buffer, const int bufferSize)
{
	int fd = conn - 1;
	int status;

	if (fd == -1)
		return 0;
	
	status = send(fd, buffer, bufferSize, 0);
	if (status == -1)
		return 0;
	
	return status;
}

int receiveData(PolonetConn conn, char *buffer, const int bufferSize)
{
	int fd = conn - 1;
	int status;

	if (fd == -1)
		return 0;

	status = recv(fd, buffer, bufferSize, 0);
	if (status == -1)
		return 0;
	
	return status;
}

void closeConnection(PolonetConn conn)
{
	int fd = conn - 1;
	
	polonetCloseSocket(fd);
}

