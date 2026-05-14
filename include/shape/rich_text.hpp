#pragma once
namespace berialdraw
{
#ifndef SWIG
/// @cond DOXYGEN_IGNORE

	/** Describes a span of text with specific font and color attributes */
	struct TextSpan
	{
		uint32_t start = 0;           ///< Start index in clean text
		uint32_t end = UINT32_MAX;    ///< End index in clean text (exclusive)
		Font *   font = 0;            ///< Font for this span (0 = default font, non-owning)
		uint32_t color = 0;           ///< Text color for this span (0 = default color)
		bool     color_set = false;   ///< Whether color was explicitly set
		Dim      line_height = 16;    ///< Cached line height for this span
		Coord    baseline = 0;        ///< Baseline offset for this span (26.6 fixed point)
	};

	/** Parses a string containing inline formatting tags and provides
	    access to per-character font and color information.

	    Format: $<"font":"arial","size":24,"color":"#FF0000">styled text$$
	    - $<json>  starts a styled span
	    - $$       resets to default style
	*/
	class RichText
	{
	public:
		/** Parse a raw text string with inline formatting tags.
		@param raw_text  The text containing formatting tags
		@param default_font  The default font used when no span overrides it
		@param default_color  The default text color */
		void parse(const String & raw_text, Font & default_font, uint32_t default_color);

		/** Get the clean text (without formatting tags) */
		const String & clean_text() const;

		/** Get the font to use at a given clean text index.
		@param clean_index  Character index in the clean text
		@return Reference to the font for that character */
		Font & font_at(uint32_t clean_index) const;

		/** Get the color to use at a given clean text index.
		@param clean_index  Character index in the clean text
		@return ARGB color value (0 means use default) */
		uint32_t color_at(uint32_t clean_index) const;

		/** Get the character size at a given clean text index, using the correct font.
		@param clean_index  Character index in the clean text
		@return Size of the character in 26.6 fixed point */
		Size char_size_at(uint32_t clean_index) const;

		/** Get the line height for the font at a given clean text index.
		@param clean_index  Character index in the clean text
		@return Line height in 26.6 fixed point */
		Dim line_height_at(uint32_t clean_index) const;

		/** Get the baseline for the font at a given clean text index.
		@param clean_index  Character index in the clean text
		@return Baseline offset in 26.6 fixed point */
		Coord baseline_at(uint32_t clean_index) const;

		/** Check if the text contains any rich formatting */
		bool has_spans() const;

		/** Convert a raw text index (with tags) to a clean text index (without tags).
		@param raw_index  Index in the raw text
		@return Corresponding index in the clean text */
		uint32_t raw_to_clean(uint32_t raw_index) const;

		/** Convert a clean text index (without tags) to a raw text index (with tags).
		@param clean_index  Index in the clean text
		@return Corresponding index in the raw text */
		uint32_t clean_to_raw(uint32_t clean_index) const;

	private:
		/** Find the span index that contains the given clean text index.
		@return index into m_spans, or UINT32_MAX if not found */
		uint32_t span_index_at(uint32_t clean_index) const;

		/** Parse a JSON formatting tag using TextStyle and create a font span */
		void parse_tag(const String & tag_content, Font & default_font, uint32_t default_color, TextSpan & span);

		/** Initialize a text span with default values */
		void init_default_span(TextSpan & span, uint32_t start, Font & font, uint32_t color);

		/** Close the current span and add it to the list if it has content */
		void close_span(TextSpan & span, uint32_t clean_index);

		String m_clean_text;                 ///< Text without formatting tags
		Vector<TextSpan> m_spans;            ///< Formatting spans
		Vector<uint32_t> m_raw_to_clean;     ///< Mapping from raw index to clean index
		Vector<uint32_t> m_clean_to_raw;     ///< Mapping from clean index to raw index
		Font * m_default_font = 0;           ///< Default font (non-owning)
		uint32_t m_default_color = 0;        ///< Default color
		FontPtr m_font_ptrs[16];             ///< Keeps fonts alive (max 16 different fonts)
		uint32_t m_font_ptrs_count = 0;      ///< Number of stored font pointers
	};
/// @endcond
#endif
}
