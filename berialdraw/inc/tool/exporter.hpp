#ifndef Exporter_hpp_INCLUDED
#define Exporter_hpp_INCLUDED

namespace berialdraw
{
	/** Interface used to export vector graphics. */
	class Exporter
	{
	public:
		/** Destructor. */
		virtual ~Exporter(){}

		/** Sets the size of the SVG.
		@param size The size of the SVG. */
		virtual void size(const Size & size) = 0;

		/** Adds a path to the SVG.
		@param outline The path outline to add.
		@param color The color of the path. */
		virtual void add_path(Outline & outline, uint32_t color) = 0;

		/** Adds text to the SVG.
		@param position The position of the text.
		@param size The size of the text bounding box.
		@param center The center point of the text.
		@param margin The margin around the text.
		@param angle The rotation angle of the text.
		@param text The content of the text.
		@param font The font used for the text.
		@param color The color of the text. */
		virtual void add_text(const Point & position, const Size & size, const Point & center, const Margin & margin, Coord angle, const String & text, const Font & font, uint32_t color) = 0;

		/** Opens a clipping and grouping section.
		@param position The position of the group.
		@param size The size of the group. */
		virtual void open_group(const Point & position, const Size & size) = 0;

		/** Closes the current clipping and grouping section. */
		virtual void close_group() = 0;

		/** Closes the SVG export.
		@param crc32 checksum of the rendered bitmap image, this is useful for automated testing. */
		virtual void close(uint32_t crc32) = 0;
	};
}

#endif