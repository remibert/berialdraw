#include "berialdraw_imp.hpp"

using namespace berialdraw;

LineChart::LineChart(Canvas * canvas):Shape(canvas)
{
	m_font_modified = 1;
}

LineChart::~LineChart()
{
	clear();
	delete[] m_data_sets;
}
void LineChart::paint(const Point & shift)
{	
	compute_lines();
}

/** Add data set in the line chart */
void LineChart::add(const DataSet & datas)
{
	if (m_count >= m_max || m_max == 0)
	{
		resize(m_max + 4);
	}
	m_data_sets[m_count++] = new DataSet(datas);
}

/** Add data set in the line chart (the datas should destroyed by the line chart destructor) */
void LineChart::add(DataSet * datas)
{
	if (m_count >= m_max || m_max == 0)
	{
		resize(m_max + 4);
	}
	m_data_sets[m_count++] = datas;
}

/** Clear all data set stored */
void LineChart::clear()
{
	if(m_max)
	{
		for(int i = 0; i < (int)m_count; i++)
		{
			if (m_data_sets[i] != 0)
			{
				delete m_data_sets[i];
				m_data_sets[i] = 0;
			}
		}
		m_count = 0;
	}
}

/** Resize the data set stored */
void LineChart::resize(uint32_t max)
{
	if (max > m_max)
	{
		DataSet ** new_data_set = new DataSet*[max]();
		if (m_data_sets && m_count)
		{
			memcpy(new_data_set, m_data_sets, sizeof(DataSet)*m_count);
			delete[] m_data_sets;
		}
		m_max = max;
		m_data_sets = new_data_set;
	}
}

/** Compute the space of chart in the size reserved */
void LineChart::compute_space(uint32_t & width, int32_t & min, int32_t & max)
{
	width  = 0;
	max = 0;
	min = 0x7FFFFFF;

	for(int ds=0; ds < (int)m_count; ds++)
	{
		if(width < m_data_sets[ds]->count())
		{
			width = m_data_sets[ds]->count();
		}

		int32_t m = m_data_sets[ds]->min();
		if(min > m)
		{
			min = m;
		}
		m = m_data_sets[ds]->max();
		if(max < m)
		{
			max = m;
		}
	}
}

/** Compute the lines */
void LineChart::compute_lines()
{
	uint32_t width;
	int32_t min;
	int32_t max;
	compute_space(width, min, max);

	int32_t delta = max-min;
	Coord x;
	Coord y;

	int32_t data;

	m_lines.clear();
	m_lines.position(this->m_position);
	m_lines.angle(this->angle());

	for(int ds=0; ds < (int)m_count; ds++)
	{
		for(int d=0; d < (int)m_data_sets[ds]->count(); d++)
		{
			data = m_data_sets[ds]->data(d);
			y = data - min;
			y = (y * ((int32_t) m_size.height_()))/(max-min);
			x = (d*(int32_t)(m_size.width_())/(int32_t)(width));
			m_lines.append_(x,y);
		}
		m_lines.color(m_data_sets[ds]->border_color());
		m_lines.thickness(m_data_sets[ds]->thickness());
		Point shift;
		m_lines.paint(shift);
		m_lines.clear();
	}
}

#ifdef _DEBUG

void LineChart::test()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0,0);
		canvas->size(window.size());
		canvas->color(Color::YELLOW,32);

	set_rand(1);
	DataSet data1;
		data1.text("Data set 1");
		for(int i = 0; i < 100; i++)
		{
			data1.add(get_rand(-100,100));
		}
		data1.border_color(DARK_GREEN);
		data1.thickness(1);

	DataSet data2;
		data2.text("Data set 2");
		for(int i = 0; i < 100; i++)
		{
			data2.add(get_rand(-100,100));
		}
		data2.border_color(ROSY_BROWN);
		data2.thickness(1);

	LineChart * chart = new LineChart(canvas);
		chart->size(460,460);
		chart->position(10,10);
		chart->clear();
		chart->text("Line chart");
		chart->add(data1);
		chart->add(data2);

	UIManager::desktop()->dispatch("test/out/linechart.svg");
}
#endif
