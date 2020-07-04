/**
 * @file Database.h
 * @author Jose Pires
 * @date 04-06-2019
 *
 * @brief Interface for database module
 *
 * Loads the settings file and log file at startup, and enables appending to
 * the database file
 */
#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>

using std::string;
using std::ifstream;
using std::fstream;

class Database{
    ifstream *setFile; /**< Settings file */
    fstream *logFile; /**< Log file */
public:
/**
 * @brief Constructs a database
 */
    Database(const string &fname);
/**
 * @brief Destructs a database
 */
    virtual ~Database();

/**
 * @brief Loads the database
 * @return a string with the serialized definitions
 *
 * The caller (the app) should use this string as required.
 *
 * Exceptions thrown:
 * - 1. If database is not opened
 */
    string load();
    
/**
 * @brief Gets a string from the history of the database
 *
 * It must be called until an string is empty
 */
string history();

/**
 * @brief writes the message to the database
 * @param str: message to write
 *
 * Exceptions thrown:
 * - 1. If database is not writable or opened
 */
    void write(const string &str);
};

#endif // DATABASE_H

