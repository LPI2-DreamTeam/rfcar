/**
 * @file Socket.h
 * @author Jose Pires
 * @date 03-06-2019
 *
 * @brief Interface for a network programming socket
 *
 * This is an abstract class; must be inherited and specified by its heirs.
 * Defines the interface to the network programming world, using the socket's
 * model.
 */

#ifndef _SOCKET_H
#define _SOCKET_H
#include <iostream>

using std::string;

class Socket
{
protected: /* So variables can be accessed by derived classes */
    int handle; /**< Handle to socket */
    int rhandle; /**< Handle to a remote socket */
    size_t rcv_len; /**< lenght of receiving buffer */
    int conn_timeout; /**< Connection timeout: -1: blocking; 0: expires immediately; >0: expires conn_timeout ms later */
    bool connected;
public:
/**
 * @brief Creates and endopoint for communication (socket)
 * @param domain: communication domain - protocol family
 * @param type: communication type - communication semantics
 * @param protocol: communication protocol
 *
 * Exceptions thrown: caller must catch
 * - 1: Failed to create socket
 *
 * For further details, see http://man7.org/linux/man-pages/man2/socket.2.html
 */
    Socket(int domain, int type, int protocol);

/**
 * @brief Virtual socket destructor
 *
 * Used to create abstract class; it must be declared like this, but must also
 * be implemented, so subclasses can destroy the base class
 */
    virtual ~Socket() = 0;

/**
 * @brief Send a string via a connected socket
 * @param s: string to send
 *
 * If a socket is connected, Send() tries to send the msg through the socket.
 *
 * Exceptions: caller must catch
 * - 1: Send via a non-connected socket
 * - 2: Send() fails the actual transmission through the socket
 */
    void Send(const string &s);

/**
 * @brief Receives a msg via a connected socket
 * @return string received
 *
 * If a socket is connected, Recv() tries to receive the msg through the socket.
 *
 * Exceptions: caller must catch
 * - 1: Receive via a non-connected socket
 * - 2: Recv() fails the actual reception through the socket
 */
    string Recv( );

/**
 * @brief Sets the timeout of the socket for non-blocking calls
 * @param seconds: nr of seconds after which a timeout occurs:
 * - -1: blocking - waits forever
 * - 0: non-blocking - returns immediately
 * - >0: timeout in seconds
 */
    void setTimeout(int seconds);

/**
 * @brief Checks if a socket is valid
 * @return true if false; otherwise false
 */
    bool isValid() const;

/**
 * @brief Close the socket
 */
    void Close();

/**
 * @brief set the receiving buffer size
 * @param len: size of receiving buffer
 *
 * Exceptions: caller must catch
 * - 1: size bigger than 4098 bytes
 */
    void setRecvSize(size_t len);

/**
 * @brief Gets the current size of receiving buffer
 * @return current size of receiving buffer
 */
    size_t RecvSize() const; 

/**
 * @brief Checks is a port is valid
 * @param port: port to check
 * @return true is port valid; otherwise false
 *
 * Subclasses must override this: depends on the type of socket
 */
    virtual bool isPortValid(int port) const = 0;
};

#endif // _SOCKET_H
