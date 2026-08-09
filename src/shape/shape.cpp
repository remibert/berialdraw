#include "berialdraw_imp.hpp"

using namespace berialdraw;

Shape::Shape(Canvas * canvas, size_t shape_size):
	m_canvas(canvas)
{
	m_color = Color::SHAPE_COLOR;
	
	if(canvas)
	{
		UIManager::invalidator()->dirty(m_canvas, Invalidator::REDRAW);
		canvas->add(this, shape_size);
	}
}

/** Constructor
@param widget Pointer to the widget object
@widget_size size of shape instance (required by invalidator) */
Shape::Shape(Widget * widget, size_t widget_size) :
	m_widget(widget)
{
	m_color = Color::SHAPE_COLOR;
	
	if(widget)
	{
		UIManager::invalidator()->dirty(m_widget, Invalidator::REDRAW);
	}
}


Shape::~Shape()
{
	if(m_canvas)
	{
		UIManager::invalidator()->dirty(m_canvas, Invalidator::REDRAW);
		m_canvas->remove(this);
	}
	if(m_widget)
	{
		UIManager::invalidator()->dirty(m_widget, Invalidator::REDRAW);
	}
}

Shape::Shape(const Shape & other)
{
	*((CommonStyle*)this) = *(const CommonStyle*)&other;

	m_canvas = other.m_canvas;
	m_widget = other.m_widget;
	m_repetition = other.m_repetition;
	m_start = other.m_start;
	m_end = other.m_end;
	m_step = other.m_step;

	if(m_canvas)
	{
		UIManager::invalidator()->dirty(m_canvas, Invalidator::GEOMETRY);
		m_canvas->add(this, sizeof(Shape));
	}
	if(m_widget)
	{
		UIManager::invalidator()->dirty(m_widget, Invalidator::GEOMETRY);
	}
}

/** repeats the display of the shape according to the selected information. This makes it easy to display graduations for example */
void Shape::repeat(Repetition type, int start, int end, int step)
{
	m_repetition = type;
	m_start = start << 6;
	m_end = end << 6;
	m_step = step << 6;
}

/** repeats the display of the shape according to the selected information. This makes it easy to display graduations for example */
void Shape::repeat_q6(Repetition type, int start, int end, int step)
{
	m_repetition = type;
	m_start = start;
	m_end = end;
	m_step = step;
}

Size Shape::content_size()
{
	Size result(0,0);
	if (m_size.is_width_undefined() == false)
	{
		result.width_q6(m_size.width_q6());
	}
	if (m_size.is_height_undefined() == false)
	{
		result.height_q6(m_size.height_q6());
	}
	return result;
}

Size Shape::marged_size()
{
	Size result = content_size();
	result.increase_q6(m_margin.left_q6() + m_margin.right_q6(), m_margin.bottom_q6() + m_margin.top_q6());
	return result;
}

Area Shape::bounding_area()
{
	// Get dimensions using marged_size which accounts for margin and border
	Size marged = marged_size();
	Coord w = marged.width_q6();
	Coord h = marged.height_q6();
	Coord dx = m_margin.left_q6();
	Coord dy = m_margin.top_q6();

	// Create 4 corner points
	FT_Vector points[4];

	if (dynamic_cast<Marker*>(this))
	{
		// For centered shapes (like Circle/Marker), corners extend ±(w/2, h/2) from center
		Coord half_w = w >> 1;
		Coord half_h = h >> 1;
		points[0].x = dx - half_w;         points[0].y = dy - half_h;
		points[1].x = dx + half_w;         points[1].y = dy - half_h;
		points[2].x = dx + half_w;         points[2].y = dy + half_h;
		points[3].x = dx - half_w;         points[3].y = dy + half_h;
	}
	else
	{
		// For non-centered shapes, corners are positioned normally
		points[1].x = dx + w;         points[1].y = dy - h;
		points[0].x = dx - w;         points[0].y = dy - h;
		points[2].x = dx + w;         points[2].y = dy + h;
		points[3].x = dx - w;         points[3].y = dy + h;
	}

	// Step 1: Translate to rotation center origin
	Coord cx = m_center.x_q6() + dx;
	Coord cy = m_center.y_q6() + dy;
	if (cx != 0 || cy != 0)
	{
		for (int i = 0; i < 4; i++)
		{
			points[i].x -= cx;
			points[i].y -= cy;
		}
	}

	// Step 2: Apply rotation if needed (FreeType angle units: <<10)
	if (m_angle != 0)
	{
		FT_Matrix rotation = vector_matrix(m_angle << 10);
		for (int i = 0; i < 4; i++)
		{
			FT_Vector_Transform(&points[i], &rotation);
		}
	}

	// Step 3: Translate to final position
	Coord px = m_position.x_q6();
	Coord py = m_position.y_q6();

	if (px != 0 || py != 0)
	{
		for (int i = 0; i < 4; i++)
		{
			points[i].x += px + cx;
			points[i].y += py + cy;
		}
	}
	
	// Extract bounding box from transformed points, clamp negatives to 0
	Coord min_x = points[0].x;
	Coord min_y = points[0].y;
	Coord max_x = points[0].x;
	Coord max_y = points[0].y;

	for (int i = 1; i < 4; i++)
	{
		if (points[i].x < min_x) min_x = points[i].x;
		if (points[i].y < min_y) min_y = points[i].y;
		if (points[i].x > max_x) max_x = points[i].x;
		if (points[i].y > max_y) max_y = points[i].y;
	}

	// Clamp negative values to 0 and convert to Size
	if (min_x < 0) min_x = 0;
	if (min_y < 0) min_y = 0;

	Area result;
	result.x_q6(min_x);
	result.y_q6(min_y);
	result.width_q6((Dim)(max_x - min_x));
	result.height_q6((Dim)(max_y - min_y));
	return result;
}

void Shape::paints(const Point & shift)
{
	if(m_repetition == Shape::REPEAT_NONE || m_start == m_end)
	{
		paint(shift);
	}
	else if(m_repetition == Shape::REPEAT_ANGLE)
	{
		if(m_start < m_end)
		{
			for(Coord a = m_start; a <= m_end; a += m_step)
			{
				Shape::angle_q6(a - (90 <<6));
				paint(shift);
			}
		}
		else
		{
			for(Coord a = m_start; a >= m_end; a -= m_step)
			{
				Shape::angle_q6(a - (90 <<6));
				paint(shift);
			}
		}
	}
	else if(m_repetition == Shape::REPEAT_POSITION_X)
	{
		if(m_start < m_end)
		{
			for(int x = m_start; x < m_end; x += abs(m_step))
			{
				m_position.set_q6(m_position.x_q6()+x, m_position.y_q6());
				paint(shift);
			}
		}
		else
		{
			for(int x = m_start; x >= m_end; x -= abs(m_step))
			{
				m_position.set_q6(m_position.x_q6()+x, m_position.y_q6());
				paint(shift);
			}
		}
	}
	else if(m_repetition == Shape::REPEAT_POSITION_Y)
	{
		if(m_start < m_end)
		{
			for(int y = m_start; y < m_end; y += m_step)
			{
				m_position.set_q6(m_position.x_q6(), m_position.y_q6()+y);
				paint(shift);
			}
		}
		else
		{
			for(int y = m_start; y >= m_end; y -= m_step)
			{
				m_position.set_q6(m_position.x_q6(), m_position.y_q6()+y);
				paint(shift);
			}
		}
	}
}

