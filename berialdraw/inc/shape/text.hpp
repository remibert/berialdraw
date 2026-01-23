#pragma once
namespace berialdraw
{
	/** Class representing text as a drawable shape with specific text styling. */
	class Text : public Shape, public TextStyle
	{
	public:
		/** Friend class to allow Glyphs to access private/protected members. */
		friend class Glyphs;

		/** Constructor
		@param canvas Pointer to the Canvas object where the text will be drawn */
		Text(Canvas * canvas);

		/** Copy constructor
		@param other Text object to copy from */
		Text(const Text & other);

		/** Destructor */
		~Text();

		/** Draw the text label on the specified desktop with a given shift.
		@param shift Reference to the Point object representing the shift to apply when drawing */
		virtual void paint(const Point & shift);

		/** Get the polygon outline of the text.
		@return Reference to the Polygon object representing the text outline (not typically used for text) */
		virtual const Polygon & polygon() const
		{
			return *m_polygon; // Never called in case of text
		}

		/** Get the size of the text content.
		@return Size object representing the content size of the text */
		virtual Size content_size();

#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		Polygon * m_polygon = 0; /**< Pointer to the Polygon object used for the text (not typically used) */
		TextBox   m_text_box;
/// @endcond
	};
}
