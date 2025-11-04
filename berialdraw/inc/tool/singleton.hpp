#pragma once
#ifndef SWIG
namespace berialdraw
{
	/** Template class implementing a thread-safe Singleton pattern with double-checked locking.
	This class template ensures that only one instance of the given type T is created.
	It uses double-checked locking to provide a thread-safe implementation and manages
	the mutex using a unique_ptr for explicit memory control.
	
	@tparam T The type of the singleton instance. */
	template <typename T> class Singleton 
	{
	public:
		/** Provides global access to the Singleton instance.
		The instance is created only once, in a thread-safe manner using double-checked locking.
		The mutex is managed using a std::unique_ptr to ensure proper memory cleanup.
		@return A pointer to the Singleton instance of type T.  */
		static T* instance()
		{
			// First check (no locking)
			if (m_instance == 0)
			{
				// Lock the mutex before entering the critical section
				std::lock_guard<std::recursive_mutex> lock(m_mutex);

				// Second check (with locking)
				if (m_instance == 0)
				{
					// Create the singleton instance
					m_instance = new T();
				}
			}
			return m_instance;
		}

		/** Provides global access to the Singleton instance.
		The instance is created only once, in a thread-safe manner using double-checked locking.
		The mutex is managed using a std::unique_ptr to ensure proper memory cleanup.
		@return A reference to the Singleton instance of type T.  */
		static T& it()
		{
			return *instance();
		}

		/** Deletes the singleton instance and cleans up the mutex.
		This function is responsible for explicitly deleting the singleton instance and
		resetting the mutex to ensure no memory leaks. */
		static void clean_up()
		{
			std::lock_guard<std::recursive_mutex> lock(m_mutex);
			delete m_instance;
			m_instance = 0;
		}

		/** Deleted copy constructor.
		Singleton instances cannot be copied. */
		Singleton(const Singleton&) = delete;

		/** Deleted assignment operator.
		Singleton instances cannot be assigned.  */
		Singleton& operator=(const Singleton&) = delete;

	protected:
/// @cond DOXYGEN_IGNORE
		/** Protected constructor.
		The constructor is protected to prevent direct instantiation outside of the Singleton class.*/
		Singleton() = default;

		/** Virtual destructor.
		Ensures proper cleanup in derived classes.  */
		virtual ~Singleton() = default;

		/** Mutex for thread-safety */
		static std::recursive_mutex m_mutex;  

	private:
		static T* m_instance;  ///< Pointer to the unique Singleton instance of type T.
/// @endcond
	};

	// Static member definitions
	template <typename T> T* Singleton<T>::m_instance = 0;

	template <typename T> std::recursive_mutex Singleton<T>::m_mutex;

}
#endif