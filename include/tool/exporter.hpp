#pragma once
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

		/** Adds an image to the SVG from an original image file (PNG, JPEG).
		@param filename The path to the image file.
		@param x The x position in the SVG (in logical pixels).
		@param y The y position in the SVG (in logical pixels).
		@param display_width The display width in the SVG (in logical pixels).
		@param display_height The display height in the SVG (in logical pixels).
		@param alpha The global alpha (0-255).
		@param fit_mode How the image fits in its area.
		@param src_width Source image width in pixels.
		@param src_height Source image height in pixels.
		@param angle The rotation angle (in Q6 degrees).
		@param center_x The x coordinate of the rotation center (in logical pixels).
		@param center_y The y coordinate of the rotation center (in logical pixels). */
		virtual void add_image_file(const char * filename, int32_t x, int32_t y, uint32_t display_width, uint32_t display_height, uint8_t alpha, ImageFitMode fit_mode = FIT, uint32_t src_width = 0, uint32_t src_height = 0, Coord angle = 0, int32_t center_x = 0, int32_t center_y = 0) = 0;

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
