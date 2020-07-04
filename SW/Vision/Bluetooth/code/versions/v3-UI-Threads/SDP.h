/**
 * @file SDP.h
 * @author Pedro Duarte
 * @author Alexandre Mano
 * @date 04-06-2019
 *
 * @brief Service Discovery Protocol interface
 *
 * It provides a way to register and search for a service, allowing dynamic 
 * address binding
 */

#ifndef SDP_H
#define SDP_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>

using std::string;

struct sdp_record
{
    string dev_name;
    string addr;
    int port;
};

class SDP
{
    public:
        /**
         * @brief: default constructor
         * @return void
         * @param none
         */
        SDP();
        /**
         * @brief: Closes the connection to the local SDP server.
         * @return void
         * @param none
         */
        ~SDP();
        uint8_t findAvailablePort();
        /**
         * @brief: Registers a service on the local machine.
         *@return the session
         *@param none
         *Exceptions caller must catch:
         * - 1: sdp(connect() fails to connect to the local machine
         */        
        void RegisterService(const uint8_t rfcomm_channel, const string &service_name, const string &svc_dsc, const string &service_prov);
        /**
         * @brief Returns the port used by the desired service.
         * @return service port
         * 
         * 
         * Exceptions caller must catch:
         * - 1: sdp_connect() fails to connect to the device
         * - 2: sdp_service_search_attr_req() fails to communicate with the SDP server
         */
        void SearchService(const string &name, std::vector<sdp_record> &recs);

    private:
        /**
         * @brief port number 
        */
        uint8_t rfcomm_channel;
        /**
        * @brief local variables that will be used to store the different data elements of the service record
        */
        uuid_t root_uuid, l2cap_uuid, rfcomm_uuid, svc_uuid_server, svc_class_uuid;
        sdp_profile_desc_t profile;
        sdp_record_t record = {0};
        /**
        * @brief list that stores data elements of the service record.
        */
        sdp_list_t *proto_list = 0;
        /**
         * @brief contains the connection to the local machine
         */
        sdp_session_t *server_session = 0;  
        /**
         * @brief stores universally unique identifiers
         */
        uint32_t svc_uuid_int[4] = {0, 0, 0, 0xABCD};
};    

#endif // SDP_H
