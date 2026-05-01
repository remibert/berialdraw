#pragma once
namespace berialdraw
{
#ifndef SWIG
/// @cond DOXYGEN_IGNORE
	class Entry;
	class Region;
	
	struct LineInfo
	{
		Size size;
		Point position;
		Coord selection_start = UINT32_MAX;
		Coord selection_end   = UINT32_MAX;
		uint32_t line_start = UINT32_MAX;
		uint32_t line_end   = UINT32_MAX;
	};

	/** Class representing text box */
	class TextBox
	{
	public:
		/** Constructor */
		TextBox();

		/** Destructor */
		~TextBox();

		/** Get the size of the TextBox content.
		@return Size object representing the content size of the TextBox */
		virtual Size content_size();

		/** Parse the text to search each line, selection position and cursor position */
		void parse(const Area & text_area, Font & font, String & text, uint32_t cursor_pos, uint32_t sel_start, uint32_t sel_end, Align text_align);

		/** Get the cursor position according to the click location */
		uint32_t cursor_position(const Point & click_location, const Area & text_area, const Point & text_shift,  Font & font, const String & text, Align text_align);

		/** Moves the text to make the area containing the cursor visible. */
		void text_shift(Point & cursor_shift, const Area & text_area);

		/** Moves the text to make the area containing the cursor visible. */
		Coord horizontal_shift(Area & cursor_clip, Point & cursor_shift, const Area & text_area);

		/** Moves the text to make the area containing the cursor visible. */
		Coord vertical_shift(Area & cursor_clip, Point & cursor_shift, const Area & text_area);

		/** Paint the text lines */
		void paint(const Point & cursor_shift, Font & font, const String & text, const Point & position, 
			const Point & center, const Margin & margin,  Coord angle, uint32_t text_color, 
			uint32_t cursor_color, uint32_t select_color, bool insertion);

		void paint(const Point & cursor_shift, Font & font, 
			const String & text, const Point & position, const Area & area, uint32_t text_color, 
			uint32_t cursor_color, uint32_t select_color, bool insertion);

	protected:

		Vector<LineInfo> m_lines;
		Size  m_lines_size;
		Size m_cursor_size;
		Point m_cursor_pos;
		Dim   m_cursor_line;
	};
/// @endcond
#endif
}
