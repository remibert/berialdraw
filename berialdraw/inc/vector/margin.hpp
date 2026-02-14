#pragma once
namespace berialdraw
{
	/** Class representing margin dimensions. */
	class Margin
	{
	public:
		/** Constructor with a single value for all margins.
		@param v Value for all margins. */
		Margin(int v);

		/** Constructor with individual margin values.
		@param t Top margin.
		@param l Left margin.
		@param b Bottom margin.
		@param r Right margin.
		@param pixel Indicates if the values are in pixels. */
		Margin(Dim t = 0, Dim l = 0, Dim b = 0, Dim r = 0, bool pixel=true);

		/** Copy constructor.
		@param p Margin object to copy. */
		Margin(const Margin& p);

		/** Destructor. */
		~Margin();

		/** Equality operator.
		@param other Margin object to compare.
		@return True if margins are equal. */
		bool operator==(const Margin& other) const
		{
			return m_top == other.m_top && m_left == other.m_left && m_bottom == other.m_bottom && m_right == other.m_right;
		}

		/** Inequality operator.
		@param other Margin object to compare.
		@return True if margins are not equal. */
		bool operator!=(const Margin& other) const
		{
			return m_top != other.m_top || m_left != other.m_left || m_bottom != other.m_bottom || m_right != other.m_right;
		}

		/** Assignment operator.
		@param other Margin object to assign from.
		@return Reference to this object. */
		inline Margin& operator=(const Margin& other)
		{
			if (this != &other)
			{
				m_top = other.m_top;
				m_left = other.m_left;
				m_bottom = other.m_bottom;
				m_right = other.m_right;
			}
			return *this;
		}

		/** Set margin values.
		@param top Top margin.
		@param left Left margin.
		@param bottom Bottom margin.
		@param right Right margin. */
		inline void set(Dim top, Dim left, Dim bottom, Dim right)
		{
			this->top(top);
			this->left(left);
			this->bottom(bottom);
			this->right(right);
		}

		/** Set top margin.
		@param t Top margin. */
		inline void top(Dim t)
		{
			m_top = t << 6;
		}

		/** Set left margin.
		@param l Left margin. */
		inline void left(Dim l)
		{
			m_left = l << 6;
		}

		/** Set bottom margin.
		@param b Bottom margin. */
		inline void bottom(Dim b)
		{
			m_bottom = b << 6;
		}

		/** Set right margin.
		@param r Right margin */
		inline void right(Dim r)
		{
			m_right = r << 6;
		}

		/** Get top margin.
		@return Top margin. */
		inline Dim top() const
		{
			return m_top >> 6;
		}

		/** Get left margin.
		@return Left margin. */
		inline Dim left() const
		{
			return m_left >> 6;
		}

		/** Get bottom margin.
		@return Bottom margin. */
		inline Dim bottom() const
		{
			return m_bottom >> 6;
		}

		/** Get right margin.
		@return Right margin. */
		inline Dim right() const
		{
			return m_right >> 6;
		}

		/** Set margin values with a precision of 64th of a pixel 
		@param top Top margin.
		@param left Left margin.
		@param bottom Bottom margin.
		@param right Right margin. */
		inline void set_(Dim top, Dim left, Dim bottom, Dim right)
		{
			top_(top);
			left_(left);
			bottom_(bottom);
			right_(right);
		}

		/** Set top margin with a precision of 64th of a pixel 
		@param t Top margin. */
		inline void top_(Dim t)
		{
			m_top = t;
		}

		/** Set left margin with a precision of 64th of a pixel 
		@param l Left margin. */
		inline void left_(Dim l)
		{
			m_left = l;
		}

		/** Set bottom margin with a precision of 64th of a pixel 
		@param b Bottom margin. */
		inline void bottom_(Dim b)
		{
			m_bottom = b;
		}

		/** Set right margin with a precision of 64th of a pixel 
		@param r Right margin. */
		inline void right_(Dim r)
		{
			m_right = r;
		}

		/** Get top margin with a precision of 64th of a pixel 
		@return Top margin. */
		inline Dim top_() const
		{
			return m_top;
		}

		/** Get left margin with a precision of 64th of a pixel 
		@return Left margin. */
		inline Dim left_() const
		{
			return m_left;
		}

		/** Get bottom margin with a precision of 64th of a pixel 
		@return Bottom margin.*/
		inline Dim bottom_() const
		{
			return m_bottom;
		}

		/** Get right margin with a precision of 64th of a pixel 
		@return Right margin. */
		inline Dim right_() const
		{
			return m_right;
		}

		/** Serialize the margin to JSON.
		@param name Name of the JSON field.
		@param it JSON iterator.    */
		void serialize(const char * name, JsonIterator & it) const;

		/** Unserialize the margin from JSON.
		@param name Name of the JSON field.
		@param it JSON iterator.     */
		void unserialize(const char * name, JsonIterator & it);

	protected:
/// @cond DOXYGEN_IGNORE
		uint16_t m_top;    ///< Top margin
		uint16_t m_left;   ///< Left margin
		uint16_t m_bottom; ///< Bottom margin
		uint16_t m_right;  ///< Right margin
/// @endcond
	};
}
