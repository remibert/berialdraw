#pragma once
namespace berialdraw
{
	class ClipMask;

	/** RectRenderer provides static methods for drawing rectangles.
	This is a utility class that separates rectangle drawing logic from the Rect shape class,
	allowing widgets and other components to draw rectangles independently. */
	class RectRenderer
	{
	public:
		/** Paint a simple filled rectangle.
		@param area Bounding area of the rectangle
		@param color Fill color */
		static void paint_rect(const Area& area, uint32_t color);

		/** Paint a rounded rectangle with optional border.
		@param area Bounding area of the rectangle
		@param radius Radius for rounded corners (Q6)
		@param color Fill color */
		static void paint_round_rect(const Area& area, Dim radius_q6, uint32_t color);

		/** Paint a rectangle with border and optional rounded corners.
		@param area Bounding area of the rectangle
		@param radius Radius for rounded corners (Q6)
		@param thickness Thickness of the border (Q6)
		@param gap Gap between border and fill (Q6)
		@param borders Which borders to draw
		@param backcolor Background color
		@param bordercolor Border color
		@param focus_thickness Optional focus thickness */
		static void paint_border_round_rect(const Area& area, Dim radius_q6,
			Dim thickness_q6, Dim gap_q6, uint8_t borders, uint32_t backcolor, 
			uint32_t bordercolor, Dim focus_thickness=0);

		/** Paint a focus indicator around a rectangle.
		@param area Bounding area
		@param borders Which borders to draw
		@param radius_q6 Radius in Q6
		@param thickness_q6 Border thickness in Q6
		@param focus_thickness Focus indicator thickness
		@param focus_gap_q6 Gap between element and focus in Q6
		@param focus_border_color Focus color */
		static void paint_focus(const Area& area, Borders borders,
			Dim radius_q6, Dim thickness_q6, Dim focus_thickness,
			Dim focus_gap_q6, uint32_t focus_border_color);

		/** Paint a focused rounded rectangle (background + border + focus).
		@param area Bounding area
		@param borders Which borders to draw
		@param color Fill color
		@param border_color Border color
		@param radius_q6 Corner radius in Q6
		@param thickness_q6 Border thickness in Q6
		@param focus_thickness Focus indicator thickness
		@param focus_gap_q6 Gap between element and focus in Q6
		@param focus_border_color Focus indicator color
		@param focused Whether to show focus indicator */
		static void paint_focused_round_rect(const Area& area,
			Borders borders,
			uint32_t color,
			uint32_t border_color,
			Dim radius_q6,
			Dim thickness_q6,
			Dim focus_thickness,
			Dim focus_gap_q6,
			uint32_t focus_border_color,
			bool focused);

		/** Build a clip mask from the inner area of a rounded rectangle.
		@param area Bounding area of the rounded rect
		@param radius Corner radius in Q6
		@param thickness Border thickness in Q6
		@param gap Gap between border and fill in Q6
		@param borders Which borders are active
		@param mask Output clip mask (cleared and filled by this call) */
		static void build_clip_mask_rounded_rect(const Area& area, Dim radius_q6, 
			Dim thickness_q6, Dim gap_q6, uint8_t borders, ClipMask& mask);

		// Geometry building helpers (used by Rect and other shape classes)
		static void create_part(Polygon& polygon, const Size& size, Coord radius, Coord thickness, uint8_t borders);
		static void create_part_outer(Polygon& polygon, const Size& size, Coord radius, Coord thickness, uint8_t borders);
		
		static void adapt_radius(Coord& radius, const Size& size, uint8_t borders);
		static void adapt_thickness(Coord& thickness, const Size& size);

	private:
		static void add_corner(Polygon& polygon, Coord x, Coord y, Coord radius, Dim thickness, uint32_t flags);
		
		static void no_border_rectangle(Polygon& polygon, Coord w, Coord h, Coord R, Coord t, uint8_t borders);
		static void rounded_border_rectangle(Polygon& polygon, Coord w, Coord h, Coord R, Coord r, Coord t, uint8_t borders);
		static void right_angle_border_rectangle(Polygon& polygon, Coord w, Coord h, Coord R, Coord r, Coord t, uint8_t borders);
		
		static void no_border_rectangle_outer(Polygon& polygon, Coord W, Coord H, Coord R, uint8_t borders);
		static void rounded_border_rectangle_outer(Polygon& polygon, Coord W, Coord H, Coord R, Coord r, Coord thickness, uint8_t borders);
		static void right_angle_border_rectangle_outer(Polygon& polygon, Coord W, Coord H, Coord R, Coord r, Coord thickness, uint8_t borders);
};

}	// namespace berialdraw
