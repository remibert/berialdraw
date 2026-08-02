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

