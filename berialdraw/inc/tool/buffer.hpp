#pragma once

namespace berialdraw
{
	/** Simple byte buffer for caching decompressed ZIP data */
	class Buffer
	{
	public:
		Buffer() = default;
		~Buffer() = default;

		/** Allocate buffer of specified size */
		void allocate(uint32_t size)
		{
			m_data.resize(size);
			m_position = 0;
		}

		/** Get pointer to buffer data */
		uint8_t* data()
		{
			return m_data.data();
		}

		/** Get const pointer to buffer data */
		const uint8_t* data() const
		{
			return m_data.data();
		}

		/** Get current size */
		uint32_t size() const
		{
			return static_cast<uint32_t>(m_data.size());
		}

		/** Clear buffer */
		void clear()
		{
			m_data.clear();
			m_position = 0;
		}

		/** Read from buffer at current position */
		uint32_t read(void *ptr, uint32_t count)
		{
			if (!ptr || count == 0 || m_position >= m_data.size())
			{
				return 0;
			}

			uint32_t available = static_cast<uint32_t>(m_data.size()) - m_position;
			uint32_t to_read = (count < available) ? count : available;

			memcpy(ptr, m_data.data() + m_position, to_read);
			m_position += to_read;

			return to_read;
		}

		/** Seek to position */
		int seek(long offset, int whence)
		{
			long new_position = 0;

			switch (whence)
			{
				case 0: // SEEK_SET
					new_position = offset;
					break;
				case 1: // SEEK_CUR
					new_position = static_cast<long>(m_position) + offset;
					break;
				case 2: // SEEK_END
					new_position = static_cast<long>(m_data.size()) + offset;
					break;
				default:
					return -1;
			}

			if (new_position < 0 || new_position > (long)m_data.size())
			{
				return -1;
			}

			m_position = new_position;
			return 0;
		}

		/** Get current position */
		long tell() const
		{
			return m_position;
		}

		/** Set position */
		void set_position(uint32_t pos)
		{
			if (pos <= m_data.size())
			{
				m_position = pos;
			}
		}

	private:
		std::vector<uint8_t> m_data;
		uint32_t m_position = 0;
	};
}
