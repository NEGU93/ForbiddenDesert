/**
 * libpolonet
 * Lightweight TCP/IP network module for educational environments
 * (C) 2011 by the libpolo team.
 *     Marc S. Ressl (mressl@itba.edu.ar)
 *     Jorge Prendes (jprendes@itba.edu.ar)
 *     Agustin Perez Moreno
 * Released under the GPL
 */

/*
 * Polonet is capable of making and receiving TCP/IP connections.
 *
 * Usage as a client:
 * - Call openConnection() to connect to a host. This returns a
 *   PolonetConn identifier. If the connection could not be established,
 *   the identifier is 0.
 *
 * Usage as a server:
 * - Call startListening() to start listening to connections on a port.
 *   It returns 1 on success, 0 on error.
 * - getAvailableConnection() gets the next available incoming connection.
 *   If there is no incoming connection, 0 is returned.
 *
 * Managing a connection:
 * - Before you can use a connection, it is pending. Use isPending() to
 *   determine if a connection is being established.
 * - isConnected() determines if a connection is established.
 * - Call sendData() to send bytes to a connection. It returns
 *   the number of bytes sent.
 * - Call receiveData() to receive bytes from a connection. It returns
 *   the number of bytes received.
 * - Call closeConnection() to close a connection. You should always
 *   close the connections you receive from openConnection() or
 *   getAvailableConnection().
 * - Note: all calls are non-blocking.
 * - Note: when disconnected, isConnected() returns true until all data has
 *   been read with receiveData().
 */

#ifndef _LIBPOLONET_H
#define _LIBPOLONET_H

#ifdef __cplusplus
extern "C" {
#endif

#define PolonetConn int

/* Client functions */
PolonetConn openConnection(const char *hostname, unsigned short port);

/* Server functions */
int startListening(unsigned short port);
void stopListening();
PolonetConn getAvailableConnection();

/* Connection functions */
int isPending(PolonetConn conn);
int isConnected(PolonetConn conn);
int sendData(PolonetConn conn, char *buffer, const int bufferSize);
int receiveData(PolonetConn conn, char *buffer, const int bufferSize);
void closeConnection(PolonetConn conn);

#ifdef __cplusplus
}
#endif

#endif
