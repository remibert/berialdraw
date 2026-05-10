#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Parse a raw text string with inline formatting tags */
void RichText::parse(const String & raw_text, Font & default_font, uint32_t default_color)
{
	m_clean_text = "";
	m_spans.clear();
	m_raw_to_clean.clear();
	m_clean_to_raw.clear();
	m_default_font = &default_font;
	m_default_color = default_color;
	m_font_ptrs_count = 0;

	// Safety check: ensure default_font is valid
	if (m_default_font != 0)
	{
		uint32_t raw_count = raw_text.count();
		uint32_t clean_index = 0;
		uint32_t i = 0;
		String tag_content;

		TextSpan current_span;
		init_default_span(current_span, 0, default_font, default_color);
		m_raw_to_clean.reserve(raw_count);

		while (i < raw_count)
		{
			wchar_t ch = raw_text.get(i);

			// Check for § character (tag delimiter)
			if (ch == L'\u00A7' && i + 1 < raw_count)
			{
				wchar_t next = raw_text.get(i + 1);

				if (next == L'{')
				{
					// §{ : start of formatting tag definition
					m_raw_to_clean.push_back(clean_index);
					m_raw_to_clean.push_back(clean_index);
					i += 2;

					if (extract_tag_content(raw_text, i, clean_index, tag_content))
					{
						close_span(current_span, clean_index);
						TextSpan new_span;
						new_span.start = clean_index;
						parse_tag(tag_content, default_font, default_color, new_span);
						current_span = new_span;
					}
					continue;
				}
				else if (next == L'\u00A7')
				{
					// §§ : reset to default style
					m_raw_to_clean.push_back(clean_index);
					m_raw_to_clean.push_back(clean_index);
					i += 2;

					close_span(current_span, clean_index);
					init_default_span(current_span, clean_index, default_font, default_color);
					continue;
				}
			}

			// Normal character : add to clean text and mappings
			m_raw_to_clean.push_back(clean_index);
			m_clean_to_raw.push_back(i);
			m_clean_text.write_char(ch);
			clean_index++;
			i++;
		}

		close_span(current_span, clean_index);

		// Ensure there's always at least one span with a valid font
		if (m_spans.size() == 0)
		{
			TextSpan default_span;
			init_default_span(default_span, 0, default_font, default_color);
			default_span.end = clean_index;
			m_spans.push_back(default_span);
		}

		// Add a final mapping entry for end-of-text
		m_clean_to_raw.push_back(raw_count);
	}
}

/** Initialize a text span with default values */
void RichText::init_default_span(TextSpan & span, uint32_t start, Font & font, uint32_t color)
{
	span.start = start;
	span.end = UINT32_MAX;
	span.font = &font;
	span.color = color;
	span.color_set = false;
	span.line_height = font.real_size().height_();
	span.baseline = font.baseline_();
}

/** Close the current span and add it to the list if it has content */
void RichText::close_span(TextSpan & span, uint32_t clean_index)
{
	if (clean_index > span.start)
	{
		span.end = clean_index;
		m_spans.push_back(span);
	}
}

/** Extract tag content between §{ and }§ */
bool RichText::extract_tag_content(const String & raw_text, uint32_t & i, uint32_t clean_index, String & tag_content)
{
	bool found_end = false;
	tag_content = "";

	while (i < raw_text.count())
	{
		wchar_t tc = raw_text.get(i);
		if (tc == L'}' && i + 1 < raw_text.count() && raw_text.get(i + 1) == L'\u00A7')
		{
			m_raw_to_clean.push_back(clean_index);
			m_raw_to_clean.push_back(clean_index);
			i += 2;
			found_end = true;
			break;
		}
		else
		{
			m_raw_to_clean.push_back(clean_index);
			tag_content.write_char(tc);
			i++;
		}
	}

	return found_end;
}

/** Get the clean text (without formatting tags) */
const String & RichText::clean_text() const
{
	return m_clean_text;
}

/** Get the font to use at a given clean text index */
Font & RichText::font_at(uint32_t clean_index) const
{
	Font * result = 0;
	uint32_t idx = span_index_at(clean_index);
	if (idx != UINT32_MAX && m_spans[idx].font != 0)
	{
		result = m_spans[idx].font;
	}
	else if (m_spans.size() > 0 && m_spans[0].font != 0)
	{
		result = m_spans[0].font;
	}
	else
	{
		// Should never reach here if parse() was called properly
		// All spans are initialized with a valid font
		assert(false && "RichText::font_at() - no valid font available");
		
		// This is unreachable, but needed for compiler
		static Font * fallback = 0;
		result = fallback;
	}
	
	return *result;
}


