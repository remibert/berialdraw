#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Add horizontal lines to an outline, skipping lines outside the clip area
bool GridPainter::add_horizontal_lines(Outline& outline, const Area& clip_area,
	Coord x, Dim width, Dim thickness, Dim count,
	const void* context, PositionGetter get_position)
{
	return add_lines(outline, clip_area, true, x, width, thickness, count, context, get_position);
}

// Add vertical lines to an outline, skipping lines outside the clip area
bool GridPainter::add_vertical_lines(Outline& outline, const Area& clip_area,
	Coord y, Dim height, Dim thickness, Dim count,
	const void* context, PositionGetter get_position)
{
	return add_lines(outline, clip_area, false, y, height, thickness, count, context, get_position);
}

// Find visible lines, create their rectangles, and append them to the outline
bool GridPainter::add_lines(Outline& outline, const Area& clip_area, bool horizontal,
	Coord span_start, Dim span_length, Dim thickness, Dim count,
	const void* context, PositionGetter get_position)
{
	bool has_lines = false;
	if (count > 0 && span_length > 0 && thickness > 0 && get_position)
	{
		int64_t clip_start = horizontal ? clip_area.y_q6() : clip_area.x_q6();
		int64_t clip_end = clip_start + (horizontal ? clip_area.height_q6() : clip_area.width_q6());
		int64_t first_visible = clip_start - thickness - 64;
		int64_t last_visible = clip_end + 64;

		// Binary-search the first line that may intersect the visible region.
		Dim first_line = 0;
		Dim end_line = count;
		while (first_line < end_line)
		{
			Dim middle = first_line + (end_line - first_line) / 2;
			if ((int64_t)get_position(context, middle) < first_visible)
			{
				first_line = middle + 1;
			}
			else
			{
				end_line = middle;
			}
		}

		// Rasterize only visible grid lines.
		for (Dim index = first_line; index < count; index++)
		{
			Coord position = get_position(context, index);
			if ((int64_t)position > last_visible)
			{
				break;
			}

			Area line_area = horizontal
				? Area(span_start, position, span_length, thickness, false)
				: Area(position, span_start, thickness, span_length, false);
			if (add_line(outline, clip_area, line_area))
			{
				has_lines = true;
			}
		}
	}
	return has_lines;
}

// Align and clip one grid-line rectangle before adding its outline contour
bool GridPainter::add_line(Outline& outline, const Area& clip_area, Area line_area)
{
	line_area.nearest_pixel();
	line_area.clip(clip_area);
	bool added = false;
	if (line_area.is_not_empty())
	{
		Coord left = line_area.x_q6();
		Coord top = line_area.y_q6();
		Coord right = left + (Coord)line_area.width_q6();
		Coord bottom = top + (Coord)line_area.height_q6();
		outline.add_point(Point(left, top, false));
		outline.add_point(Point(right, top, false));
		outline.add_point(Point(right, bottom, false));
		outline.add_point(Point(left, bottom, false));
		outline.next_contour();
		added = true;
	}
	return added;
}