#pragma once
namespace berialdraw
{
	/** Specialized vector class allowing you to add an element or remove it at a given location */
	template<class T> class Vector
	{
	public:
		/** Constructor */
		Vector()
		{
		}

		/** Destructor */
		virtual ~Vector()
		{
		}

		/** Returns the number of elements in the vector */
		inline uint32_t size() const
		{
			return (uint32_t)m_vector.size();
		}

		/** Indicates if index is in vector */
		inline bool inbound(int32_t index) const
		{
			bool result = false;
			if(index < 0)
			{
				if(((uint32_t)-index) <= m_vector.size())
				{
					result = true;
				}
			}
			else
			{
				if(index < (int)m_vector.size())
				{
					result = true;
				}
			}
			return result;
		}

		/** Provides read access to element at the given index in the vector. */
		inline T & operator[](int32_t index)
		{
			if (index < 0)
			{
				return m_vector[m_vector.size() +index];
			}
			else
			{
				return m_vector[index];
			}
		}

		/** Provides read access to element at the given index in the vector. */
		inline const T operator[](int32_t index) const
		{
			if (index < 0)
			{
				return m_vector[m_vector.size() +index];
			}
			else
			{
				return m_vector[index];
			}
		}

		/** Erases the elements of the vector */
		inline void clear()
		{
			m_vector.clear();
		}

		/** Tests if the vector is empty at the time this method is called */
		inline bool empty() const
		{
			return m_vector.empty();
		}

		/** Returns the maximum size to which the vector can grow without having to allocate more memory */
		inline uint32_t capacity() const
		{
			return m_vector.capacity();
		}

		/** Appends the given item to the end of the vector */
		inline void push_back(T value)
		{
			m_vector.push_back(value);
		}

		/** Appends the given item to the end of the vector or set empty item if found */
		inline void push_or_reuse(T value)
		{
			for (size_t i = 0; i < size(); i++)
			{
				if (m_vector[i] == 0)
				{
					m_vector[i] = value;
					return;
				}
			}
			push_back(value);
		}

		/** Allocates enough space to grow the vector to size n without having to allocate more memory later */
		inline void reserve(uint32_t n)
		{
			m_vector.reserve(n);
		}

		/** Changes the size of the vector to the requested size, deleting or adding elements as necessary */
		inline void shrink_to_fit()
		{
			m_vector.shrink_to_fit();
		}

		/** Changes the size of the vector to the requested size, deleting or adding elements as necessary */
		inline void resize(uint32_t n)
		{
			m_vector.resize(n);
		}

		/** Insert item in the vector */
		inline void insert(int32_t index, T value)
		{
			if(index < 0)
			{
				index = (int32_t)m_vector.size() + index + 1;
			}

			if (index >= 0 && index < (int)m_vector.size())
			{
				// insert new item in vector
				m_vector.insert(m_vector.begin() + index, value);
			}
			else
			{
				m_vector.push_back(value);
			}
		}

		/** Remove item in the vector */
		inline void remove(int32_t index)
		{
			if(index < 0)
			{
				index = (int32_t)m_vector.size() + index;
			}
			m_vector.erase(m_vector.begin() + index);
		}

	protected:
#ifndef SWIG
/// @cond DOXYGEN_IGNORE
		std::vector<T> m_vector;
/// @endcond
#endif
	};
}
