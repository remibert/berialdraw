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
		Coord baseline_ref = 0;  ///< Reference baseline for the line (26.6 fixed point)
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

		void paint(const Point & cursor_shift, Font & font, 
			const String & text, const Point & position, const Area & area, uint32_t text_color, 
			uint32_t cursor_color, uint32_t select_color, bool insertion);

		void paint(Font& font, const String& text, const Point& position, const Area& area, uint32_t text_color);

		/** Paint the text lines */
		void paint(Font& font,
			const String& text, const Point& position, const Point& center,
			const Margin& margin, Coord angle, uint32_t text_color);

		/** Paint the text lines */
		void paint(const Point& cursor_shift, Font& font,
			const String& text, const Point& position, const Point& center,
			const Margin& margin, Coord angle, uint32_t text_color,
			uint32_t cursor_color, uint32_t select_color, bool insertion);

	private:
		/** Process a normal (non-newline) character: track cursor position and advance the line width */
		void process_character(uint32_t i, uint32_t cursor_pos, LineInfo & info, Dim & line_width);

		/** Finalize the current line when a newline character is found, and reset the running state for the next line */
		void finalize_line_at_newline(uint32_t i, uint32_t cursor_pos, Font & font, Dim line_height,
			LineInfo & info, Dim & line_width, Dim & max_line_height, Coord & max_baseline);

		/** Finalize the trailing line(s) after the main character loop: a line ending exactly on a trailing
		carriage return, and/or the last line when it has no terminating newline */
		void finalize_trailing_line(uint32_t i, uint32_t cursor_pos, uint32_t sel_start, uint32_t sel_end,
			const Size & space_size, LineInfo info, Dim line_width, Dim max_line_height, Coord max_baseline);

		/** Move each line's x position and the cursor/selection according to the horizontal text alignment */
		void apply_alignment(const Area & text_area, Align text_align, uint32_t cursor_pos);

		/** Paint the selection background rectangle for a single line, if it has a selection */
		void paint_selection(const LineInfo & line, const Point & position, const Point & cursor_shift,
			Dim accumulated_height, const Point & line_center, Coord angle, uint32_t select_color);

		/** Paint a single line's text, segment by segment (each segment shares the same font and color) */
		void paint_line_segments(const LineInfo & line, const Point & position, const Point & cursor_shift,
			Dim accumulated_height, const Point & line_center, const Margin & margin, Coord angle, uint32_t text_color);

		/** Paint the text cursor rectangle if it is located on this line */
		void paint_cursor(uint32_t i, const LineInfo & line, const Point & position, const Point & cursor_shift,
			Dim accumulated_height, const Point & line_center, Coord angle, uint32_t cursor_color, bool insertion);

	protected:
		RichText m_rich_text;
		Vector<LineInfo> m_lines;
		Size  m_lines_size;
		Size m_cursor_size;
		Point m_cursor_pos;
		Dim   m_cursor_line;
	};
/// @endcond
#endif
}
