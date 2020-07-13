#include "IO_GPIO.hpp"

namespace IO {

	GPIO::States GPIO::global_states = {0, 0, 0};

	GPIO::GPIO() {

	}


	Error GPIO::configure(Config* config) {
		return OK;
	}


	bool GPIO::run() {
		return false;
	}


	Error GPIO::insertNewConversion(number value) {
		return OK;
	}


	Error GPIO::fetchLastConversion() {
		return OK;
	}


	bool GPIO::isConfigured() {
		return false;
	}


	bool GPIO::isAssigned() {
		return false;
	}


	bool GPIO::isRunning() {
		return false;
	}

	GPIO* GPIO::grabAvailableObject() {
		return nullptr;
	}


	GPIO* GPIO::releaseObject() {
		return nullptr;
	}
}