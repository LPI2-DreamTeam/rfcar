#ifndef _COM_STREAM_H
#define _COM_STREAM_H

namespace COM{

	class Stream {

	private: 	// Private members

		LL* device;
		uint8 id;
		Stream.ParserCallback parser;
		Direction direction;
		MEM.LinkedList<MEM.CircularBuffer<char*>> buffer;
		uint32 line_len;
		Error last_error;
	
	public: 	// Private methods
		
		/**
		 * @param config
		 */
		Stream(Stream.Config* config);
			
		void clearBuffer();
			
		void emptyBuffer();
			
		void bufferEmpty();
			
		/**
		 * @param payload
		 * @param size
		 * @param packet_id
		 */
		void insertLine(char* payload, uint32 size, uint32 packet_id);
			
		Error getLastError();

	};
}

#endif //_COM_STREAM_H