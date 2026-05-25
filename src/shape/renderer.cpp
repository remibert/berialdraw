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
	draw(shape.position(), shape.margin(), shift, shape.center(), shape.color(), shape.angle_(), copy_poly.outline());
}

void Renderer::draw_image(const Point & position, const Size & size, const Point & center, const Margin & margin, Coord angle, const uint32_t * pixels, uint32_t width, uint32_t height, uint8_t alpha)
{
	Framebuf * framebuf = UIManager::framebuf();
	if (pixels == nullptr || width == 0 || height == 0 || alpha == 0 || framebuf == nullptr)
	{
		return;
	}

	// Normalize angle to 0, 90, 180, 270
	int32_t angle_deg = 0;
	if (angle != 0)
	{
		angle_deg = (int32_t)(angle % 360);
		if (angle_deg < 0)
		{
			angle_deg += 360;
		}
	}

	// Determine rotated image dimensions
	uint32_t dst_width  = width;
	uint32_t dst_height = height;
	if (angle_deg == 90 || angle_deg == 270)
	{
		dst_width  = height;
		dst_height = width;
	}

	// Compute position with margins
	FT_Matrix matrix = vector_matrix((angle << 10));
	FT_Vector vec;

	Coord xx = position.x_() + margin.left_();
	Coord yy = position.y_() + margin.top_();

	// Apply center of rotation
	if (center.x_() != 0 || center.y_() != 0)
	{
		vec.x = -center.x_();
		vec.y = -center.y_();
		FT_Vector_Transform(&vec, &matrix);
		xx += vec.x;
		yy += vec.y;
	}

	// Apply scale
	Coord sx = (xx * (Coord)m_scale) >> 12;
	Coord sy = (yy * (Coord)m_scale) >> 12;

	// Check visibility
	if (m_region->is_inside_scale(sx, sy, dst_width, dst_height, m_scale) == Region::OUT)
	{
		return;
	}

	// Draw pixels 1:1 with rotation (image is expected at screen resolution)
	for (uint32_t py = 0; py < dst_height; py++)
	{
		for (uint32_t px = 0; px < dst_width; px++)
		{
			// Map destination (px, py) to source pixel based on rotation
			uint32_t src_x;
			uint32_t src_y;

			switch (angle_deg)
			{
			default:
			case 0:
				src_x = px;
				src_y = py;
				break;
			case 90:
				src_x = py;
				src_y = (dst_width - 1) - px;
				break;
			case 180:
				src_x = (dst_width  - 1) - px;
				src_y = (dst_height - 1) - py;
				break;
			case 270:
				src_x = (dst_height - 1) - py;
				src_y = px;
				break;
			}

			uint32_t pixel = pixels[src_y * width + src_x];
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

void Renderer::draw(const Point & position, const Margin & margin, const Point & shift, const Point & center, uint32_t color, Coord angle_, Outline & out)
{
	FT_Outline & outline = out.get();
	m_color = color;
	m_params.gray_spans = (FT_SpanFunc)draw_vector;

	// Add margins
	Coord dx = margin.left_();
	Coord dy = margin.top_();

	// Move to position + margin + shift
	Coord x  = position.x_() + dx + shift.x_();
	Coord y  = position.y_() + dy + shift.y_();

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

	// If no rotation
	if(angle_ == 0)
	{
		// Move to shape to take into account the rotation center
		FT_Outline_Translate(&outline, -(center.x_()+dx), -(center.y_()+dy));
	}
	else
	{
		FT_Matrix rotation = vector_matrix((angle_ << 10));

		// Move to shape to take into account the rotation center
		FT_Outline_Translate(&outline, -(center.x_()+dx), -(center.y_()+dy));

		// Rotation
		FT_Outline_Transform (&outline, &rotation);
	}

	// If shape must be moved
	if(x != 0 || y != 0)
	{
		FT_Outline_Translate(&outline, x, y);
	}

	// If export svg required
	if (UIManager::exporter())
	{
		UIManager::exporter()->add_path(out, m_color);
	}

	// If the scale different than 1 defined
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
