#pragma once
namespace berialdraw
{
	/** Single span within a clip mask row.
	Stores the x start, length, and FreeType anti-alias coverage (0–255). */
	struct ClipSpan
	{
		int16_t  x;      ///< X start in renderer scale-space pixels
		uint16_t len;    ///< Span length in renderer scale-space pixels
		uint8_t  alpha;  ///< FreeType coverage (0=transparent, 255=opaque)
	};

	/** Descriptor for one corner row stored in the clip mask */
	struct ClipRow
	{
		uint32_t span_offset;  ///< Index into ClipMask::m_spans flat pool
		Coord    y;            ///< Absolute Y in renderer scale-space
		uint8_t  span_count;   ///< Number of spans for this row
	};

	/** Provides per-pixel clipping from any FreeType-rasterized vector shape.
	Built in streaming fashion from the span callbacks delivered top-to-bottom
	by the FreeType rasterizer.
	
	The mask is split into three zones to minimise memory:
	- Top corner rows: stored per line with full span data (anti-aliased edges).
	- Rectangle zone: compressed to just x/y bounds (most common, zero allocation).
	- Bottom corner rows: same as top. */
	class ClipMask
	{
	public:
		/** Create an empty clip mask */
		ClipMask();

		/** Destroy the clip mask */
		~ClipMask();

		/** Reset to empty state */
		void clear();

		/** Add one FreeType span row to the mask.
		Must be called in strictly top-to-bottom order (as FreeType delivers spans).
		@param y Y coordinate in renderer scale-space
		@param count Number of spans on this row
		@param spans FreeType span array */
		void add_span_row(int y, int count, const FT_Span * spans);

		/** Finalize the mask after all rows have been added.
		Trims Vector storage to the exact allocated size. */
		void finalize();

		/** Test whether a horizontal run (x, y, len) in renderer scale-space
		overlaps the clip mask.
		@param x  X start coordinate
		@param y  Y coordinate
		@param len Run length in scale-space pixels
		@return IN, PART, or OUT */
		Overlap is_inside(Coord x, Coord y, Dim len) const;

		/** Return true if the mask contains no data */
		bool is_empty() const;

#ifdef _DEBUG
		static void test();
		static void test1();
#endif

	private:
/// @cond DOXYGEN_IGNORE
		/** Streaming build zone */
		enum Zone : uint8_t { TOP_CORNERS, RECT, BOTTOM_CORNERS };

		Vector<ClipSpan> m_spans;         ///< Flat span pool for all corner rows
		Vector<ClipRow>  m_rows;          ///< All corner rows (top then bottom)
		uint16_t         m_bottom_start;  ///< Index in m_rows where bottom corners start
		Zone             m_zone;          ///< Current streaming zone state

		Coord   m_rect_y_top;             ///< Y start of rectangle zone (inclusive)
		Coord   m_rect_y_bottom;          ///< Y end   of rectangle zone (exclusive)
		int16_t m_rect_x_left;            ///< X left  bound of rectangle zone
		int16_t m_rect_x_right;           ///< X right bound of rectangle zone

		/** Check overlap of (x, len) against a single corner row's spans */
		Overlap check_row_spans(uint32_t span_offset, uint8_t span_count, Coord x, Dim len) const;

		/** Scan corner rows [start, end) forward from cursor.
		Rows are in ascending y order so the cursor only moves forward;
		it resets to start only when y jumps back (new paint cycle). */
		Overlap search_corner_rows(uint16_t start, uint16_t end, uint16_t & cursor, Coord x, Coord y, Dim len) const;

		mutable uint16_t m_last_top_idx;    ///< Scan cursor for top corner rows
		mutable uint16_t m_last_bottom_idx; ///< Scan cursor for bottom corner rows
/// @endcond
	};
}
