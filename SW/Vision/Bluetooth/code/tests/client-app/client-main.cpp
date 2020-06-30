/**
 * @file client-main.cpp
 * @author Jose Pires
 * @date 04-06-2019
 *
 * @brief client app for RFCommClient
 */
#include "AppClient.h"
#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;
using std::runtime_error;

int main(int argc, char **argv)
{
    AppClient *app;

/* Construct the app */
    try{
        app = new AppClient();
    }catch(runtime_error &e)
    {
        cout << e.what() << endl;
    }

/* Execute the app */
    app->exec();
    return 0;
}
