/**
 * @file RFCommSocketServer.cpp
 * @author Jose Pires
 * @date 03-06-2019
 *
 * @brief Implementation for a RFCommSocketServer
 *
 * The server should:
 * 1. create an RFCOMM socket
 * 2. bind to a port and address
 * 3. Listen for incomming connections
 * 4. Accept a connection
 */
#include "RFCommSocketServer.h"
#include <unistd.h>
#include <stdexcept>
#include <string.h> /* strerror */

using std::runtime_error;
//RFCommSocketServer::NrOfInstances() const
//{
//   return nr_instances; 
//}

RFCommSocketServer::RFCommSocketServer(int port) : state(sstate::created), cli_sock(-1){

    try{
        Bind(port); /* Bind to port */
    }
    catch(runtime_error &e){}

    /* Start Listening */
    try{
        Listen();
    }
    catch(runtime_error &e){
        throw runtime_error( e.what() );
    }
}

RFCommSocketServer::~RFCommSocketServer()
{
    /* Own socket is destroyed by base clase; must destroy client socket */
    close( this->cli_sock );
}

void RFCommSocketServer::Bind(int port)
{
/* Update local port */
    this->loc_addr.rc_channel = (uint8_t) port;

/* 
DESCRIPTION         top

       When a socket is created with socket(2), it exists in a name space
       (address family) but has no address assigned to it.  bind() assigns
       the address specified by addr to the socket referred to by the file
       descriptor sockfd.  addrlen specifies the size, in bytes, of the
       address structure pointed to by addr.  Traditionally, this operation
       is called “assigning a name to a socket”.

       It is normally necessary to assign a local address using bind()
       before a SOCK_STREAM socket may receive connections (see accept(2)).

       The rules used in name binding vary between address families.
       Consult the manual entries in Section 7 for detailed information.
       For AF_INET, see ip(7); for AF_INET6, see ipv6(7); for AF_UNIX, see
       packet(7);
       for AF_X25, see x25(7); and for AF_NETLINK, see netlink(7).

       The actual structure passed for the addr argument will depend on the
       address family.  The sockaddr structure is defined as something like:

           struct sockaddr {
               sa_family_t sa_family;
               char        sa_data[14];
           }

       The only purpose of this structure is to cast the structure pointer
       passed in addr in order to avoid compiler warnings.  See EXAMPLE
       below.

RETURN VALUE         top

       On success, zero is returned.  On error, -1 is returned, and errno is
       set appropriately.
 */

    int status = bind(handle, (struct sockaddr *) &loc_addr, 
                      sizeof(loc_addr));
    if(status < 0)
    {
        string s = "[Socket Server]: Failed to bind - ";
        s += string( strerror(errno) );
        throw runtime_error(s);
    }
    this->state = sstate::binded;
}

void RFCommSocketServer::Listen()
{
    if(this->state != sstate::binded )
        throw runtime_error("[Socket server]: Socket must be binded first");

/*     
DESCRIPTION  
       listen() marks the socket referred to by sockfd as a passive 
       that is, as a socket that will be used to accept incoming connection
       requests using accept(2).

       The sockfd argument is a file descriptor that refers to a socket of
       type SOCK_STREAM or SOCK_SEQPACKET.

       The backlog argument defines the maximum length to which the queue of
       pending connections for sockfd may grow.  If a connection request
       arrives when the queue is full, the client may receive an error with
       an indication of ECONNREFUSED or, if the underlying protocol supports
       retransmission, the request may be ignored so that a later reattempt
       at connection succeeds.

RETURN VALUE

       On success, zero is returned.  On error, -1 is returned, and errno is
       set appropriately.
 */
    int backlog = 1;
    int status = listen(this->handle, backlog);
    if(status < 0)
    {
        string s = "[Socket Server]: Failed to listen - ";
        s += string( strerror(errno) );
        throw runtime_error(s);
    }
    this->state = sstate::listening;
}

