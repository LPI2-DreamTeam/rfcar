#ifndef _COM_REDUNDANCY_ENGINE_H
#define _COM_REDUNDANCY_ENGINE_H

namespace COM {

	class RedundancyEngine {

		typedef struct Config_T {
			Protocol protocol;
			Role role;
			uint32_t port;
		} Config;

	private: 	// Private members

		MEM::LinkedList<Stream*> streams;
		Error last_error;
	
	public: 	// Private methods
		
		/**
		 * @param config
		 */
		RedundancyEngine(COM::RedundancyEngine::Config* config)
			
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