/**
 * @file App.h
 * @author Jose Pires
 * @date 18-06-2019
 * @date v2: 24-06-2019
 *
 * @brief Interface for BT Chat app (CLI interface)
 */

#ifndef APP_H
#define APP_H
#include "Database.h"
#include "RFCommSocketServer.h"
#include "RFCommSocketClient.h"
#include "SDP.h"
#include <thread> // thread
#include <mutex> // mutex
#include <queue> // string queue
#include <condition_variable>

using std::thread;
using std::mutex;
using std::queue;
using std::condition_variable;
using std::unique_lock;

class App{
    Database *db; /**< App's database */
    RFCommSocketServer *serv; /**< Socket server */
    RFCommSocketClient *cli; /**< Socket client */
    SDP *sdp_serv; /**< Service Discovery Protocol server */
    string hostname; /**< hostname */
    thread *accept_th; // accept thread: accepts a connection
    thread *send_th; // send thread: writes data to comm output buffer
    thread *recv_th; // recv thread: reads data from comm input buffer
    queue<string> send_buff; // comm output buffer: used to store outgoing data
    queue<string> recv_buff; // comm input buffer: used to store incoming data
    mutex imut; // Mutex for input buffer: protects access to shared buffer
    mutex omut; // Mutex for output buffer: protects access to shared buffer
    mutex accept_mut;
    condition_variable accept_conn;
    condition_variable send_data;
    condition_variable recv_data;
    Socket *sock; // socket ptr to point to correct socket for send/recv
    bool connected; /* Flag to signal a connection has been established */
/* Handlers */
    void connRequested_cb();
    void findService_CB(const string &name);
    void connect_CB(const string &addr, int port);
    void send_CB(const string &s);
    void help_CB();
    /* thread functions */
    void accept_thread_fcn();
    void send_thread_fcn();
    void send_socket_thread_fcn(string s);
    void recv_thread_fcn(); 
    void recv_thread_updateUI_fcn();
public:

/**
 * @brief Constructs the App Server
 */
App();

/**
 * @brief Executes the AppServer event loop
 */
void exec();

};


#endif // APP_H
