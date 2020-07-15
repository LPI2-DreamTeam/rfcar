#include "WCOM_Stream.hpp"

namespace WCOM {

    /**
     * @param config
     */
    Stream::Stream(Stream::Config* config) {

    }

    /**
     * @return void
     */
    void Stream::clearBuffer() {
        return;
    }

    void Stream::emptyBuffer() {

    }

    void Stream::bufferEmpty() {

    }

    /**
     * @param payload
     * @param size
     * @param packet_id
     */
    void Stream::insertLine(char* payload, uint32_t size, uint32_t packet_id) {

    }

    /**
     * @return Error
     */
    Error Stream::getLastError() {
        return OK;
    }
}
