#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Create an empty clip mask
ClipMask::ClipMask()
{
	m_bottom_start    = 0;
	m_zone            = TOP_CORNERS;
	m_rect_y_top      = 0;
	m_rect_y_bottom   = 0;
	m_rect_x_left     = 0;
	m_rect_x_right    = 0;
	m_last_top_idx    = 0;
	m_last_bottom_idx = 0;
}

// Destroy the clip mask
ClipMask::~ClipMask()
{
}

// Reset to empty state
void ClipMask::clear()
{
	m_spans.clear();
	m_rows.clear();
	m_bottom_start    = 0;
	m_zone            = TOP_CORNERS;
	m_rect_y_top      = 0;
	m_rect_y_bottom   = 0;
	m_rect_x_left     = 0;
	m_rect_x_right    = 0;
	m_last_top_idx    = 0;
	m_last_bottom_idx = 0;
}

// Add one FreeType span row in streaming top-to-bottom order
void ClipMask::add_span_row(int y, int count, const FT_Span * spans)
{
	bool is_rect_row     = false;
	bool store_as_corner = false;

	// Detect a full-coverage single-span row (rectangle zone candidate)
	if (count == 1 && spans[0].coverage == 255)
	{
		int16_t x_left  = (int16_t)spans[0].x;
		int16_t x_right = (int16_t)(spans[0].x + spans[0].len);

		if (m_zone == TOP_CORNERS)
		{
			// Enter rectangle zone for the first time
			m_rect_x_left   = x_left;
			m_rect_x_right  = x_right;
			m_rect_y_top    = (Coord)y;
			m_rect_y_bottom = (Coord)(y + 1);
			m_zone          = RECT;
			is_rect_row     = true;
		}
		else if (m_zone == RECT && x_left == m_rect_x_left && x_right == m_rect_x_right)
		{
			// Extend rectangle zone downward by one row
			m_rect_y_bottom = (Coord)(y + 1);
			is_rect_row     = true;
		}
		else
		{
			// x bounds changed, or already past the rect zone: corner row
			store_as_corner = true;
		}
	}

	if (!is_rect_row)
	{
		// Exiting rectangle zone: record where bottom corners begin
		if (m_zone == RECT)
		{
			m_bottom_start = (uint16_t)m_rows.size();
			m_zone         = BOTTOM_CORNERS;
		}
		store_as_corner = true;
	}

	// Store this row in the flat span pool
	if (store_as_corner)
	{
		uint32_t offset = (uint32_t)m_spans.size();
		for (int i = 0; i < count; i++)
		{
			ClipSpan s;
			s.x     = (int16_t)spans[i].x;
			s.len   = (uint16_t)spans[i].len;
			s.alpha = spans[i].coverage;
			m_spans.push_back(s);
		}
		ClipRow row;
		row.span_offset = offset;
		row.y           = (Coord)y;
		row.span_count  = (uint8_t)count;
		m_rows.push_back(row);
	}
}

// Finalize: trim storage and fix bottom_start when no bottom corners exist
void ClipMask::finalize()
{
	m_spans.shrink_to_fit();
	m_rows.shrink_to_fit();

	// If build ended inside the rect zone, all rows seen are top corners
	if (m_zone == RECT)
	{
		m_bottom_start = (uint16_t)m_rows.size();
	}
}

// Return true when the mask holds no useful data
bool ClipMask::is_empty() const
{
	return m_rows.empty() && (m_rect_y_top == m_rect_y_bottom);
}

// Scan corner rows [start, end) using a persistent cursor
// Rows are in ascending y order; cursor advances forward and resets only when y regresses
Overlap ClipMask::search_corner_rows(uint16_t start, uint16_t end, uint16_t & cursor, Coord x, Coord y, Dim len) const
{
	Overlap result = Overlap::OUT;

	// Reset cursor if y jumped back (new paint cycle) or cursor drifted out of range
	if (cursor < start || cursor >= end || m_rows[cursor].y > y)
	{
		cursor = start;
	}

	// Scan forward from cursor (sorted ascending, so never need to go back)
	for (uint16_t i = cursor; i < end; i++)
	{
		if (m_rows[i].y > y)
		{
			// Passed the target row without finding it
			break;
		}
		if (m_rows[i].y == y)
		{
			// Update cursor so next call starts here
			cursor = i;
			result = check_row_spans(m_rows[i].span_offset, m_rows[i].span_count, x, len);
			break;
		}
	}

	return result;
}

// Check overlap of (x, len) against one corner row's span list
Overlap ClipMask::check_row_spans(uint32_t span_offset, uint8_t span_count, Coord x, Dim len) const
{
	Overlap result  = Overlap::OUT;
	Coord   x_end   = x + (Coord)len;
	bool    any_in  = false;
	bool    any_out = false;

	for (uint8_t j = 0; j < span_count; j++)
	{
		const ClipSpan & sp      = m_spans[span_offset + j];
		Coord            sp_left  = (Coord)sp.x;
		Coord            sp_right = (Coord)(sp.x + sp.len);

		// Overlap between [x, x_end) and [sp_left, sp_right)
		if (x < sp_right && x_end > sp_left)
		{
			any_in = true;
			if (x < sp_left || x_end > sp_right)
			{
				any_out = true;
			}
		}
	}

	if (any_in)
	{
		result = any_out ? Overlap::PART : Overlap::IN;
	}
	return result;
}

// Test whether a horizontal run overlaps the clip mask
Overlap ClipMask::is_inside(Coord x, Coord y, Dim len) const
{
	Overlap result = Overlap::OUT;

	// Fast path: rectangle zone (no allocation, constant-time)
	if (m_rect_y_top < m_rect_y_bottom && y >= m_rect_y_top && y < m_rect_y_bottom)
	{
		Coord x_end = x + (Coord)len;
		if (x >= (Coord)m_rect_x_right || x_end <= (Coord)m_rect_x_left)
		{
			result = Overlap::OUT;
		}
		else if (x >= (Coord)m_rect_x_left && x_end <= (Coord)m_rect_x_right)
		{
			result = Overlap::IN;
		}
		else
		{
			result = Overlap::PART;
		}
	}
	else
	{
		// Determine which corner zone to search and pick the right cursor
		if (m_rect_y_top < m_rect_y_bottom)
		{
			if (y < m_rect_y_top)
			{
				// Top corner zone
				result = search_corner_rows(0, m_bottom_start, m_last_top_idx, x, y, len);
			}
			else
			{
				// Bottom corner zone (y >= m_rect_y_bottom)
				result = search_corner_rows(m_bottom_start, (uint16_t)m_rows.size(), m_last_bottom_idx, x, y, len);
			}
		}
		else
		{
			// No rect zone: search all rows with the top cursor
			result = search_corner_rows(0, (uint16_t)m_rows.size(), m_last_top_idx, x, y, len);
		}
	}

	return result;
}

#ifdef _DEBUG
// Basic unit test for ClipMask streaming build and is_inside
void ClipMask::test1()
{
}

void ClipMask::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		test1();
	}
}
#endif
