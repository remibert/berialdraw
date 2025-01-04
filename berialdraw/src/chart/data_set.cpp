#include "berialdraw_imp.hpp"

using namespace berialdraw;

DataSet::DataSet()
{
	m_font_modified = 1;
	m_thickness = 1<<6;
}

DataSet::DataSet(const DataSet & other)
{
	resize(other.m_max);
	memcpy(m_data, other.m_data, sizeof(int32_t) * other.m_count);
	m_count       = other.m_count;
	// m_color       = other.m_color;
	m_border_color = other.m_border_color;
	m_thickness   = other.m_thickness;
	m_font_size    = other.m_font_size;
	m_font_familly = other.m_font_familly;
	text(other.m_text);
}

DataSet::~DataSet()
{
	if (m_data)
	{
		delete[]m_data;
	}
}

void DataSet::clear()
{
	m_count = 0;
}

void DataSet::add(int32_t value)
{
	if (m_count >= m_max || m_max == 0)
	{
		resize(m_max + 16);
	}
	m_data[m_count++] = value;
}

void DataSet::resize(uint32_t max)
{
	if (max > m_max)
	{
		int32_t * new_data = new int32_t[max]();
		if (m_data && m_count)
		{
			memcpy(new_data, m_data, sizeof(int32_t)*m_count);
			delete[] m_data;
		}
		m_max = max;
		m_data = new_data;
	}
}

int32_t DataSet::data(uint32_t index) const
{
	int32_t result = 0x7FFFFFF;
	if (m_data && m_count)
	{
		if (index < m_count)
		{
			result = m_data[index];
		}
	}
	return result;
}

/** Return the max value in data set */
int32_t DataSet::max()
{
	int32_t result = 0;
	for(int i = 0; i < (int)m_count; i++)
	{
		if(result < m_data[i])
		{
			result = m_data[i];
		}
	}
	return result;
}

/** Return the min value in data set */
int32_t DataSet::min()
{
	int32_t result = 0x7FFFFFFF;
	for(int i = 0; i < (int)m_count; i++)
	{
		if(result > m_data[i])
		{
			result = m_data[i];
		}
	}
	return result;
}
