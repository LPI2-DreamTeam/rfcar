/**
 * @file server-main.cpp
 * @author Jose Pires
 * @date 04-06-2019
 *
 * @brief Server app for RFCommServer
 */
#include "AppServer.h"
#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;
using std::runtime_error;

int main(int argc, char **argv)
{
    AppServer *app;

/* Construct the app */
    try{
        app = new AppServer();
    }catch(runtime_error &e)
    {
        cout << e.what() << endl;
    }

/* Execute the app */
    app->exec();
    return 0;
}
