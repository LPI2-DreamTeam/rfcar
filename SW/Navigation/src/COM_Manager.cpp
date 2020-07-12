#include "COM_Manager.hpp"

namespace COM {

		/**
	 * @param protocol
	 * @param role
	 * @param port
	 * @return LL*
	 */
	LL* Manager::createDevice(Protocol protocol, Role role, uint32 port) {
		return null;
	}

	/**
	 * @param stream
	 * @return Error
	 */
	Error Manager::removeDevice(LL* stream) {
		return null;
	}

	/**
	 * @param config
	 * @param polling_rate_read
	 * @return Stream*
	 */
	Stream* Manager::createStream(Stream.Config* config, CLK.Time polling_rate_read) {
		return null;
	}

	/**
	 * @param stream
	 * @return Error
	 */
	Error Manager::removeStream(Stream* stream) {
		return null;
	}

	/**
	 * @param stream
	 * @return uint32
	 */
	uint32 Manager::fetchLastMsg(Stream* stream) {
		return null;
	}

	/**
	 * @param buffer
	 * @param stream
	 * @return Error
	 */
	Error Manager::queueMsg(uint8_t* buffer, Stream stream) {
		return null;
	}

	/**
	 * @return Error
	 */
	Error Manager::getLastError() {
		return null;
	}

	/**
	 * @param device
	 */
	void Manager::fetch(LL* device) {

	}
}