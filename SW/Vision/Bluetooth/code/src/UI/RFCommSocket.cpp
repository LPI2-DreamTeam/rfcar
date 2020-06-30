/**
 * @file RFCommSocket.cpp
 * @author Jose Pires
 * @date 03-06-2019
 *
 * @brief Implementation for a RFCOMM socket
 *
 * It creates a specific RFCOMM socket and retrieves a handle to it.
 * - It must be connected to be useful; thus the need for specialization
 */
#include "RFCommSocket.h"
#include <sstream>
#include <stdexcept>

using std::runtime_error;

#define PORT_NR_MIN 1
#define PORT_NR_MAX 30

RFCommSocket::RFCommSocket( ) : 
    Socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM ) { 
/* After initializing the base class, one can initialize the
 * RFComm address to default values 
 */
    this->loc_addr.rc_family = RFCommSocket::bd_family;
    this->loc_addr.rc_bdaddr = RFCommSocket::bd_any;
    this->loc_addr.rc_channel = 1;

    this->rem_addr.rc_family = RFCommSocket::bd_family;
    this->rem_addr.rc_bdaddr = RFCommSocket::bd_any;
    this->rem_addr.rc_channel = 1;
}

RFCommSocket::~RFCommSocket(){}

bool RFCommSocket::isPortValid(int port) const
{
    return ( !(port < PORT_NR_MIN || port > PORT_NR_MAX) );
}

string RFCommSocket::localAddrInfo() const
{
    if( !this->connected )
        throw runtime_error("[RFCommSocket]: No address info can be retrieved, because the socket is not connected");
    return addrInfo( &loc_addr );
}

string RFCommSocket::remoteAddrInfo() const
{
    if( !this->connected )
        throw runtime_error("[RFCommSocket]: No address info can be retrieved, because the socket is not connected");
    return addrInfo( &rem_addr );
}

string RFCommSocket::addrInfo(const sockaddr_rc *addr) const
{
    /* Get address in string form */
    char addr_buf[1024] = { 0 };
    ba2str( &(addr->rc_bdaddr), addr_buf);
/* Create string */
    //std::stringstream ss;
    //ss << "Addr: " << string(addr_buf) << ", Port: " << (int)addr->rc_channel;
    //string s = "Addr: ";
    //s += string(addr); /* Add address */
    //s += ", Port: ";
    //s += string( (char)this->loc_addr.rc_channel ); /* Add port */
    return string(addr_buf);
}
