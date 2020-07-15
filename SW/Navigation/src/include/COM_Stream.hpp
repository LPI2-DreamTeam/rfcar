#ifndef _COM_STREAM_H
#define _COM_STREAM_H

#include "COM_LL.hpp"
#include "MEM_LinkedList.hpp"
#include "MEM_CircularList.hpp"

namespace COM{

	class Stream {
	
	public:

		typedef void *(ParserCallback)(uint32_t/*buffer size*/, uint8_t* /*buffer ptr*/) ;

		typedef struct Config_T {
			COM::LL<>* device;
			uint8_t id;
			COM::Stream::ParserCallback parser;
			COM::Direction direction;
		} Config;

	private: 	// Private members

		COM::LL<>* device;
		uint8_t id;
		COM::Stream::ParserCallback parser;
		COM::Direction direction;
		MEM::LinkedList<MEM::CircularList<char*>> buffer;
		uint32_t line_len;
		COM::Error last_error;
	
	public: 	// Private methods
		
		/**
		 * @param config
		 */
		Stream(COM::Stream::Config* config);
			
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

#endif //_COM_STREAM_H