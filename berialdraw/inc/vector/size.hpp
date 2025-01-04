#ifndef Size_hpp_INCLUDED
#define Size_hpp_INCLUDED

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

		/** Destroy size */
		~Size();

		/** Check if the size is equal */
		bool operator==(const Size& other) const;

		/** Check if the size is not equal */
		bool operator!=(const Size& other) const;

		/** Set the content of size with other size */
		Size& operator=(const Size& other);

		/** Set size with width and height in pixels */
		void set(Dim w, Dim h);

		/** Get the height in pixels */
		Dim height() const;

		/** Get the width in pixels */
		Dim width() const;

		/** Set height in pixels */
		void height(Dim h);

		/** Set the width in pixels */
		void width(Dim w);

		/** Set size with width and height with a precision of 64th of a pixel */
		void set_(Dim w, Dim h);

		/** Get the height with a precision of 64th of a pixel */
		Dim height_() const;

		/** Get the width with a precision of 64th of a pixel */
		Dim width_() const;

		/** Set the height with a precision of 64th of a pixel */
		void height_(Dim h);

		/** Set the width and height with a precision of 64th of a pixel */
		void width_(Dim w);

		/** Get the middle of size */
		Size middle() const;


		/** Decrease size with width and height in pixels */
		void decrease(const Margin & margin);

		/** Increase size with width and height in pixels */
		void increase(const Margin & margin);


		/** Decrease size with width and height in pixels */
		void decrease(const Size & size);

		/** Increase size with width and height in pixels */
		void increase(const Size & size);


		/** Decrease size with width and height in pixels */
		void decrease(Dim w, Dim h);

		/** Increase size with width and height in pixels */
		void increase(Dim w, Dim h);

		/** Decrease size with width and height with a precision of 64th of a pixel */
		void decrease_(Dim w, Dim h);

		/** Increase size with width and height with a precision of 64th of a pixel */
		void increase_(Dim w, Dim h);

		/** Resizes itself on the nearest pixel */
		void nearest_pixel();

		/** Serialize the content of widget into json */
		void serialize(const char * name, JsonIterator & it) const;

		/** Unserialize the content of widget from json */
		void unserialize(const char * name, JsonIterator & it);

		/** Indicates if width is not defined */
		bool is_width_undefined() const;

		/** Indicates if height is not defined */
		bool is_height_undefined() const;

		/** Clean the size and set to undefined */
		void clean();

		/** Print content */
		void print(const char * name) const;

		/** Substract b to a and avoid negative value */
		static Dim subs(Dim a, Dim b);
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
#endif