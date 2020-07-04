/**
 * @file btchat-main.cpp
 * @author Jose Pires
 * @date 19-06-2019
 *
 * @brief Main for BT chat application
 */
#include "App.h"
#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;
using std::runtime_error;

int main(int argc, char **argv)
{
    App *app;

/* Construct the app */
    try{
        app = new App();
    }catch(runtime_error &e){
        cout << e.what() << endl;
        return -1;
    }

/* Execute the app */
    app->exec();
    return 0;
}

