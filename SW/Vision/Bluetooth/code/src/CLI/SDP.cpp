/**
 * @file SDP.cpp
 * @author Pedro Duarte
 * @author Alexandre Mano
 * @date 04-06-2019
 *
 * @brief Service Discovery Protocol interface
 *
 * It provides a way to register and search for a service, allowing dynamic 
 * address binding
 */
#include "SDP.h"
#include <stdexcept>

using std::runtime_error;

#ifndef DEBG
#define DEBG
#include <iostream>
using std::cout;
using std::endl;
#endif

SDP::SDP()
{

}

SDP::~SDP()
{
    sdp_close(server_session);
}

uint8_t SDP::RegisterService(const string &service_name, const string &svc_dsc, const string &service_prov)
{
    sdp_list_t *l2cap_list = 0,
               *rfcomm_list = 0,
               *root_list = 0,
               *access_proto_list = 0,
               *svc_class_list = 0,
               *profile_list = 0;
    sdp_data_t *channel = 0;
    int sock, status;

    struct sockaddr_rc to_bind;
    sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    to_bind.rc_family = AF_BLUETOOTH;

    const bdaddr_t tmp = {0, 0, 0, 0, 0, 0};
    to_bind.rc_bdaddr = tmp;

    for(rfcomm_channel = 1; rfcomm_channel <= 30; rfcomm_channel++)
    {
        to_bind.rc_channel = rfcomm_channel;
        status = bind(sock,(struct sockaddr *)&to_bind, sizeof(to_bind));
        if(status == 0) break;
    }

    sdp_uuid128_create(&svc_uuid_server, &svc_uuid_int);
    sdp_set_service_id(&record, svc_uuid_server);

    sdp_uuid16_create(&svc_class_uuid, SERIAL_PORT_SVCLASS_ID);
    svc_class_list = sdp_list_append(0, &svc_class_uuid);
    sdp_set_service_classes(&record, svc_class_list);

    sdp_uuid16_create(&profile.uuid, SERIAL_PORT_PROFILE_ID);    
    profile.version = 0x0100;
    profile_list = sdp_list_append(0, &profile);
    sdp_set_profile_descs(&record, profile_list);

    sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
    root_list = sdp_list_append(0, &root_uuid);
    sdp_set_browse_groups(&record, root_list);

    sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
    l2cap_list = sdp_list_append(0, &l2cap_uuid);
    proto_list = sdp_list_append(0, l2cap_list);

    sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
    channel = sdp_data_alloc(SDP_UINT8, &rfcomm_channel);
    rfcomm_list = sdp_list_append(0, &rfcomm_uuid);
    sdp_list_append(rfcomm_list, channel);
    sdp_list_append(proto_list, rfcomm_list);
    access_proto_list = sdp_list_append(0, proto_list);    
    sdp_set_access_protos(&record, access_proto_list);

    sdp_set_info_attr(&record, service_name.c_str(), service_prov.c_str(), svc_dsc.c_str());

    const bdaddr_t tmp2 = {0, 0, 0, 0xff, 0xff, 0xff};
    server_session = sdp_connect(&tmp, &tmp2, SDP_RETRY_IF_BUSY);
    if(server_session == (sdp_session_t*)NULL) 
    {
        string con_err = "[SDP Server]: Failed to connect to the local machine - ";
        con_err += string(strerror(errno));
        throw runtime_error(con_err);
        
    }
    sdp_record_register(server_session, &record, 0); 
    
    sdp_data_free (channel);
    sdp_list_free (l2cap_list, 0);
    sdp_list_free (rfcomm_list, 0);
    sdp_list_free (root_list, 0);
    sdp_list_free (access_proto_list, 0);
    sdp_list_free (svc_class_list, 0);
    sdp_list_free (profile_list, 0);

    return rfcomm_channel;
}

void SDP::SearchService(const string &name, std::vector<sdp_record> &recs)
{
    recs.clear();
    sdp_record temp_rec;

    int status;
    bdaddr_t target;
    uuid_t svc_uuid_client;
    sdp_list_t *response_list, *search_list, *attrid_list;
    sdp_session_t *client_session = 0;
    uint32_t range = 0x0000ffff;
    uint8_t port = 0;
    inquiry_info *devices = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    char addr[19] = {0};
    char dev_name[248] = {0};
    char serv_name_buff[248] = {0};

    // search for a communication target    
    dev_id = hci_get_route(NULL);
    sock = hci_open_dev(dev_id);
    if (dev_id < 0 || sock < 0)
    {
        string con_err = "[SDP Client]: Failed to access the local bluetooth adapter - ";
        con_err += string(strerror(errno));
        throw runtime_error(con_err);
    }

    len = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;

    devices = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
    
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &devices, flags);
    
    if(num_rsp <= 0)
    {
        string con_err = "[SDP Client]: Failed to find devices - ";
        con_err += string(strerror(errno));
        throw runtime_error(con_err);
    }
    
    for (i = 0; i < num_rsp; i++)
    {
        ba2str(&(devices+i)->bdaddr, addr);
        memset(dev_name, 0, sizeof(dev_name));
        if (hci_read_remote_name(sock, &(devices+i)->bdaddr,
                                 sizeof(dev_name), dev_name, 0) != 0)
            strcpy(dev_name, "[unknown]");
        
        // connect to the SDP server running on the remote machine
        str2ba(addr, &target);
        const bdaddr_t tmp = {0, 0, 0, 0, 0, 0};
        client_session = sdp_connect(&tmp, &target, SDP_RETRY_IF_BUSY);
        if (client_session == (sdp_session_t *)NULL)
        {
            string con_err = "[SDP Client]: Failed to find device - ";
            con_err += string(strerror(errno));
            throw runtime_error(con_err);
        }

        sdp_uuid128_create(&svc_uuid_client, &svc_uuid_int);
        search_list = sdp_list_append(0, &svc_uuid_client);
        attrid_list = sdp_list_append(0, &range);

        response_list = NULL;
        
        status = sdp_service_search_attr_req(client_session, search_list,
                                             SDP_ATTR_REQ_RANGE,
                                             attrid_list, &response_list);

        if(status == 0)
        {
            sdp_list_t *proto_list = NULL;
            sdp_list_t *r = response_list;

            // go through each of the service records
            for (; r; r = r->next)
            {
                sdp_record_t *rec = (sdp_record_t*) r->data;

                // Get a list of the protocol sequences
                if(sdp_get_access_protos(rec, &proto_list) == 0 && sdp_get_service_name(rec, serv_name_buff, 248) == 0)
                {
                    // get the RFCOMM port number
                    port = sdp_get_proto_port(proto_list, RFCOMM_UUID);
                    sdp_list_free(proto_list, 0);
                }                
                sdp_record_free(rec);

                #ifdef DEBG
                cout << serv_name_buff << " : " << name << ";" << endl;
                #endif
                
                if( string(serv_name_buff) == name ) {
                    temp_rec.addr = addr;
                    temp_rec.dev_name = dev_name;
                    temp_rec.port = port;
                    recs.push_back(temp_rec);
                    break;
                }
            }
        }
        else
        {
            string com_err = "[SDP Client]: Failed to communicate with the SDP server - ";
            com_err += string(strerror(errno));
            throw runtime_error(com_err);
        }
        sdp_list_free(response_list, 0);
        sdp_list_free(search_list, 0);
        sdp_list_free(attrid_list, 0);
        sdp_close(client_session);
    }
    free(devices);
    close(sock);
}
