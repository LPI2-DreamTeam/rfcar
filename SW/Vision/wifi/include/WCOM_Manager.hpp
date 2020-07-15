#ifndef _WCOM_MANAGER_H
#define _WCOM_MANAGER_H

#include "CLK_Timer.hpp"
#include "MEM_LinkedList.hpp"
#include "CLK_Timer.hpp"
#include "WCOM_Stream.hpp"
#include "WCOM_RedundancyEngine.hpp"
#include "WCOM_LL.hpp"

namespace WCOM {

    class Manager {

    private: 

        MEM::LinkedList<Stream> streams;
        MEM::LinkedList<RedundancyEngine> redundancy_engines;
        CLK::Timer timer;
        Error last_error;
		
    private:	// Private methods

		/**
		 * @param device
		 */
        void fetch(LL<>* device);

    public: 	// Public methods
		
		/**
		 * @param protocol
		 * @param role
		 * @param port
		 */
        LL<>* createDevice(Protocol protocol, Role role, uint32_t port);
			
        /**
         * @param stream
         */
        Error removeDevice(LL<>* stream);
			
        /**
         * @param config
         * @param polling_rate_read
         */
        Stream* createStream(WCOM::Stream::Config* config,
                             CLK::Time polling_rate_read);
			
        /**
         * @param stream
         */
        Error removeStream(Stream* stream);
			
        /**
         * @param stream
         */
        uint32_t fetchLastMsg(Stream* stream);
			
        /**
         * @param buffer
         * @param stream
         */
        Error queueMsg(uint8_t* buffer, Stream stream);
			
        Error getLastError();

    };
}

#endif //_WCOM_MANAGER_H
