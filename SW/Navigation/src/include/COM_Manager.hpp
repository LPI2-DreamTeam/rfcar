/**
 * Project Untitled
 */


#ifndef _COM_MANAGER_H
#define _COM_MANAGER_H

namespace COM {

	class Manager {

	private: 

		MEM.LinkedList<Stream> streams;
		MEM.LinkedList<RedundancyEngine> redundancy_engines;
		CLK.Timer timer;
		Error last_error;
		
	private:	// Private methods

		/**
		 * @param device
		 */
		void fetch(COM::LL* device);

	public: 	// Public methods
		
		/**
		 * @param protocol
		 * @param role
		 * @param port
		 */
		LL* createDevice(Protocol protocol, Role role, uint32 port);
			
		/**
		 * @param stream
		 */
		Error removeDevice(LL* stream);
			
		/**
		 * @param config
		 * @param polling_rate_read
		 */
		Stream* createStream(Stream.Config* config, CLK.Time polling_rate_read);
			
		/**
		 * @param stream
		 */
		Error removeStream(Stream* stream);
			
		/**
		 * @param stream
		 */
		uint32 fetchLastMsg(Stream* stream);
			
		/**
		 * @param buffer
		 * @param stream
		 */
		Error queueMsg(uint8_t* buffer, Stream stream);
			
		Error getLastError();

	};
}

#endif //_COM_MANAGER_H