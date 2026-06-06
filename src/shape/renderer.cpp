#include "berialdraw_imp.hpp"

using namespace berialdraw;

extern "C"
{
	#include "ft_system.c"
}

inline uint32_t compute_alpha(uint8_t a_, uint32_t b_) 
{
	return ((((a_)+1)*(b_))>>8) ;
}

Renderer::~Renderer()
{
	ft_grays_vector_raster.raster_done(m_raster);
	delete m_region;
	delete m_pool;
}

Renderer::Renderer(Dim width, Dim height, Dim scale, uint32_t pool_size)
{
	m_color = 0;
	m_region = new Region;
	m_scale = scale;
	m_size.set(width, height);

	m_pool = new uint8_t[sizeof(gray_TWorker) + pool_size]();
	m_pool_size = sizeof(gray_TWorker) + pool_size;

	memset(&m_params, 0, sizeof(m_params));
	ft_grays_vector_raster.raster_new(&FtSystemAllocator, &m_raster);

	m_params.target = 0;
	m_params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
	m_params.gray_spans = (FT_SpanFunc)draw_vector;
	m_params.user = this;
	m_params.clip_box.xMin = 0;
	m_params.clip_box.xMax = (width*m_scale)>>6;
	m_params.clip_box.yMin = 0;
	m_params.clip_box.yMax = (height*m_scale)>>6;
}

void Renderer::clear()
{
	UIManager::framebuf()->clear();
}

uint32_t Renderer::color()
{
	return m_color;
}

void Renderer::region(Region & region)
{
	*m_region = region;
}

Region & Renderer::region()
{
	return *m_region;
}

void Renderer::draw_line(Coord x, Coord y, uint32_t length, uint32_t coverage, uint32_t alpha, uint32_t color)
{
	Framebuf * framebuf = UIManager::framebuf();

	// Proceed only if there is something to draw
	if (coverage && framebuf && alpha)
	{
		// Combine the base color with the computed alpha value
		uint32_t pixel_color = color | (compute_alpha(alpha, coverage) << 24);
		Dim add = 0;

		if ((x<<6) % m_scale)
		{
			add = m_scale>>6;
		}
		if ((length <<6)%m_scale)
		{
			add += m_scale>>6;
		}

		// Check if the horizontal line is fully within the drawable region
		Region::Overlap draw = m_region->is_inside_scale(x, y, length+add, m_scale >> 6, m_scale);

		if (draw == Region::IN)
		{
			// Draw the entire line in one go
			framebuf->fill_rect(x, y, length, 1, pixel_color);
		}
		else if (draw == Region::PART)
		{
			// The line is partially visible, so we draw it in segments
			uint32_t step = 64;
			uint32_t pos  = 0;
			uint32_t len  = step;

			// Iterate over the line in chunks
			for (pos = 0; pos < length; pos += step)
			{
				// Adjust the length for the last segment if needed
				if ((length - pos) <= step)
				{
					len = (length - pos);
				}

				// Check if the current segment is fully visible
				draw = m_region->is_inside_scale(x + pos, y, len+add, m_scale >> 6, m_scale);
				if (draw == Region::IN)
				{
					// Draw the visible segment
					framebuf->fill_rect(x + pos, y, len, 1, pixel_color);
				}
				else if (draw == Region::PART)
				{
					// Segment is partially visible, draw pixel by pixel
					for (uint32_t i = 0; i < len; i++)
					{
						if (m_region->is_inside_scale((x + i + pos), y, m_scale))
						{
							// Draw individual visible pixel
							framebuf->pixel(x + i + pos, y, pixel_color);
						}
					}
				}
				else
				{
					// Check if the rest of the line is completely outside the drawable region
					draw = m_region->is_inside_scale(x + pos, y, length - pos+add, m_scale >> 6, m_scale);

					// If so, stop drawing
					if (draw == Region::OUT)
					{
						break;
					}
				}
			}
		}
	}
}

