/**
 * @file server-main.cpp
 * @author Jose Pires
 * @date 04-06-2019
 *
 * @brief Server app for RFCommServer
 */
#include "RFCommSocketServer.h"
#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;
using std::runtime_error;

int main(int argc, char **argv)
{
    
    //struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    //char buf[1024] = { 0 }; /**< Buffer */
    //int s, client, bytes_read;
    //socklen_t opt = sizeof(rem_addr);

    /* Check command line arguments */
    if( argc < 2 )
    {	
        cout << "usage: %s " << argv[0] << " <bt-port> \n";
        return -1;
    }

    //// allocate socket
    //s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    RFCommSocketServer serv = RFCommSocketServer();

    //// bind socket to port of the first available 
    //// local bluetooth adapter
    //loc_addr.rc_family = AF_BLUETOOTH;
    //loc_addr.rc_bdaddr = *BDADDR_ANY;
    //loc_addr.rc_channel = (uint8_t) atoi(argv[1]);
    //bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    try{
        /* Bind to port */
        serv.Bind( atoi(argv[1]) );
        /* Print out address info */
        cout << "[Binded]  " << serv.localAddrInfo() << endl;
        ///* Print out Local BT info */
        //ba2str(&loc_addr.rc_bdaddr, buf);
        //fprintf(stderr, "Local BT adapter:\nAddress: %s, Port: %u\n", buf,
        //        loc_addr.rc_channel); 
        //memset(buf, 0, sizeof(buf));
    }catch(runtime_error &e)
    {
        cout << e.what() << endl;
        return -3;
    }
    
    try{
        serv.Listen();
        cout << "Listening..." << endl;
        //// put socket into listening mode
        //listen(s, 1);
    }catch(runtime_error &e)
    {
        cout << e.what() << endl;
        return -4;
    }

    try{
        serv.Accept();
        cout << "Accepted connection from " << serv.remoteAddrInfo() << endl;
        //// accept one connection
        //client = accept(s, (struct sockaddr *)&rem_addr, &opt);

        //ba2str( &rem_addr.rc_bdaddr, buf );
        //fprintf(stderr, "accepted connection from %s\n", buf);
        //memset(buf, 0, sizeof(buf));
    }catch(runtime_error &e)
    {
        cout << e.what() << endl;
        return -5;
    }

    string msg_read;
    while(1)
    {
        try{
            cout << "Receiving: " << endl;
            msg_read = serv.Recv();
            cout << "Msg received: " << msg_read << endl;
            //// read data from the client
//  //  bytes_read = read(client, buf, sizeof(buf));
            //bytes_read = recv(client, buf, sizeof(buf), 0);
            //if( bytes_read > 0 ) 
            //    printf("received [%s]\n", buf);
        }catch(runtime_error &e)
        {
            cout << e.what() << endl;
//            return -6;
        }

    }


/* Construtor takes care of closing both sockets */
    //// close connection
    //close(client);
    //close(s);
    return 0;
}