bool RFCommSocketServer::Accept()
{
/* Check previous state is correct */
    if(this->state != sstate::listening )
        throw runtime_error("[Socket server]: Socket must be listening first");

/* If the server socket is readable, it means that at least one connection in 
 * pending on the connection queue to be accepted*/
    if( !this->isReadable() )
        return false;

/* Initialize temporary client address */
    struct sockaddr_rc cli_addr = { 0 };

/* Accept connection from client: if socket is not marked as *nonblocking*,
 * then accept will block until a connection is present.
 * It returns:
 * - an non negative integer: file descriptor of the accepted socket
 * - -1: error and errno is set appropriately 

DESCRIPTION         top

       The accept() system call is used with connection-based socket types
       (SOCK_STREAM, SOCK_SEQPACKET).  It extracts the first connection
       request on the queue of pending connections for the listening socket,
       sockfd, creates a new connected socket, and returns a new file
       descriptor referring to that socket.  The newly created socket is not
       in the listening state.  The original socket sockfd is unaffected by
       this call.

       The argument sockfd is a socket that has been created with socket(2),
       bound to a local address with bind(2), and is listening for
       connections after a listen(2).

       The argument addr is a pointer to a sockaddr structure.  This
       structure is filled in with the address of the peer socket, as known
       to the communications layer.  The exact format of the address
       returned addr is determined by the socket's address family (see
       socket(2) and the respective protocol man pages).  When addr is NULL,
       nothing is filled in; in this case, addrlen is not used, and should
       also be NULL.

       The addrlen argument is a value-result argument: the caller must
       initialize it to contain the size (in bytes) of the structure pointed
       to by addr; on return it will contain the actual size of the peer
       address.

       The returned address is truncated if the buffer provided is too
       small; in this case, addrlen will return a value greater than was
       supplied to the call.

       If no pending connections are present on the queue, and the socket is
       not marked as nonblocking, accept() blocks the caller until a
       connection is present.  If the socket is marked nonblocking and no
       pending connections are present on the queue, accept() fails with the
       error EAGAIN or EWOULDBLOCK.

       In order to be notified of incoming connections on a socket, you can
       use select(2), poll(2), or epoll(7).  A readable event will be
       delivered when a new connection is attempted and you may then call
       accept() to get a socket for that connection.  Alternatively, you can
       set the socket to deliver SIGIO when activity occurs on a socket; see
       socket(7) for details.

       If flags is 0, then accept4() is the same as accept().  The following
       values can be bitwise ORed in flags to obtain different behavior:

       SOCK_NONBLOCK   Set the O_NONBLOCK file status flag on the open file
                       description (see open(2)) referred to by the new file
                       descriptor.  Using this flag saves extra calls to
                       fcntl(2) to achieve the same result.

       SOCK_CLOEXEC    Set the close-on-exec (FD_CLOEXEC) flag on the new
                       file descriptor.  See the description of the
                       O_CLOEXEC flag in open(2) for reasons why this may be
                       useful.

RETURN VALUE         top

       On success, these system calls return a nonnegative integer that is a
       file descriptor for the accepted socket.  On error, -1 is returned,
       errno is set appropriately, and addrlen is left unchanged.
 */
    socklen_t opt = sizeof(cli_addr);
    int status = accept(this->handle, (struct sockaddr *)&cli_addr, &opt);
/* Failed to accept: throw exception for caller to catch to catch */
    if(status < 0 ) 
    {
        string s = "[Socket Server]: Failed to accept - ";
        s += string( strerror(errno) );
        throw runtime_error(s);
    }
    /* Else, the new client socket should be returned */
    this->rhandle = status;
    /* Make the new socket non-blocking, because it doesn't inherit from
     * accepting socket */
    setTimeout(this->rhandle, 0);
    /* Add to tracking */
    addToTracking(this->rhandle);

/* Update remote address */
    this->rem_addr.rc_bdaddr = cli_addr.rc_bdaddr; 
    this->rem_addr.rc_channel = cli_addr.rc_channel;
    return true;
}

string RFCommSocketServer::localAddrInfo()
{
    if( this->state < sstate::binded )
        throw runtime_error("[RFCommSocketServer]: No address info can be retrieved, because the socket was not binded");
    
    return addrInfo( &loc_addr );
}
