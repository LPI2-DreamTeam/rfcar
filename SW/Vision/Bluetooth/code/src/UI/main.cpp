/**
 * @file main.cpp
 * @author Pedro Duarte
 * @date 23-06-2019
 *
 * @brief Main function for program
 */
#include "mainwindow.h"
#include <QApplication>

/**
 * @brief Main function for program
 * @param argc: nr of args
 * @param argv: array of arguments
 * @return execution status
 *
 * Initialization: Starts the application, creates the main window, and 
 * displays to user
 * Event Loop: runs the program event loop
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