/** Fill rectangle */
void Renderer::fillrect(Coord x, Coord y, Dim width, Dim height, uint32_t color)
{
	// Get color and alpha
	uint32_t col   = color & 0x00FFFFFF;
	uint32_t alpha = color >> 24;

	x      = (x      * m_scale) >> 6;
	y      = (y      * m_scale) >> 6;
	width  = (width  * m_scale) >> 6;
	height = (height * m_scale) >> 6;

	// If the buffer is visible
	if(m_region->is_inside_scale(x, y, width, height, m_scale) != Region::OUT)
	{
		// Fill rectangle
		for (uint32_t row = 0; row < height; row++)
		{
			// Fill line
			draw_line(x, y+row, width, 255, alpha, col);
		}
	}
}

void Renderer::paint(int y, int count, const FT_Span* spans)
{
	uint32_t color = m_color & 0x00FFFFFF;
	uint32_t alpha = m_color >> 24;

	for (; count > 0; count--, spans++)
	{
		draw_line(spans->x, y, spans->len, spans->coverage, alpha, color);
	}
}

void Renderer::draw_buffer(Coord x_, Coord y_, const uint8_t * buffer, Dim width, Dim height, uint32_t color)
{
	// Get color and alpha
	uint32_t col   = color & 0x00FFFFFF;
	uint32_t alpha = color >> 24;

	Coord x = (x_ * (Coord)m_scale) >> 12;
	Coord y = (y_ * (Coord)m_scale) >> 12;

	// If the buffer is visible
	if(m_region->is_inside_scale(x, y, width, height, m_scale) != Region::OUT)
	{
		// For each bitmap pixel draw it
		for (uint32_t row = 0; row < height; row++)
		{
			for (uint32_t column = 0; column < width; column++)
			{
				uint8_t coverage = buffer[(row * width) + column];

				// If pixel is not transparent
				if(coverage)
				{
					uint32_t length = 1;
					const uint8_t * buf = &(buffer[(row * width) + column + length]);

					// Count all other pixels with same color in the row
					while ((column + length) < width && *buf == coverage)
					{
						buf++;
						length++;
					}

					// Draw line
					draw_line(x+column, y+row, length, coverage, alpha, col);
					column += (length-1);
				}
			}
		}
	}
}

void Renderer::draw(const Shape & shape, const Point & shift)
{
	// Copy and create polygon
	Polygon copy_poly(shape.polygon());
	draw_outline(shape.position(), shape.margin(), shift, shape.center(), shape.color(), shape.angle_(), copy_poly.outline());
}

/** Apply common outline transformations: translate to center, rotate, translate to position.
This is the core transform logic shared by draw() and transform_rect(). */
void Renderer::apply_outline_transforms(FT_Outline & outline, const Point & position, const Margin & margin, const Point & center, Coord angle)
{
	// Add margins
	Coord dx = margin.left_();
	Coord dy = margin.top_();

	// Position + margin
	Coord x = position.x_() + dx;
	Coord y = position.y_() + dy;

	// Step 1: Move to rotation center
	FT_Outline_Translate(&outline, -(center.x_() + dx), -(center.y_() + dy));

	// Step 2: Apply rotation if needed
	if (angle != 0)
	{
		FT_Matrix rotation = vector_matrix((angle << 10));
		FT_Outline_Transform(&outline, &rotation);
	}

	// Step 3: Translate to final position
	if (x != 0 || y != 0)
	{
		FT_Outline_Translate(&outline, x, y);
	}
}

