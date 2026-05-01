#pragma once
namespace berialdraw
{
	/** Shared pointer implementation */
	template<typename T> class SharedPtr
	{
	public:
/// @cond DOXYGEN_IGNORE
		typedef T element_type;
/// @endcond
		/** Default constructor */
		SharedPtr() noexcept : m_ptr(nullptr), m_count(nullptr)
		{
		}


		/** Copy constructor
		@param ptr SharedPtr to copy from */
		SharedPtr(const SharedPtr& ptr)
		{
			m_ptr = ptr.m_ptr;
			++*(m_count = ptr.m_count);
		}

		/** Constructor
		@param ptr Raw pointer to manage */
		explicit SharedPtr(T* ptr)
		{
			m_ptr = 0;
			m_count = 0; 
			try
			{
				m_ptr = ptr;
				m_count = new uint32_t(1); 
			}
			catch(...)
			{
				if (m_ptr)
				{
					delete m_ptr;
					m_ptr = 0;
				}
				if (m_count)
				{
					delete m_count;
					m_count = 0;
				}
			}
		}


		/** Move constructor */
		SharedPtr(SharedPtr&& other) noexcept
			: m_ptr(other.m_ptr), m_count(other.m_count)
		{
			other.m_ptr = nullptr;
			other.m_count = nullptr;
		}

		/** Destructor */
		virtual ~SharedPtr()
		{
			destroy();
		}

		/** Assignment operator
		@param ptr SharedPtr to assign from
		@return Reference to this SharedPtr */
		SharedPtr& operator=(const SharedPtr& ptr)
		{
			if (m_count != ptr.m_count)
			{
				destroy();
				m_ptr = ptr.m_ptr;
				++*(m_count = ptr.m_count);
			}
			return *this;
		}

		/** Dereference operator
		@return Reference to the managed object */
		T& operator*() const
		{
			return *m_ptr;
		}
	
		/** Member access operator
		@return Pointer to the managed object */
		T* operator->() const
		{
			return m_ptr;
		}
			
		/** Get the raw pointer
		@return Raw pointer to the managed object */
		T* get() const noexcept
		{
			return m_ptr;
		}

		/** Get the reference count
		@return Reference count */
		uint32_t count() const noexcept
		{
			return m_count ? *m_count : 0;
		}

	protected:
/// @cond DOXYGEN_IGNORE
		/** Destroy the managed object and the reference count */
		void destroy()
		{
			if (m_count)
			{
				if (--(*m_count) == 0)
				{
					delete m_ptr;
					m_ptr   = 0;
					delete m_count;
					m_count = 0;
				}
			}
		}

		T*      m_ptr;   /**< Raw pointer to the managed object */
		uint32_t* m_count; /**< Reference count */
/// @endcond
	};
}
