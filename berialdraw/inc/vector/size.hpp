#pragma once
namespace berialdraw
{
	class Margin;
	/** Class representing a size with width and height. */
	class Size
	{
	public:
		/**  Enumeration for maximum size value.*/
		enum 
		{
			MAX_SIZE = 0x7FFFFFFF, ///< Maximum size value.
		};

		/** Create a size */
		Size();

		/** Create a size */
		Size(Dim w, Dim h, bool pixel=true);

		/** Create a copy of a size */
		Size(const Size& p);

		/** Check if the size is equal */
		inline bool operator==(const Size& other) const
		{
			return m_width == other.m_width && m_height == other.m_height;
		}

		/** Check if the size is not equal */
		inline bool operator!=(const Size& other) const
		{
			return m_width != other.m_width || m_height != other.m_height;
		}

		/** Set the content of size with other size */
		inline Size& operator=(const Size& other)
		{
			if (this != &other)
			{
				this->m_width = other.m_width;
				this->m_height = other.m_height;
				this->m_width_undefined = other.m_width_undefined;
				this->m_height_undefined = other.m_height_undefined;
			}
			return *this;
		}

		/** Set size with width and height in pixels */
		inline void set(Dim w, Dim h)
		{
			width(w);
			height(h);
		}

		/** Get the height in pixels */
		inline Dim height() const
		{
			return (m_height+63) >> 6;
		}

		/** Get the width in pixels */
		inline Dim width() const
		{
			return (m_width+63) >> 6;
		}

		/** Set height in pixels */
		inline void height(Dim h)
		{
			if (h != Size::MAX_SIZE)
			{
				m_height = h << 6;
			}
			else
			{
				m_height = h;
			}
			m_height_undefined = 0;
		}

		/** Set the width in pixels */
		inline void width(Dim w)
		{
			if (w != Size::MAX_SIZE)
			{
				m_width  = w << 6;
			}
			else
			{
				m_width = w;
			}
			m_width_undefined = 0;
		}

		/** Set size with width and height with a precision of 64th of a pixel */
		inline void set_(Dim w, Dim h)
		{
			m_width  = w;
			m_height = h;
			m_width_undefined = 0;
			m_height_undefined = 0;
		}

		/** Get the height with a precision of 64th of a pixel */
		inline Dim height_() const
		{
			return m_height;
		}

		/** Get the width with a precision of 64th of a pixel */
		inline Dim width_() const
		{
			return m_width;
		}

		/** Set the height with a precision of 64th of a pixel */
		inline void height_(Dim h)
		{
			m_height = h;
			m_height_undefined = 0;
		}

		/** Set the width and height with a precision of 64th of a pixel */
		inline void width_(Dim w)
		{
			m_width = w;
			m_width_undefined = 0;
		}

		/** Get the middle of size */
		Size middle() const
		{
			Size  m(m_width>>1, m_height>>1, false);
			return m;
		}

		/** Decrease size with width and height in pixels */
		void decrease(const Margin & margin);

		/** Increase size with width and height in pixels */
		void increase(const Margin & margin);

		/** Decrease size with width and height in pixels */
		void decrease(const Size & size)
		{
			decrease_(size.width_(), size.height_());
		}

		/** Increase size with width and height in pixels */
		void increase(const Size & size)
		{
			increase(size.width_(), size.height_());
		}

		/** Decrease size with width and height in pixels */
		void decrease(Dim w, Dim h)
		{
			decrease_(w<<6, h<<6);
		}

		/** Increase size with width and height in pixels */
		void increase(Dim w, Dim h)
		{
			increase_(w<<6, h<<6);
		}

		/** Decrease size with width and height with a precision of 64th of a pixel */
		void decrease_(Dim w, Dim h);

		/** Increase size with width and height with a precision of 64th of a pixel */
		void increase_(Dim w, Dim h);

		/** Serialize the content of size to json */
		void serialize(const char * name, JsonIterator & it) const;

		/** Unserialize the content of widget from json */
		void unserialize(const char * name, JsonIterator & it);

		/** Indicates if width is not defined */
		bool is_width_undefined() const
		{
			return m_width_undefined;
		}

		/** Indicates if height is not defined */
		bool is_height_undefined() const
		{
			return m_height_undefined;
		}

		/** Clean the size and set to undefined */
		void clean()
		{
			m_width = 0;
			m_height = 0;
			m_width_undefined = 1;
			m_height_undefined = 1;
		}

		/** Print content */
		void print(const char * name) const;

		/** Positions itself on the nearest pixel */
		void nearest_pixel()
		{
			m_width  = ((m_width  + 32) >> 6) << 6;
			m_height = ((m_height + 32) >> 6) << 6;
			m_width_undefined = 0;
			m_height_undefined = 0;
		}

		/** Substract b to a and avoid negative value */
		static inline Dim subs(Dim a, Dim b)
		{
			if (b >= a)
			{
				return 0;
			}
			return a-b;
		}
	protected:
/// @cond DOXYGEN_IGNORE
		/** width coordinate with a precision of 64th of a pixel  */
		unsigned int m_width:31;

		/** Indicates that width is not yet defined */
		unsigned int m_width_undefined:1;

		/** height coordinate with a precision of 64th of a pixel */
		unsigned int m_height:31;

		/** Indicates that height is not yet defined */
		unsigned int m_height_undefined:1;
/// @endcond
	};
}
