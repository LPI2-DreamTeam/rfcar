#include "Mutex.h"



Mutex::Mutex() : self(), owner(empty_thread_id) {}

Mutex::~Mutex() {}
