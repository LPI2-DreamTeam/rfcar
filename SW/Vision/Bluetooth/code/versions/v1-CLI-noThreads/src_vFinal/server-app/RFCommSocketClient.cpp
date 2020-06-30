/**
 * @file RFCommSocketClient.cpp
 * @author Jose Pires
 * @date 03-06-2019
 *
 * @brief Implementation for the RFCommSocketClient
 */

#include "RFCommSocketClient.h"
#include <unistd.h>
#include <stdexcept>
#include <string.h> /* strerror */

using std::runtime_error;

void RFCommSocketClient::Connect(string &hostAddr, int port)
{
    if( !this->isPortValid(port) )
        throw runtime_error("[Socket Client]: Invalid port nr. (must be between 1 and 30)");


/* Initialize temporary serv address */
    struct sockaddr_rc serv_addr = { 0 };
    serv_addr.rc_family = AF_BLUETOOTH;


/* These values should be obtained upon a service discovery */
    serv_addr.rc_channel = (uint8_t) port; /* Server Port */
    str2ba( hostAddr.c_str(), &serv_addr.rc_bdaddr );
/* 
DESCRIPTION         top

       The connect() system call connects the socket referred to by the file
       descriptor sockfd to the address specified by addr.  The addrlen
       argument specifies the size of addr.  The format of the address in
       addr is determined by the address space of the socket sockfd; see
       socket(2) for further details.

       If the socket sockfd is of type SOCK_DGRAM, then addr is the address
       to which datagrams are sent by default, and the only address from
       which datagrams are received.  If the socket is of type SOCK_STREAM
       or SOCK_SEQPACKET, this call attempts to make a connection to the
       socket that is bound to the address specified by addr.

       Generally, connection-based protocol sockets may successfully
       connect() only once; connectionless protocol sockets may use
       connect() multiple times to change their association.  Connectionless
       sockets may dissolve the association by connecting to an address with
       the sa_family member of sockaddr set to AF_UNSPEC (supported on Linux
       since kernel 2.2).

RETURN VALUE         top

       If the connection or binding succeeds, zero is returned.  On error,
       -1 is returned, and errno is set appropriately.
 */
    socklen_t opt = sizeof(serv_addr);
    int status = connect( this->handle,  (struct sockaddr *)&serv_addr, opt );
    if(status < 0)
    {
        this->connected = false;
        string s = "[Socket Client]: Failed to connect - ";
        s += string( strerror(errno) );
        throw runtime_error(s);
    }
/* Update connection status */
    this->connected = true;
/* Update remote address */
    this->rem_addr.rc_bdaddr = serv_addr.rc_bdaddr; 
    this->rem_addr.rc_channel = serv_addr.rc_channel; 
}
