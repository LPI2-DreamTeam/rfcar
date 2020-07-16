#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H

#include "main.hpp"
#include "MEM.hpp"

#ifdef _LINUX_
#include <list>
#include <mutex>
#include <condition_variable>
#include <iostream>
 

#define MAX_ALLOWED_SIZE	8192
#define MIN_ALLOWED_SIZE	2

#endif

namespace MEM {

	template <typename T, uint32_t max_size = 0>
	class CircularList {
	
	private:

#ifdef _LINUX_
		std::mutex list_mutex;					/*! Mutex for use with std::unique_lock in managing accesses and operations */
		std::condition_variable list_condition;	/*! Condition variable for use in waits (see waiting_data: bool) */
#endif
		
		T list[max_size + 1];	/*! Array for element storage */
		int32_t p_write;		/*! Writing iterator */
		int32_t p_read;			/*! Reading iterator */

		Error last_error;	/*! Result from the execution of the last function */
		bool waiting_data;	/*! True when pop() or popBulk() are waiting for an element in BlockingMode::BLOCKING */
		bool dead;			/*! Once dead, the list may no longer be used. Accessing or operating upon it will yield Error::DEAD */


		/*! @return Whether or not the list is empty
		*/
		inline bool _isEmpty() {
			return (p_write == p_read);
		}


		/*! @return Whether or not the list is full
		*/
		inline bool _isFull() {
			return (((p_read - p_write) % (max_size + 1)) == 1);
		}


		/*! @return Number of elements present in the list
		*/
		inline uint32_t _size() {
			return ((p_write - p_read) % (max_size + 1));
		}


		/*! @return Number of spaces left in the list
		*/
		inline uint32_t _available() {
			return ((p_read - p_write - 1) % (max_size + 1));
		}

	public:

		CircularList() {

			this->waiting_data = false;
			
			// Reset list size
			p_write = 0;
			p_read = 0;

			// If max_size is not within allowed bounds, the class should not be allowed to function at all, as it might behave
			// unpredictably otherwise
			if (max_size <= MAX_ALLOWED_SIZE || max_size < MIN_ALLOWED_SIZE) {
				this->dead = false;
				last_error = OK;

			} else {
				this->dead = true;
				last_error = INVALID_SIZE;
			}
		}


		~CircularList() {
			this->empty();
		}


		/*! @brief Push one element into the list
		*
		*	@param value Value to be stored  in the list
		*
		*	@return Result of the operation
		*/
		Error push(T value) {
#ifdef DEBUG
			auto _full_ = _isFull();
			auto _empty_ = _isEmpty();
			auto _size_ = _size();
			auto _available_ = _available();
#endif

			std::unique_lock<std::mutex> lock(this->list_mutex);

			if (this->dead) {
				last_error = DEAD;

			} else if (_isFull()) {
				last_error = FULL;

			} else {

				// Insert the element into the list and increment iterator
				list[(p_write++) % (max_size + 1)] = value;

				// Notify any thread that might be waiting for an object to be inserted into the empty list
				if (this->waiting_data)
					this->list_condition.notify_one();

				last_error = OK;
			}

			return last_error;
		}


		/*! @brief Push multiple elements into the list
		*
		*	@param source Array with the elements to store in the list
		*	@param len Length of elements to be pushed into the list, should there be available space 
		*
		*	@return Result of the operation
		*/
		Error pushBulk(T* source, uint32_t len) {
#ifdef DEBUG
			auto _full_ = _isFull();
			auto _empty_ = _isEmpty();
			auto _size_ = _size();
			auto _available_ = _available();
#endif

			std::unique_lock<std::mutex> lock(this->list_mutex);

			if (this->dead) {
				last_error = DEAD;

			} else if (_isFull()) {
				last_error = FULL;

			} else if (_available() < len) {
				last_error = INVALID_SIZE;		

			} else {
				
				uint32_t it = 0;
				while (it < len) {

					// Insert the element into the list and increment iterator
					list[(p_write++) % (max_size + 1)] = source[it++];
				}

				last_error = OK;

				// Notify any thread that might be waiting for an object to be inserted into the empty list
				if (this->waiting_data)
					this->list_condition.notify_one();
			}

			return last_error;
		}


