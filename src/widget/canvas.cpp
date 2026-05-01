#include "berialdraw_imp.hpp"

using namespace berialdraw;

Canvas::Canvas(Widget * parent):
	Widget("canvas",parent, sizeof(Canvas))
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
}

Canvas::~Canvas()
{
	clear();
}

/** Copy all styles of the canvas */
void Canvas::copy(const Canvas & canvas)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&canvas);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&canvas);
}

/** Copy all styles of the canvas */
void Canvas::copy(const Canvas * canvas)
{
	if(canvas)
	{
		copy(*canvas);
	}
}

/** Return the size of content without marges */
Size Canvas::content_size()
{
	if ((m_content_size.is_width_undefined() && m_content_size.is_height_undefined()) || m_geometry_modified)
	{
		m_geometry_modified = 0;
		if ((m_size.is_height_undefined() && m_size.is_width_undefined()))
		{
			Size size;

			// Compute the content size according to all shapes registered
			for(uint32_t i = 0; i < m_shapes.size(); i++)
			{
				if (m_shapes[i])
				{
					Size marged_size = m_shapes[i]->marged_size();
					Point position = m_shapes[i]->position();
					if (marged_size.width_() + position.x_() > size.width_())
					{
						size.width_(marged_size.width_() + position.x_());
					}
					if (marged_size.height_() + position.y_() > size.height_())
					{
						size.height_(marged_size.height_() + position.y_());
					}
				}
			}
			m_content_size = size;
		}
		else
		{
			m_content_size = m_size;
		}
	}
	return m_content_size;
}

void Canvas::place(const Area & area, bool in_layout)
{
	if (!is_absolute())
	{
		in_layout = true;
	}
	place_in_area(area, in_layout);
	Widget::place(area, in_layout);
}

void Canvas::paint(const Region & parent_region)
{
	Region region(parent_region);

	// Clipping canvas
	region.intersect(m_foreclip);

	// If widget visible
	if (region.is_inside(m_foreclip.position(), m_foreclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);

		Exporter * exporter = UIManager::exporter();
		if (exporter)
		{
			// Clipping canvas svg
			exporter->open_group(m_foreclip.position(), m_foreclip.size());
		}

		// Draw background color
		if(m_color)
		{
			Rect rect(0);
			rect.size(m_foreclip.size());
			rect.color(stated_color(m_color));
			rect.paint(m_foreclip.position());
		}

		// Redraw all shapes
		for(uint32_t i = 0; i < m_shapes.size(); i++)
		{
			if (m_shapes[i])
			{
				m_shapes[i]->paints(m_foreclip.position());
			}
		}

		if (exporter)
		{
			// Stop clipping canvas svg
			exporter->close_group();
		}
	}
}

/** Add shape into the canvas (all shapes added will be destroyed when the canvas destroy) */
void Canvas::add(Shape * shape, size_t shape_size)
{
	UIManager::invalidator()->dirty(this, Invalidator::GEOMETRY);
	if(shape)
	{
		bool found = false;

		UIManager::invalidator()->add(this, shape, shape_size);

		m_content_size.clean();
		for(uint32_t i = 0; i < m_shapes.size(); i++)
		{
			if(m_shapes[i] == shape)
			{
				found = true;
				break;
			}
		}
		if(found == false)
		{
			m_shapes.push_back(shape);
		}
		else
		{
			found = true;
		}
	}
}

/** Remove shape from the canvas  */
void Canvas::remove(Shape * shape)
{
	if(shape)
	{
		m_content_size.clean();
		UIManager::invalidator()->remove(shape);
		uint32_t size = m_shapes.size();
		for(uint32_t i = 0; i < size; i++)
		{
			if(m_shapes[i] == shape)
			{
				m_shapes.remove(i);
				break;
			}
		}
	}
}

/** Clear all shape on canvas */
void Canvas::clear()
{
	m_content_size.clean();
	while (m_shapes.size() > 0)
	{
		UIManager::invalidator()->remove(m_shapes[0]);
		delete m_shapes[0];
	}
	m_shapes.clear();
}

/** Get the widget hovered */
Widget * Canvas::hovered(const Region & parent_region, const Point & position)
{
	Region region(parent_region);
	region.intersect(m_foreclip);

	// If the widget hovered
	if(region.is_inside(position) != Region::Overlap::OUT)
	{
		return this;
	}
	return 0;
}

/** Serialize the content of widget into json */
void Canvas::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Canvas::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
}


