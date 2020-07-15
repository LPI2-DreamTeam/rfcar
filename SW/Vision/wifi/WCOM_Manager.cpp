#include "WCOM_Manager.hpp"

namespace WCOM {

    /**
     * @param protocol
     * @param role
     * @param port
     * @return LL*
     */
    LL<>* Manager::createDevice(Protocol protocol, Role role, uint32_t port) {
        return nullptr;
    }

    /**
     * @param stream
     * @return Error
     */
    Error Manager::removeDevice(LL<>* stream) {
        return OK;
    }

    /**
     * @param config
     * @param polling_rate_read
     * @return Stream*
     */
    Stream* Manager::createStream(Stream::Config* config,
                                  CLK::Time polling_rate_read) {
        return nullptr;
    }

    /**
     * @param stream
     * @return Error
     */
    Error Manager::removeStream(Stream* stream) {
        return OK;
    }

    /**
     * @param stream
     * @return uint32_t
     */
    uint32_t Manager::fetchLastMsg(Stream* stream) {
        return 0;
    }

    /**
     * @param buffer
     * @param stream
     * @return Error
     */
    Error Manager::queueMsg(uint8_t* buffer, Stream stream) {
        return OK;
    }

    /**
     * @return Error
     */
    Error Manager::getLastError() {
        return OK;
    }

    /**
     * @param device
     */
    void Manager::fetch(LL<>* device) {

    }
}
