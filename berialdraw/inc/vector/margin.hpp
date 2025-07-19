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
		bool operator==(const Margin& other) const;

		/** Inequality operator.
		@param other Margin object to compare.
		@return True if margins are not equal. */
		bool operator!=(const Margin& other) const;

		/** Assignment operator.
		@param other Margin object to assign from.
		@return Reference to this object. */
		Margin& operator=(const Margin& other);
    
		/** Set margin values.
		@param top Top margin.
		@param left Left margin.
		@param bottom Bottom margin.
		@param right Right margin. */
		void set(Dim top, Dim left, Dim bottom, Dim right);

		/** Set top margin.
		@param t Top margin. */
		void top(Dim t);

		/** Set left margin.
		@param l Left margin. */
		void left(Dim l);

		/** Set bottom margin.
		@param b Bottom margin. */
		void bottom(Dim b);

		/** Set right margin.
		@param r Right margin */
		void right(Dim r);

		/** Get top margin.
		@return Top margin. */
		Dim top() const;

		/** Get left margin.
		@return Left margin. */
		Dim left() const;

		/** Get bottom margin.
		@return Bottom margin. */
		Dim bottom() const;

		/** Get right margin.
		@return Right margin. */
		Dim right() const;

		/** Set margin values with a precision of 64th of a pixel 
		@param top Top margin.
		@param left Left margin.
		@param bottom Bottom margin.
		@param right Right margin. */
		void set_(Dim top, Dim left, Dim bottom, Dim right);

		/** Set top margin with a precision of 64th of a pixel 
		@param t Top margin. */
		void top_(Dim t);

		/** Set left margin with a precision of 64th of a pixel 
		@param l Left margin. */
		void left_(Dim l);

		/** Set bottom margin with a precision of 64th of a pixel 
		@param b Bottom margin. */
		void bottom_(Dim b);

		/** Set right margin with a precision of 64th of a pixel 
		@param r Right margin. */
		void right_(Dim r);

		/** Get top margin with a precision of 64th of a pixel 
		@return Top margin. */
		Dim top_() const;

		/** Get left margin with a precision of 64th of a pixel 
		@return Left margin. */
		Dim left_() const;

		/** Get bottom margin with a precision of 64th of a pixel 
		@return Bottom margin.*/
		Dim bottom_() const;

		/** Get right margin with a precision of 64th of a pixel 
		@return Right margin. */
		Dim right_() const;

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
