/**
 * @file AppClient.h
 * @author Jose Pires
 * @date 04-06-2019
 *
 * @brief Interface for Client app for Client/server
 */

#ifndef APPSERVER_H
#define APPSERVER_H
#include "Database.h"
#include "RFCommSocketClient.h"
#include "SDP.h"

class AppClient{
    Database *db; /**< App's database */
    RFCommSocketClient *cli; /**< Socket Client */
    sdp *sdp_serv; /**< Service Discovery Protocol server */
    string hostname; /**< hostname */
/* Handlers */
    void connection_CB(const string &addr, int port);
    void send_CB(const string &s);
    void recv_CB();
    void help_CB();
public:
/**
 * @brief Constructs the App Client
 */
AppClient();

/**
 * @brief Executes the AppClient event loop
 */
void exec();
};


#endif // APPSERVER_H
