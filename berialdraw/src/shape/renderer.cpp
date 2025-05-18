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

	if (scale == 0)
	{
		m_scale = 1<<6;
	}
	else if (scale <= 15)
	{
		m_scale = scale << 6;
	}
	else
	{
		m_scale = scale;
	}

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

		// Check if the horizontal line is fully within the drawable region
		Region::Overlap draw = m_region->is_inside_scale(x, y, length, m_scale >> 6, m_scale);

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
				draw = m_region->is_inside_scale(x + pos, y, len, m_scale >> 6, m_scale);
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
					draw = m_region->is_inside_scale(x + pos, y, length - pos, m_scale >> 6, m_scale);

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

void Renderer::draw(Coord x_, Coord y_, const uint8_t * buffer, Dim width, Dim height, uint32_t color)
{
	// Get color and alpha
	uint32_t col   = color & 0x00FFFFFF;
	uint32_t alpha = color >> 24;

	Coord x = (x_*m_scale)>>12;
	Coord y = (y_*m_scale)>>12;

	// If the buffer is visible
	if(m_region->is_inside_scale(x, y, width, height, m_scale) != Region::OUT)
	{
		// For each bitmap pixel draw it
		for (uint32_t row = 0; row < height; row++)
		{
			for (uint32_t column = 0; column < width; column++)
			{
				uint8_t coverage = buffer[(row * width) + column];

				// If pixel is visible
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
					draw_line(x+column, y+/*height-*/row, length, coverage, alpha, col);
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
