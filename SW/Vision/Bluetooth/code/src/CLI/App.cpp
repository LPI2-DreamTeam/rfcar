/**
 * @file App.cpp
 * @author Jose Pires
 * @date 18-06-2019
 *
 * @brief Implementation for BT Chat app (CLI interface)
 */
#include "App.h"
#include <stdexcept>

using std::cin;
using std::cout;
using std::endl;
using std::runtime_error;

#ifndef DEBG
#define DEBG
#endif

/* Service configs */
//#define SERVICE_NAME "BTCHAT\0"
//#define SERVICE_DSC "Bluetooth Chat\0"
//#define SERVICE_PROV "MIEEICOM\0"

static const string service_name = "BTCHAT";
static const string service_dsc = "Bluetooth Chat";
static const string service_prov = "MIEEICOM";


App::App()
{
    /* Create server, binds to a port, and starts listening */
    serv = new RFCommSocketServer();
        
    /* Print out address info */
#ifdef DEBG
    cout << "[Binded]  " << serv->localAddrInfo() << endl;
    cout << "Listening..." << endl;
#endif

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

   /* Start SDP */
   sdp_serv = new SDP();

   /* Register service */
   sdp_serv->RegisterService(service_name, service_dsc, service_prov);

    /* Create a new client socket */
   cli = new RFCommSocketClient();

   /* Initialize comm socket as client */
   this->sock = this->cli;

   /* Create threads */
   this->accept_th = new thread(&App::accept_thread_fcn, this);
   this->send_th = new thread(&App::send_thread_fcn, this);
   this->recv_th = new thread(&App::recv_thread_fcn, this);

   /* Run threads in background */
   accept_th->detach();
   send_th->detach();
   recv_th->detach();

/* Print help */
    help_CB();
}

void App::help_CB()
{
    cout << "       BTChat App           " << endl << endl
         << "f <service_name>: list all devices with specified service name" << endl
         << "c <port>: connects to a port" << endl
         << "s <msg>: sends a message    " << endl
         << "q: quit the program" << endl << endl;
}

void App::connRequested_cb()
{
    try{
        serv->Accept();
        cout << "Accepted connection from " << serv->remoteAddrInfo() << endl;
    }catch(runtime_error &e)
    {
        cout << e.what() << endl;
    }
}

void App::connect_CB(const string &addr, int port)
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

        connected = true;
    }catch(runtime_error &e)
    {
        cout << e.what() << endl;
        return;
    }
}

void App::findService_CB(const string &name)
{
    std::vector<sdp_record> recs;

    /* Get all devices that advertise the requested service */
    try{
        cout << "Name:" << name << ";" << endl;
        sdp_serv->SearchService(name, recs);
    }catch(runtime_error &e)
    {
        cout << e.what() << endl;
        return;
    }

    if( !recs.size() )
    {
        cout << "No services founds" << endl;
        return;
    }

    /* Print out all records details */
    for(size_t i = 0; i < recs.size(); i++)
    {
        cout << "Device: " << recs[i].dev_name << endl;
        cout << "Addr: " << recs[i].addr << endl;
        cout << "Port: " << recs[i].port << endl;
    }
}

void App::send_CB(const string &s)
{
    if(!connected)
        return;
    std::lock_guard<mutex> u_lk(omut); // protect shared resource: output buffer
    send_buff.push( s ); /* Store string in output buffer */
    send_data.notify_one(); /* Notify Send thread that it can send data */
}

/* ------------- Threads --------------- */
void App::accept_thread_fcn()
{
    int status;
    /* Connected is a shared variable; it can be set by connect call */
    while( !connected ){
        try{
            status = serv->Accept();
        }
        catch(runtime_error &e){
            //DisplayWarnMsgbox("[Accept thread fcn]: ", e.what());
            cout << "[Accept thread fcn]: error " << e.what() << endl;
        }

        /* Server accepted a connection; signal connection and update socket */
        if(status)
        {
            unique_lock<mutex> u_lk(accept_mut); // protect shared resource
            connected = true;
            //ui->connectButton->setEnabled(false);
            //initiateDatabase("00:00:00:00:00:00"); //to do: obter o endereço bluetooth do target
            sock = serv;
            u_lk.unlock();
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void App::send_thread_fcn()
{
    string s;
    while(1) // thread runs forever
    {
        unique_lock<mutex> u_lk(omut); // protect shared resource: output buffer
        send_data.wait(u_lk,
                       [this]{ return !send_buff.empty(); } ); // wait for send condition to be activated, with some data to process
        s = send_buff.front( ); /* Retrieve string from output buffer */
        send_buff.pop(); /* Remove element from queue */
        u_lk.unlock(); /* Unlock buffer */

        /* Create a thread to send through socket */
        thread t(&App::send_socket_thread_fcn, this, s);

        /* Run thread in the background to process more data */
        t.detach();
    }
}

void App::send_socket_thread_fcn(string s)
{
    size_t sz = s.size();
    /* while all data has not been sent */
    while( sz )
    {
        /* Get msg size */
        sz = s.size();
        /* Check if socket is writable */
        if(this->sock->isWritable() )
        {
            try{
                /* Store msg to send */
                const string msg = s;
#ifdef DEBG
            cout << "Sending: " << msg << endl;
#endif
                /* Send the data and get the nr. of bytes sent */
                int status = sock->Send(msg);
                if(status == -1) 
                    continue; /* Could not send data; try again */
                /* Update nr. of bytes to send */
                sz -= status;
                /* Update msg string */
                s.erase(s.begin(), s.begin() + status);
            }catch(runtime_error &e){
                cout << e.what() << endl;
            }
        }
    }
    
}

void App::recv_thread_fcn()
{
    int sz;
    string s;
    string msg = "";
    /* Runs forever */
    while( 1 )
    {
        if(connected)
        {
            unique_lock<mutex> u_lk(imut); // protect shared resource: output buffer
            recv_data.wait(u_lk,
                           [this]{ return !this->sock->isReadable(); } ); // wait for condition to read socket to be activated, with some data to process
            u_lk.unlock();

            while(1)
            {
                try {
                    sz = this->sock->Recv( s );
                    if(sz == -1)
                        break; // no more data to read; EAGAIN or EWOULDBLOCK was set
                    /* Append data to msg */
                    msg += s;
                } catch (runtime_error &e) {
                    cout << e.what() << endl;
                }
                
            }

            /* Push data to input buffer for thread Update UI to consume */
            u_lk.lock();
            recv_buff.push(msg);
            u_lk.unlock();

            /* Create a thread to update UI with received data */
            thread t(&App::recv_thread_updateUI_fcn, this);

            /* Run thread in the background to process more data */
            t.detach();

            /* Clear msg */
            string msg = "";
        }
    }
}

void App::recv_thread_updateUI_fcn()
{
    unique_lock<mutex> u_lk(imut); // protect shared resource: input buffer
    string s = recv_buff.front( ); /* Retrieve string from input buffer */
    recv_buff.pop(); /* Remove element from queue */
    /* Update UI */
    //ui->chatBox->formatText(s);
    cout << "[Recv]: " << s << endl;
    u_lk.unlock(); /* Unlock buffer */
}

/**
 * @brief Executes the App event loop
 */
void App::exec()
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
            cin >> line;
            cin >> port;
            connect_CB(line, port);
            break;
        case 'f':
        case 'F':
            cin >> line;
            findService_CB(line);
            break;
        case 'q':
        case 'Q':
            return;
        default:
            /* Purge input */
            getline(cin, line);
            help_CB();
        }
    }
}