		/*! @brief Pop one element from the list
		*
		*	@param destination Pointer to the position in memory whre to store the value
		*	@param blocking Whether the function operating mode is BLOCKING or NON_BLOCKING
		*					- In BLOCKING mode, whenever the list is empty, it waits for there to be something in the list and 
		*					pops the first element.
		*					- In NON_BLOCKING mode, whenever the list is empty, it returns halfway through the operation, 
		*					returning Error::EMPTY
		*
		*	@return Result of the operation
		*/
		Error pop(T* destination, BlockingMode blocking = BlockingMode::NON_BLOCKING) {
#ifdef DEBUG
			auto _full_ = _isFull();
			auto _empty_ = _isEmpty();
			auto _size_ = _size();
			auto _available_ = _available();
#endif

			std::unique_lock<std::mutex> lock(this->list_mutex);

			if (this->dead) {
				last_error = DEAD;
				return DEAD;

			} else if (_isEmpty()){

				// If in BlockingMode::NON_BLOCKING, give up immediately
				if (!blocking) {
					last_error = EMPTY;
					return EMPTY;
				}

				// If in BlockingMode::BLOCKING, wait until something appears in the list
				while (_isEmpty()) {
					this->waiting_data = true;
					this->list_condition.wait(lock, [this]() {return this->dead || !_isEmpty(); });
					this->waiting_data = false;
				}

			}

			// Take element from the list and increment iterator
			*destination = this->list[(p_read++) % (max_size + 1)];
			
			last_error = OK;

			return last_error;
		}


		/*! @brief Pop multiple elements from the list
		*
		*	@param destination Array for storing the elements into
		*	@param max Maximum number of elements to be popped from the list, should they be present
		*	@param blocking Whether the function operating mode is BLOCKING or NON_BLOCKING
		*					- In BLOCKING mode, whenever the list is emptied but the operation is still running, it waits for 
		*					there to be something in the list and continues popping elements.
		*					- In NON_BLOCKING mode, whenever the list is emptied but the operation is still running, it returns 
		*					halfway through the operation
		*
		*	@return The number of elements actually popped from the list
		*/
		uint32_t popBulk(T* destination, uint32_t max, BlockingMode blocking = BlockingMode::NON_BLOCKING) {
#ifdef DEBUG
			auto _full_ = _isFull();
			auto _empty_ = _isEmpty();
			auto _size_ = _size();
			auto _available_ = _available();
#endif
			
			std::unique_lock<std::mutex> lock(this->list_mutex);
			uint32_t popped = 0;	/*! Counter of popped items*/
			
			if (this->dead) {
				last_error = DEAD;
				
			} else if (_isEmpty()) {
				last_error = EMPTY;

			} else {

				while (popped < max) {

					// Take element from the list and increment iterator and counter of popped elements
					destination[popped++] = list[(p_read++) % (max_size + 1)];


					// If in BlockingMode::BLOCKING, block until something appears in the list
					if (_isEmpty() && popped == max) {
						
						// If in BlockingMode::NON_BLOCKING, give up immediately
						if (!blocking)
							break;

						// If in BlockingMode::BLOCKING, wait until something appears in the list
						while (_isEmpty()) {
							this->waiting_data = true;
							this->list_condition.wait(lock, [this]() {return this->dead || !_isEmpty(); });
							this->waiting_data = false;
						}

					}
				}

				last_error = OK;
			}

			return popped;
		}


		/*! @brief Get the number of elements in the list
		*
		*	@return The number of elements in the list
		*/
		uint32_t size() {

			std::unique_lock<std::mutex> lock(this->list_mutex);

			if (this->dead)
				last_error = DEAD;
			else
				last_error = OK;

			return _size();
		}


		/*! @brief Break the functioning of the list
		*
		*	Once dead, the list may no longer be used. Accessing or operating upon it will yield Error::DEAD
		*/
		void kill() {

			std::unique_lock<std::mutex> lock(this->list_mutex);

			this->dead = true;
			if (this->waiting_data)
				this->list_condition.notify_all();

			last_error = OK;
		}


		/*! @brief Empty the list
		*/
		inline void empty() {
			std::unique_lock<std::mutex> lock(this->list_mutex);
			this->p_read = this->p_write;
			last_error = OK;
		}


		/*! @brief Get the result from the last function execution
		*
		*	@return General result code from the last function execution
		*/
		inline Error getLastError() {
			std::unique_lock<std::mutex> lock(this->list_mutex);
			return last_error;
		}
	};

}

#endif