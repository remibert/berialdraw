#include "berialdraw_imp.hpp"

using namespace berialdraw;

inline void calc_point(Point & p, Dim radius, FT_Vector & sincos)
{
	p.x_(p.x_() + (Coord)((((int64_t)(sincos.x)*(int64_t)(radius)) + (1 << 15))>>16));
	p.y_(p.y_() + (Coord)((((int64_t)(sincos.y)*(int64_t)(radius)) + (1 << 15))>>16));
}

inline void calc_cubic(Point & p, Dim radius, FT_Vector & sincos)
{
	p.x_(p.x_() + (Coord)((((int64_t)(sincos.y)*(int64_t)(radius)) + (1 << 15))>>16));
	p.y_(p.y_() + (Coord)((((int64_t)(sincos.x)*(int64_t)(radius)) + (1 << 15))>>16));
}

Pie::Pie(Canvas * canvas) : 
	Shape(canvas, sizeof(Pie)),
	m_polygon(0)
{
	m_thickness = 0;
}

Pie::Pie(const Pie & other):
	Shape(other),
	RoundStyle(other),
	PieStyle(other),
	m_polygon(0)
{
}

void Pie::get_slice(const Point & center, Dim radius, Coord start_angle, Coord sweep_angle, Point & p1, Point & c1, Point &c2, Point &p2)
{
	FT_Vector sincos;
	Coord d;

	d = ((int64_t)radius * 4521);
	d = (Coord)(((((int64_t)sweep_angle) * d) / (90<<6)) + (1)) >> 13;

	FT_Vector_Unit(&sincos, start_angle<<10);
	p1 = center;
	calc_point(p1, radius, sincos);
	calc_cubic(c1, d, sincos);
	c1.x_(p1.x_() - c1.x_());
	c1.y_(p1.y_() + c1.y_());

	FT_Vector_Unit(&sincos, (sweep_angle + start_angle)<<10);
	p2 = center;
	calc_point(p2, radius, sincos);
	calc_cubic(c2, d, sincos);
	c2.x_(p2.x_() + c2.x_());
	c2.y_(p2.y_() - c2.y_());
}

void Pie::build_filled(Dim radius)
{
	Coord start_angle;
	Coord sweep_angle;
	Coord angle;


	if (m_start_angle < m_end_angle)
	{
		sweep_angle = m_end_angle - m_start_angle;
	}
	else
	{
		sweep_angle = m_start_angle - m_end_angle;
	}

	start_angle = - m_start_angle - sweep_angle;

	while (sweep_angle > 0)
	{
		if (sweep_angle > (90<<6))
		{
			angle = (90<<6);
		}
		else
		{
			angle = sweep_angle;
		}
		Point p1;
		Point c1;
		Point c2;
		Point p2;
		get_slice(m_center, radius, start_angle, angle, p1, c1, c2, p2);
		m_polygon.add_point(p1);
		m_polygon.add_cubic(c1);
		m_polygon.add_cubic(c2);
		m_polygon.add_point(p2);
		start_angle += angle;
		sweep_angle -= angle;
	}
	m_polygon.add_point(m_center);
}

void Pie::build_empty()
{
	Dim radius = m_radius + (m_thickness>>1);
	Coord sweep_angle;
	Coord end_angle;
	Coord angle;

	if (m_start_angle < m_end_angle)
	{
		sweep_angle = m_end_angle - m_start_angle;
	}
	else
	{
		sweep_angle = m_start_angle - m_end_angle;
	}

	end_angle = -m_start_angle;

	while (sweep_angle > 0)
	{
		if (sweep_angle > (90<<6))
		{
			angle = (90<<6);
		}
		else
		{
			angle = sweep_angle;
		}
		Point p1;
		Point c1;
		Point c2;
		Point p2;
		get_slice(m_center, radius, end_angle-angle, angle, p1, c1, c2, p2);
		m_polygon.add_point(p2);
		m_polygon.add_cubic(c2);
		m_polygon.add_cubic(c1);
		m_polygon.add_point(p1);
		end_angle -= angle;
		sweep_angle -= angle;
	}
}

void Pie::paint(const Point & shift)
{
	if (UIManager::invalidator()->is_dirty(m_canvas))
	{
		Coord sweep_angle;
		if (m_end_angle > (360<<6))
		{
			m_end_angle =  m_end_angle % (360<<6);
		}
		m_start_angle =  m_start_angle % (360<<6);

		if (m_start_angle < m_end_angle)
		{
			sweep_angle = m_end_angle - m_start_angle;
		}
		else
		{
			sweep_angle = m_start_angle - m_end_angle;
		}

		m_polygon.clear();
		if (m_thickness == 0)
		{
			build_filled(m_radius);
			m_polygon.add_point(m_center);
		}
		else
		{
			build_empty();
			build_filled(m_radius -(m_thickness>>1));

			if(m_rope)
			{
				Point p1;
				Point c1;
				Point c2;
				Point p2;

				get_slice(m_center, m_radius + (m_thickness >>1), - m_start_angle - sweep_angle, sweep_angle, p1, c1, c2, p2);

				PolyLines lines(0);
				lines.thickness_(m_thickness);
				lines.append(p1);
				lines.append(m_center);
				lines.append(p2);
				lines.build();
				m_polygon.next_contour();
				m_polygon.outline().append(lines.outline());
			}
		}
	}
	UIManager::renderer()->draw(*this, shift);
}

