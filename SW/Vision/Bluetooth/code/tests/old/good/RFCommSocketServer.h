/**
 * @file RFCommSocketServer.h
 * @author Jose Pires
 * @date 03-06-2019
 *
 * @brief Interface for a RFCOMM socket server
 *
 * The server should:
 * 1. create an RFCOMM socket
 * 2. bind to a port and address
 * 3. Listen for incomming connections
 * 4. Accept a connection
 */
#ifndef RFCOMMSocketServer_H
#define RFCOMMSocketServer_H

#include "RFCommSocket.h"

enum sstate{
    created = 0,
    binded,
    listening,
    accepted,
    closed
};

class RFCommSocketServer : public RFCommSocket
{
    enum sstate state;
    int cli_sock;
//    static size_t nr_instances = 0;
public:
    RFCommSocketServer();
    ~RFCommSocketServer();
   
/**
 * @brief Binds the socket server to the local BT adapter and a port (default 1)
 * @param port: port nr. to bind to
 *
 * This function throws an exception for one of two errors:
 * - 1: Invalid port nr. passed (must be between 1 and 30);
 * - 2: Failed to bind the port
 */ 
    void Bind(int port = 1);

/**
 * @brief Listen for connections on a socket
 *
 * This function throws an exception for one of two errors:
 * - 1: Socket was not binded first
 * - 2: Failed to listen on the socket
 */
    void Listen();

/**
 * @brief Accepts connections on a socket
 *
 * This function throws an exception for one of two errors:
 * - 1: Socket must be in listening mode first
 * - 2: Failed to accept on the socket
 */
    void Accept();
//    static size_t NrOfInstances() const;

/**
 * @brief Retrieves local address info in human readable form
 * @return string containing the local address info
 *
 * Overrides RFCommSocket::localAddrInfo()
 */
string localAddrInfo();
};

#endif // RFCOMMSocketServer_H