/** Transform a rectangle using the same FT_Outline logic as draw().
Creates a 4-point outline, applies the same transformations (including scale), then extracts bounding box.
origin_x/origin_y allow placing the rect at an offset in the widget's local coordinate system (for centering). */
void Renderer::transform_rect(Coord origin_x, Coord origin_y, Dim rect_w, Dim rect_h, const Point & position, const Margin & margin, const Point & center, Coord angle, Coord & out_min_x, Coord & out_min_y)
{
	// Create a 4-point outline representing the rectangle (with optional origin offset)
	FT_Vector points[4];
	points[0].x = origin_x;                     points[0].y = origin_y;
	points[1].x = origin_x + (Coord)rect_w;     points[1].y = origin_y;
	points[2].x = origin_x + (Coord)rect_w;     points[2].y = origin_y + (Coord)rect_h;
	points[3].x = origin_x;                     points[3].y = origin_y + (Coord)rect_h;

	unsigned char tags[4] = {FT_CURVE_TAG_ON, FT_CURVE_TAG_ON, FT_CURVE_TAG_ON, FT_CURVE_TAG_ON};
	unsigned short contours[1] = {3};  // Last point index of contour

	FT_Outline outline;
	outline.n_contours = 1;
	outline.n_points = 4;
	outline.points = points;
	outline.tags = tags;
	outline.contours = contours;
	outline.flags = 0;

	// Apply common transformations (steps 1-3)
	apply_outline_transforms(outline, position, margin, center, angle);

	// Step 4: Apply scale (same as draw() after export)
	if (m_scale != 1 << 6)
	{
		FT_Matrix matrix;
		matrix.xx = (FT_Fixed)m_scale << 10;
		matrix.xy = 0;
		matrix.yx = 0;
		matrix.yy = (FT_Fixed)m_scale << 10;
		FT_Outline_Transform(&outline, &matrix);
	}

	// Extract bounding box from transformed points (now in screen coordinates)
	out_min_x = points[0].x >> 6;  // Convert from Q6 to screen pixels
	out_min_y = points[0].y >> 6;
	for (int i = 1; i < 4; i++)
	{
		Coord px = points[i].x >> 6;
		Coord py = points[i].y >> 6;
		if (px < out_min_x) out_min_x = px;
		if (py < out_min_y) out_min_y = py;
	}
}

void Renderer::draw_image(const Point & position, const Size & size, const Point & center, const Margin & margin, Coord angle, ImageItem* item, ImageFitMode fit_mode, uint8_t alpha)
{
	Framebuf * framebuf = UIManager::framebuf();
	
	bool should_draw = false;
	Coord sx = 0, sy = 0;
	uint32_t rot_w = 0, rot_h = 0;
	const uint32_t* pixels = nullptr;

	// Validation phase
	if (framebuf && item && item->is_valid() && alpha > 0 && size.width_() > 0 && size.height_() > 0)
	{
		// Calculate screen dimensions
		uint32_t size_px_w = size.width_() >> 6;
		uint32_t size_px_h = size.height_() >> 6;
		uint32_t scale_factor = m_scale >> 6;
		uint32_t final_w = size_px_w * scale_factor;
		uint32_t final_h = size_px_h * scale_factor;

		if (final_w > 0 && final_h > 0)
		{
			// Compute fitted image dimensions in logical pixels
			uint32_t fit_lpx_w = 0, fit_lpx_h = 0;
			ImageProcessor::compute_fit_size(item->source_width(), item->source_height(),
				size_px_w, size_px_h, fit_mode, fit_lpx_w, fit_lpx_h);

			// Centering offset in Q6 (image within widget area)
			Coord off_x = (Coord)(size.width_()  - (fit_lpx_w << 6)) / 2;
			Coord off_y = (Coord)(size.height_() - (fit_lpx_h << 6)) / 2;

			// Transform fitted image rect with centering offset
			transform_rect(off_x, off_y, (Dim)(fit_lpx_w << 6), (Dim)(fit_lpx_h << 6),
				position, margin, center, angle, sx, sy);

			// Get pre-rotated pixels from cache
			pixels = item->get_pixels(angle, final_w, final_h, fit_mode, rot_w, rot_h);

			// Finalize drawing conditions
			if (pixels && rot_w > 0 && rot_h > 0 && 
				m_region->is_inside_scale(sx, sy, rot_w, rot_h, m_scale) != Region::OUT)
			{
				should_draw = true;
			}
		}
	}

	// Export image to SVG if exporter active
	if (item && UIManager::exporter())
	{
		// Compute area top-left: position is reference point, center is offset to widget origin
		int32_t svg_x = (int32_t)((position.x_() - center.x_()) >> 6);
		int32_t svg_y = (int32_t)((position.y_() - center.y_()) >> 6);

		// Rotation center offset from area top-left (includes margin)
		int32_t svg_cx = (int32_t)((center.x_() + (Coord)margin.left_()) >> 6);
		int32_t svg_cy = (int32_t)((center.y_() + (Coord)margin.top_()) >> 6);

		UIManager::exporter()->add_image_file(
			item->filename().c_str(),
			svg_x,
			svg_y,
			(uint32_t)(size.width_() >> 6),
			(uint32_t)(size.height_() >> 6),
			alpha,
			fit_mode,
			item->source_width(),
			item->source_height(),
			angle,
			svg_cx,
			svg_cy);
	}

	// Single point of exit: perform drawing
	if (should_draw)
	{
		for (uint32_t py = 0; py < rot_h; py++)
		{
			for (uint32_t px = 0; px < rot_w; px++)
			{
				uint32_t pixel = pixels[py * rot_w + px];
				uint8_t pixel_alpha = (uint8_t)((pixel >> 24) & 0xFF);

				// Combine pixel alpha with widget alpha
				uint8_t final_alpha = (uint8_t)(((uint32_t)pixel_alpha * (uint32_t)alpha) / 255);

				if (final_alpha > 0)
				{
					uint32_t col = (pixel & 0x00FFFFFF);
					draw_line(sx + px, sy + py, 1, 255, final_alpha, col);
				}
			}
		}
	}
}

