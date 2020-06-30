/**
 * @file AppServer.cpp
 * @author Jose Pires
 * @date 04-06-2019
 *
 * @brief Implementation for Server app for Client/server
 */
#include "AppServer.h"
#include <stdexcept>

using std::cin;
using std::cout;
using std::endl;
using std::runtime_error;

AppServer::AppServer()
{
    /* Create server */
    serv = new RFCommSocketServer();
    /* Create database */
    db = new Database();
/* Get hostname */
    hostname = db->load();
/* Load history */
   string hist;
   while( 1 )
   {
       hist = db->history();
       if( hist.empty() )
           break;
       cout << hist << endl;
   }
/* Print help */
    help_CB();
}

void AppServer::help_CB()
{
    cout << "        RFCommServer App      " << endl << endl
         << "s <msg>: sends a message" << endl
         << "c <port>: connects to a port" << endl
         << "q: quit the program" << endl << endl;
}

void AppServer::connection_CB(int port)
{
    //// bind socket to port of the first available 
    //// local bluetooth adapter
    try{
        /* Bind to port */
        serv->Bind( port );
        /* Print out address info */
        cout << "[Binded]  " << serv->localAddrInfo() << endl;
    }catch(runtime_error &e)
    {
        cout << e.what() << endl;
        return;
    }
    
    try{
        serv->Listen();
        cout << "Listening..." << endl;
    }catch(runtime_error &e)
    {
        cout << e.what() << endl;
        return;
    }
    try{
        serv->Accept();
        cout << "Accepted connection from " << serv->remoteAddrInfo() << endl;
    }catch(runtime_error &e)
    {
        cout << e.what() << endl;
    }
}

void AppServer::send_CB(const string &s)
{
    try{
        cout << "Sending: " << endl;
        serv->Send(s);
    }catch(runtime_error &e)
    {
        cout << e.what() << endl;
    }
}
void AppServer::recv_CB()
{
    try{
        cout << "Receiving: " << endl;
        string msg_read = serv->Recv();
        cout << "Msg received: " << msg_read << endl;
/* Append msg to log file */
        string logmsg = hostname + ": " + msg_read;
        db->write(logmsg);
    }catch(runtime_error &e)
    {
        cout << e.what() << endl;
    }
}

/**
 * @brief Executes the AppServer event loop
 */
void AppServer::exec()
{
    char user_cmd;
    string line;
    int port;
    while(1)
    {
        cin >> user_cmd;
        switch(user_cmd)
        {
        case 's':
        case 'S':
            getline(cin, line);
            send_CB(line);
            break;
        case 'c':
        case 'C':
            cin >> port;
            connection_CB(port);
            break;
        case 'q':
        case 'Q':
            return;
        default:
            /* Purge input */
            getline(cin, line);
            help_CB();
        }
        /* Try to read from socket */
        recv_CB();
        
    }
}
