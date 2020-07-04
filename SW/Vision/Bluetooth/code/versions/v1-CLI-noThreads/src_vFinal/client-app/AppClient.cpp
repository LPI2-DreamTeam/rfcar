/**
 * @file AppClient.cpp
 * @author Jose Pires
 * @date 04-06-2019
 *
 * @brief Implementation for Client app for Client/server
 */
#include "AppClient.h"
#include <stdexcept>

using std::cin;
using std::cout;
using std::endl;
using std::runtime_error;

AppClient::AppClient()
{
    /* Create server */
    cli = new RFCommSocketClient();
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

void AppClient::help_CB()
{
    cout << "        RFCommClient App      " << endl << endl
         << "s <msg>: sends a message" << endl
         << "c <bt addr> <port>: connects to a server address and port" << endl
         << "q: quit the program" << endl << endl;
}

void AppClient::connection_CB(const string &addr, int port)
{
    //// bind socket to port of the first available 
    //// local bluetooth adapter
    try{
        /* Bind to port */
        cli->Connect(addr, port );
        /* Print out address info */
        cout << "Connected: " << endl
             << "--- Local addr: " << cli->localAddrInfo() << endl
             << "--- Remote addr:" << cli->remoteAddrInfo() << endl;
    }catch(runtime_error &e)
    {
        cout << e.what() << endl;
        return;
    }
}

void AppClient::send_CB(const string &s)
{
    try{
        cout << "Sending: " << endl;
        cli->Send(s);
    }catch(runtime_error &e)
    {
        cout << e.what() << endl;
    }
}
void AppClient::recv_CB()
{
    try{
        cout << "Receiving: " << endl;
        string msg_read = cli->Recv();
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
 * @brief Executes the AppClient event loop
 */
void AppClient::exec()
{
    char user_cmd;
    string line;
    string addr;
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
            cin >> addr;
            cin >> port;
            connection_CB(addr, port);
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
