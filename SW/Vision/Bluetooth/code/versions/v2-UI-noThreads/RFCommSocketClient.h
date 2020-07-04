/**
 * @file RFCommSocketClient.h
 * @author Jose Pires
 * @date 03-06-2019
 *
 * @brief Interface for a RFCOMM socket client
 *
 * The client should create an RFCOMM socket and connect to host, so it can 
 * transfer data (send and receive)
 */
#ifndef RFCOMMSocketClient_H
#define RFCOMMSocketClient_H

#include "RFCommSocket.h"

class RFCommSocketClient : public RFCommSocket
{
public:
    /* Constructor and destructor can be default ones */

/**
 * @brief Initiates a connection on a socket
 * @param hostAddr: host address to connect to
 * @param port: port. nr to connect to (default = 1)
 *
 * This function throws an exception for one of two errors:
 * - 1: Invalid port
 * - 2: Failed to connect on the host socket (address and port)
 */
    void Connect(const string &hostAddr, int port = 1);
};


#endif // RFCOMMSocketClient_H
