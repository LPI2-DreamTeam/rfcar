#ifndef _WCOM_STREAM_H
#define _WCOM_STREAM_H

#include "WCOM_LL.hpp"
#include "MEM_LinkedList.hpp"
#include "MEM_CircularList.hpp"

namespace WCOM{

    class Stream {
	
    public:

        typedef void *(ParserCallback)(uint32_t, uint8_t);

        typedef struct Config_T {
            WCOM::LL<>* device;
            uint8_t id;
            WCOM::Stream::ParserCallback parser;
            WCOM::Direction direction;
        } Config;

    private: 	// Private members
        
        WCOM::LL<>* device;
        uint8_t id;
        WCOM::Stream::ParserCallback parser;
        WCOM::Direction direction;
        MEM::LinkedList<MEM::CircularList<char*> > buffer;
        uint32_t line_len;
        WCOM::Error last_error;
	
    public: 	// Private methods
		
		/**
		 * @param config
		 */
        Stream(WCOM::Stream::Config* config);
			
        void clearBuffer();
			
        void emptyBuffer();
			
        void bufferEmpty();
			
        /**
         * @param payload
         * @param size
         * @param packet_id
         */
        void insertLine(char* payload, uint32_t size, uint32_t packet_id);
			
        Error getLastError();

    };
}

#endif //_WCOM_STREAM_H