/** Get the color to use at a given clean text index */
uint32_t RichText::color_at(uint32_t clean_index) const
{
	uint32_t idx = span_index_at(clean_index);
	if (idx != UINT32_MAX && m_spans[idx].color_set)
	{
		return m_spans[idx].color;
	}
	return m_default_color;
}

/** Get the character size at a given clean text index */
Size RichText::char_size_at(uint32_t clean_index) const
{
	Font & font = font_at(clean_index);
	Size result = font.real_size();
	
	// Bounds check on clean text
	if (clean_index < m_clean_text.count())
	{
		wchar_t ch = m_clean_text.get(clean_index);
		result = font.char_size(ch);
	}
	
	return result;
}

/** Get the line height for the font at a given clean text index */
Dim RichText::line_height_at(uint32_t clean_index) const
{
	Font & font = font_at(clean_index);
	return font.real_size().height_();
}

/** Get the baseline for the font at a given clean text index */
Coord RichText::baseline_at(uint32_t clean_index) const
{
	Coord result = 0;
	uint32_t idx = span_index_at(clean_index);
	if (idx != UINT32_MAX)
	{
		result = m_spans[idx].baseline;
	}
	return result;
}

/** Check if the text contains any rich formatting */
bool RichText::has_spans() const
{
	bool result = false;
	
	// Has spans if there are multiple spans, or a single span with custom font/color
	if (m_spans.size() > 1)
	{
		result = true;
	}
	else if (m_spans.size() == 1)
	{
		if (m_spans[0].font != 0 || m_spans[0].color_set)
		{
			result = true;
		}
	}
	
	return result;
}

/** Convert a raw text index to a clean text index */
uint32_t RichText::raw_to_clean(uint32_t raw_index) const
{
	uint32_t result = 0;
	if (raw_index < m_raw_to_clean.size())
	{
		result = m_raw_to_clean[raw_index];
	}
	else if (m_raw_to_clean.size() > 0)
	{
		result = m_clean_text.count();
	}
	return result;
}

/** Convert a clean text index to a raw text index */
uint32_t RichText::clean_to_raw(uint32_t clean_index) const
{
	uint32_t result = 0;
	if (clean_index < m_clean_to_raw.size())
	{
		result = m_clean_to_raw[clean_index];
	}
	else if (m_clean_to_raw.size() > 0)
	{
		result = m_clean_to_raw[m_clean_to_raw.size() - 1];
	}
	return result;
}

/** Find the span index that contains the given clean text index */
uint32_t RichText::span_index_at(uint32_t clean_index) const
{
	uint32_t result = UINT32_MAX;
	for (uint32_t i = 0; i < m_spans.size(); i++)
	{
		if (clean_index >= m_spans[i].start && clean_index < m_spans[i].end)
		{
			result = i;
			break;
		}
	}
	
	// If no exact match found, return the last span as fallback
	// (handles boundary cases where spans are adjacent but don't overlap)
	if (result == UINT32_MAX && m_spans.size() > 0)
	{
		result = m_spans.size() - 1;
	}
	
	return result;
}

/** Parse a JSON formatting tag using TextStyle and create a font span */
void RichText::parse_tag(const String & tag_content, Font & default_font, uint32_t default_color, TextSpan & span)
{
	init_default_span(span, span.start, default_font, default_color);

	bool valid_json = false;
	String json_str;
	json_str.print("{%s}", tag_content.c_str());

	Json json;
	try
	{
		json.unserialize(json_str.c_str());
		valid_json = true;
	}
	catch (JsonUnserializeError &)
	{
		valid_json = false;
	}

	if (valid_json)
	{
		TextStyle style;
		style.font_size(default_font.pixel_size());
		style.text_color(default_color);

		JsonIterator it(json);
		style.unserialize(it);
		style.select_font();

		// Extract color if specified
		if (it.exists(StyleNames::TEXT_COLOR))
		{
			span.color = style.text_color();
			span.color_set = true;
		}

		// Extract font if font-related properties specified
		if (it.exists(StyleNames::TEXT_FONT_FAMILY) || it.exists(StyleNames::TEXT_FONT_SIZE))
		{
			Fonts * fonts = UIManager::fonts();
			if (fonts != 0)
			{
				FontPtr font_ptr = fonts->select(style.font_familly(), style.font_size());
				if (font_ptr.get() != 0)
				{
					if (m_font_ptrs_count < 16)
					{
						m_font_ptrs[m_font_ptrs_count] = font_ptr;
						m_font_ptrs_count++;
					}
					span.font = font_ptr.get();
				}
			}
		}

		// Update line_height and baseline from resolved font
		if (span.font != 0)
		{
			span.line_height = span.font->real_size().height_();
			span.baseline = span.font->baseline_();
		}
	}
}
