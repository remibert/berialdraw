#pragma once
namespace berialdraw
{
	/** Shared helper for adding clipped grid-line rectangles to a vector outline. */
	class GridPainter
	{
	public:
		/** Get a grid-line start coordinate in Q6 for the specified index. */
		typedef Coord (*PositionGetter)(const void* context, Dim index);

		/** Add horizontal lines to an outline, skipping lines outside the clip area. */
		static bool add_horizontal_lines(Outline& outline, const Area& clip_area,
			Coord x, Dim width, Dim thickness, Dim count,
			const void* context, PositionGetter get_position);

		/** Add vertical lines to an outline, skipping lines outside the clip area. */
		static bool add_vertical_lines(Outline& outline, const Area& clip_area,
			Coord y, Dim height, Dim thickness, Dim count,
			const void* context, PositionGetter get_position);

	protected:
		static bool add_lines(Outline& outline, const Area& clip_area, bool horizontal,
			Coord span_start, Dim span_length, Dim thickness, Dim count,
			const void* context, PositionGetter get_position);

		static bool add_line(Outline& outline, const Area& clip_area, Area line_area);
	};
}