void Renderer::draw_outline(const Point & position, const Margin & margin, const Point & shift, const Point & center, uint32_t color, Coord angle_, Outline & out)
{
	FT_Outline & outline = out.get();
	m_color = color;
	m_params.gray_spans = (FT_SpanFunc)draw_vector;

	// If the polygon must be zoomed
	if (out.zoom_() != 1<< 6)
	{
		FT_Matrix matrix;
		matrix.xx = (FT_Fixed)out.zoom_() << 10;
		matrix.xy = 0;
		matrix.yx = 0;
		matrix.yy = (FT_Fixed)out.zoom_() << 10;
		FT_Outline_Transform(&outline, &matrix);
	}

	// Create position with shift included
	Point pos_with_shift;
	pos_with_shift.x_(position.x_() + shift.x_());
	pos_with_shift.y_(position.y_() + shift.y_());

	// Apply common transformations (steps 1-3)
	apply_outline_transforms(outline, pos_with_shift, margin, center, angle_);

	// If export svg required
	if (UIManager::exporter())
	{
		UIManager::exporter()->add_path(out, m_color);
	}

	// If the scale different than 1 defined (step 4)
	if (m_scale != 1<<6)
	{
		// Resize the polygon according to the render scale
		FT_Matrix matrix;
		matrix.xx = (FT_Fixed)m_scale << 10;
		matrix.xy = 0;
		matrix.yx = 0;
		matrix.yy = (FT_Fixed)m_scale << 10;
		FT_Outline_Transform(&outline, &matrix);
	}

	// Render shape
	m_params.source = &outline;
	ft_grays_vector_raster.raster_reset(m_raster, m_pool, (unsigned long)m_pool_size);
	ft_grays_vector_raster.raster_render(m_raster, &m_params);
}

/** Get the scale factor for screen resolution. A value of 64 corresponds to a scale factor of 1 (1 << 6).
@return The current scale factor. */
Dim Renderer::scale_()
{
	return m_scale;
}

/** Get the scale factor for screen resolution. 
@return The current scale factor. */
Dim Renderer::scale()
{
	return m_scale>>6;
}

/** Return the renderer size 
@return size */
Size Renderer::size() const
{
	return m_size;
}
