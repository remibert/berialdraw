#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Create widget cells (used to compute the cells sizes)*/
Cells::Cells(Widget * widget)
{
	rebound(widget);
}

Cells::~Cells()
{
	reallocate(0,0);
}

/** Create or recreate cells array 
@param rows_count count of rows
@param columns_count count of columns */
void Cells::reallocate(Dim rows_count, Dim columns_count)
{
	if(rows_count != m_rows_count || columns_count != m_columns_count)
	{
		if (m_widths)
		{
			delete m_widths;
		}
		if(m_heights)
		{
			delete m_heights;
		}
		if(m_row_positions)
		{
			delete m_row_positions;
		}
		if(m_col_positions)
		{
			delete m_col_positions;
		}
		m_widths = 0;
		m_heights = 0;
		m_row_positions = 0;
		m_col_positions = 0;

		if(rows_count > 0 && columns_count > 0)
		{
			m_widths       = new Cell[columns_count]();
			m_heights      = new Cell[rows_count]();
			m_row_positions = new Dim[rows_count]();
			m_col_positions = new Dim[columns_count]();
			m_rows_count    = rows_count;
			m_columns_count = columns_count;
		}
	}
	else
	{
		if(m_widths == 0)
		{
			m_widths       = new Cell[columns_count]();
			m_rows_count    = rows_count;
		}
		if(m_heights == 0)
		{
			m_heights      = new Cell[rows_count]();
			m_columns_count = columns_count;
		}
		
		for (Dim row = 0; row < rows_count; row ++)
		{
			m_heights[row].m_marged     = 0;
			m_heights[row].m_maxi       = Size::MAX_SIZE;
			m_heights[row].m_mini       = 0;
			m_heights[row].m_placed     = 0;
			m_heights[row].m_size_policy = SizePolicy::NORMAL_SIZE;
		}
		for (Dim column = 0; column < columns_count; column++)
		{
			m_widths[column].m_marged     = 0;
			m_widths[column].m_maxi       = Size::MAX_SIZE;
			m_widths[column].m_mini       = 0;
			m_widths[column].m_placed     = 0;
			m_widths[column].m_size_policy = SizePolicy::NORMAL_SIZE;
		}
	}
}

/** Select the first widget according to its row and column
@param widget pointer on widget
@param row the row id
@param column the column id 
@return the widget handle found or null */
Widget * Cells::first(Widget * widget, Dim row, Dim column)
{
	Widget * result = 0;
	if (widget)
	{
		Widget * current = widget->m_children;
		
		while (current)
		{
			if (current->row() == row && current->column() == column)
			{
				result = current;
				break;
			}
			current = current->m_next;
		}
	}
	return result;
}

/** Select the next widget according to its row and column
@param widget pointer on widget
@param row the row id
@param column the column id 
@return the widget handle found or null */
Widget * Cells::next(Widget * widget, Dim row, Dim column)
{
	if (widget)
	{
		Widget * current = widget;
		do
		{
			current = current->m_next;
			if (current && current->row() == row && current->column() == column)
			{
				return current;
			}
		}
		while(current);
	}
	return 0;
}

/** Recompute the min and max size of rows and columns of the widget */
Size Cells::calc_sizes(Widget * widget)
{
	if (widget)
	{
		Size min_size;
		Size max_size;
		Size marged_size;
		Dim row, column;
		Widget * current;

		// Iterate through all widgets directly (single traversal)
		current = widget->m_children;
		while(current)
		{
			row = current->row();
			column = current->column();

			// Only process if widget is in a valid cell
			if (row < m_rows_count && column < m_columns_count)
			{
				min_size = current->min_size();
				max_size = current->max_size();

				if (current->size_policy() != SizePolicy::NORMAL_SIZE)
				{
					if (m_widths [column].m_size_policy == SizePolicy::NORMAL_SIZE)
					{
						switch(current->size_policy())
						{
						case SizePolicy::ENLARGE_WIDTH :
						case SizePolicy::ENLARGE_ALL   :
							m_widths [column].m_size_policy = SizePolicy::ENLARGE_ALL;
							break;
						case SizePolicy::SHRINK_WIDTH  :
						case SizePolicy::SHRINK_ALL    :
							m_widths [column].m_size_policy = SizePolicy::SHRINK_ALL;
							break;
						default:
							break;
						}
					}
					if (m_heights [row].m_size_policy == SizePolicy::NORMAL_SIZE)
					{
						switch(current->size_policy())
						{
						case SizePolicy::ENLARGE_HEIGHT:
						case SizePolicy::ENLARGE_ALL   :
							m_heights [row].m_size_policy = SizePolicy::ENLARGE_ALL;
							break;
						case SizePolicy::SHRINK_HEIGHT :
						case SizePolicy::SHRINK_ALL    :
							m_heights [row].m_size_policy = SizePolicy::SHRINK_ALL;
							break;
						default:
							break;
						}
					}
				}

				// If the min size is greater than the max size
				if(min_size.width_() > max_size.width_())
				{
					max_size.width_(min_size.width_());
				}
				if(min_size.height_() > max_size.height_())
				{
					max_size.height_(min_size.height_());
				}
	
				marged_size = current->marged_size();

			// If the mininal size is smaller than content size, set to the content size
				if (min_size.width_() < marged_size.width_())
				{
					min_size.width_(marged_size.width_());
				}
				if (min_size.height_() < marged_size.height_())
				{
					min_size.height_(marged_size.height_());
				}

				// If the maximal size is smaller than content size, set to the content size
				if (max_size.width_() < marged_size.width_())
				{
					max_size.width_(marged_size.width_());
				}
				if (max_size.height_() < marged_size.height_())
				{
					max_size.height_(marged_size.height_());
				}

				// Keep the maximal of content width
				if(marged_size.width_() > m_widths[column].m_marged)
				{
					m_widths[column].m_marged = marged_size.width_();
				}

				// Keep the maximal of content height
				if (marged_size.height_() > m_heights[row].m_marged)
				{
					m_heights[row].m_marged = marged_size.height_();
				}

				// Keep the maximal of minimal width
				if (min_size.width_() > m_widths[column].m_mini)
				{
					m_widths [column].m_mini = min_size.width_();
				}

				// Keep the maximal of minimal height
				if (min_size.height_() > m_heights[row].m_mini)
				{
					m_heights[row].m_mini = min_size.height_();
				}

				// Keep the minimal of maximal width
				if (max_size.width_() < m_widths[column].m_maxi)
				{
					m_widths [column].m_maxi = max_size.width_();
				}

				// Keep the minimal of maximal height
				if (max_size.height_() < m_heights[row].m_maxi)
				{
					m_heights[row].m_maxi = max_size.height_();
				}
			}
			current = current->m_next;
		}

		// Compute all row sizes
		m_min_size.height_(0);
		m_max_size.height_(0);
		m_marged_size.height_(0);
		for (row = 0; row < m_rows_count; row ++)
		{
			m_min_size.increase_   (0, m_heights[row].m_mini);
			m_max_size.increase_   (0, m_heights[row].m_maxi);
			m_marged_size.increase_(0, m_heights[row].m_marged);
		}

		// Compute all column sizes
		m_min_size.width_(0);
		m_max_size.width_(0);
		m_marged_size.width_(0);
		for (column = 0; column < m_columns_count; column++)
		{
			m_min_size.increase_   (m_widths[column].m_mini  , 0);
			m_max_size.increase_   (m_widths[column].m_maxi  , 0);
			m_marged_size.increase_(m_widths[column].m_marged, 0);
		}
	}

	Size result = m_marged_size;

	if (m_min_size.width_() > m_marged_size.width_())
	{
		result.width_(m_min_size.width_());
	}

	if (m_min_size.height_() > m_marged_size.height_())
	{
		result.height_(m_min_size.height_());
	}

	return result;
}

Dim Cells::reduce_mini(Cell * cells, Dim count, Dim missing, Dim available)
{
	Dim minsize;
	Dim placed = 0;

	for (Dim i = 0; i < count; i++)
	{
		minsize = cells[i].m_mini;

		// If not enough space
		if(cells[i].m_placed > minsize)
		{
			uint64_t sub = ((((uint64_t)cells[i].m_placed - (uint64_t)minsize) * (uint64_t)missing) <<7)/(uint64_t)available;
			sub = (sub + 64)>>7;
			cells[i].m_placed -= (Dim)sub;
		}
		else
		{
			cells[i].m_placed = minsize;
		}

		placed += cells[i].m_placed;
	}
	return placed;
}


Dim Cells::reduce_marged(Cell * cells, Dim count, Dim missing)
{
	Dim size;
	Dim placed = 0;
	Dim total_available = 0;
	for (Dim i = 0; i < count; i++)
	{
		total_available += cells[i].m_placed - cells[i].m_marged;
	}
	for (Dim i = 0; i < count; i++)
	{
		size = cells[i].m_marged;
		uint64_t sub = (((uint64_t)missing * ((uint64_t)cells[i].m_placed - (uint64_t)cells[i].m_marged))<<7) / (uint64_t)total_available;
		sub = (sub + 64)>>7;
		cells[i].m_placed -= (uint32_t)sub;
		placed += cells[i].m_placed;
	}
	return placed;
}


// Handles the case where the layout still has room, but not enough for all widgets
Dim Cells::flatten_marged(Cell * cells, Dim count, Dim area_size,  Dim missing, Dim available)
{
	Dim placed = 0;
	Dim total_recovered = 0;
	Dim total_missing = 0;
	Dim count_missing = 0;
	
	// Recover space not essential
	for (Dim i = 0; i < count; i++)
	{
		if (cells[i].m_placed > cells[i].m_marged)
		{
			total_recovered += cells[i].m_placed - cells[i].m_marged;
			cells[i].m_placed = cells[i].m_marged;
		}
		else if (cells[i].m_placed < cells[i].m_marged)
		{
			count_missing++;
		}
	}

	Dim to_remove_space;
	Dim to_remove_count;

	// Distributes the recovered space in priority to those who are not too greedy
	do
	{
		to_remove_space = 0;
		to_remove_count = 0;
		placed = 0;
		for (Dim i = 0; i < count; i++)
		{
			if (cells[i].m_placed < cells[i].m_marged)
			{
				uint64_t add = (((total_recovered << 7) / count_missing) + 64)>>7;
				if ((cells[i].m_placed + add) > cells[i].m_marged)
				{
					to_remove_space = (cells[i].m_placed + (uint32_t)add) - cells[i].m_marged;
					to_remove_count++;
					cells[i].m_placed = cells[i].m_marged;
				}
			}
			placed += cells[i].m_placed;
		}
		count_missing -= to_remove_count;
		total_recovered -= to_remove_space;
	}
	while (to_remove_count != 0);

	// If there is still room, distribute the remaining space to those who do not have enough.
	if (total_recovered > 0 && count_missing > 0)
	{
		placed = 0;
		for (Dim i = 0; i < count; i++)
		{
			if (cells[i].m_placed < cells[i].m_marged)
			{
				uint64_t add = (((total_recovered << 7) / count_missing) + 64)>>7;
				cells[i].m_placed += (uint32_t)add;
			}
			placed += cells[i].m_placed;
		}
	}

	// If this exceeds the allowed space
	if (placed > area_size)
	{
		Dim replaced = placed -area_size;
		placed = 0;

		// Reduce each cell equally
		for (Dim i = 0; i < count; i++)
		{
			cells[i].m_placed -= replaced / count;
			placed += cells[i].m_placed;
		}
	}
	return placed;
}

Dim Cells::resize_dim(Cell * cells, Dim count, Dim area_size, bool on_width)
{
	Dim result           = area_size;
	Dim missing_mini      = 0; 
	Dim available_mini    = 0;
	Dim missing_marged    = 0;
	Dim available_marged  = 0;
	Dim ideal_cell_size    = area_size / count;
	Dim ideal_current_size = 0;
	Dim previous_max_count = 0;
	Dim max_size          = 0;
	Dim max_count         = 0;
	uint32_t policy = 0;

	// Reduce the max size to area size
	for (Dim i = 0; i < count; i++)
	{
		if (cells[i].m_mini == Size::MAX_SIZE)
		{
			cells[i].m_mini = area_size;
		}
		if (cells[i].m_maxi == Size::MAX_SIZE)
		{
			cells[i].m_maxi = area_size;
		}

		if (cells[i].m_size_policy == SizePolicy::ENLARGE_ALL)
		{
			policy++;
		}
		else if (cells[i].m_size_policy == SizePolicy::SHRINK_ALL)
		{
			if (cells[i].m_marged < cells[i].m_mini)
			{
				cells[i].m_maxi = cells[i].m_mini;
			}
			else
			{
				cells[i].m_maxi = cells[i].m_marged;
			}
		}
	}
	
	// Compute the maximals sizes
	do
	{
		previous_max_count = max_count;
		max_size = 0;
		max_count = 0;

		// Set the ideal size of all cells
		for (Dim i = 0; i < count; i++)
		{
			if (ideal_cell_size > cells[i].m_maxi)
			{
				max_size += cells[i].m_maxi;
				max_count ++;
			}
		}

		if (max_size > 0)
		{
			if (max_count < count)
			{
				ideal_cell_size = (area_size - max_size) / (count - max_count);
			}
		}
	}
	while(previous_max_count < max_count);


	// If max size set for all cells but the layout size is greater than total max size
	if(count == max_count)
	{
		ideal_cell_size += (area_size - max_size)/count;
		
		for (Dim i = 0; i < count; i++)
		{
			cells[i].m_maxi += (area_size - max_size)/count;
		}
	}

	// Set the ideal size of all cells
	for (Dim i = 0; i < count; i++)
	{
		ideal_current_size = ideal_cell_size;

		if (ideal_cell_size > cells[i].m_maxi)
		{
			ideal_current_size = cells[i].m_maxi;
		} 

		cells[i].m_placed = ideal_current_size;

		// If the minimal size greater than ideal size
		if(cells[i].m_mini > ideal_current_size)
		{
			// Add missing space for minimal size
			missing_mini += cells[i].m_mini - ideal_current_size;
		}
		else
		{
			// Add available space in minimal size
			available_mini += ideal_current_size -  cells[i].m_mini;
		}

		// If the content size greater than ideal size
		if(cells[i].m_marged > ideal_current_size)
		{
			// Add missing space for content size
			missing_marged += cells[i].m_marged - ideal_current_size;
		}
		else
		{
			// Add available space in content size
			available_marged += ideal_current_size -  cells[i].m_marged;
		}
	}

	// If space missing for all minimal size
	if(missing_mini > 0)
	{
		// If space missing for minimal size but avaible space is enough to place all minimal size
		if(missing_mini <= available_mini)
		{
			result = reduce_mini(cells, count, missing_mini, available_mini);
		}
		// If space missing for content size but available space is enough to place all content size
		else if(missing_marged <= available_marged)
		{
			result = reduce_mini(cells,   count, available_mini, available_mini);
			result = reduce_marged(cells, count, missing_mini-available_mini);
		}
		else
		{
			result = flatten_marged(cells, count, area_size, missing_marged, available_marged);
		}
	}

	// One or more elements push the others
	if (policy > 0)
	{
		Dim reclaimed_space = 0;

		// Reduces all elements that are larger than they should be.
		for (Dim i = 0; i < count; i++)
		{
			if (cells[i].m_size_policy == SizePolicy::NORMAL_SIZE)
			{
				if (cells[i].m_marged >= cells[i].m_mini)
				{
					if (cells[i].m_placed > cells[i].m_marged)
					{
						reclaimed_space += cells[i].m_placed - cells[i].m_marged;
						cells[i].m_placed = cells[i].m_marged;
					}
				}
				else 
				{
					if (cells[i].m_placed > cells[i].m_mini)
					{
						reclaimed_space += cells[i].m_placed - cells[i].m_mini;
						cells[i].m_placed = cells[i].m_mini;
					}
				}
			}
		}

		// Enlarges elements that have a size policy
		result = 0;
		for (Dim i = 0; i < count; i++)
		{
			if (cells[i].m_size_policy == SizePolicy::ENLARGE_ALL)
			{
				cells[i].m_placed += (reclaimed_space/policy);
			}
			result += cells[i].m_placed;
		}
	}
 	return result;
}

/** Resize the cells according to the area size */
Size Cells::resize(const Area & area)
{
	Size result;
	Dim available_width = area.width_();
	Dim available_height = area.height_();
	
	// Reserve space for grid lines BEFORE distributing to cells
	if (m_vertical_line_thickness > 0 && m_columns_count > 0)
	{
		available_width -= (m_columns_count + 1) * m_vertical_line_thickness;
	}
	if (m_horizontal_line_thickness > 0 && m_rows_count > 0)
	{
		available_height -= (m_rows_count + 1) * m_horizontal_line_thickness;
	}
	
	// Distribute only the remaining space to cells
	result.width_(resize_dim(m_widths, m_columns_count, available_width, true));
	result.height_(resize_dim(m_heights, m_rows_count, available_height, false));
	
	return result;
}

/** Places widgets according to the area position */
void Cells::place(Widget *widget, const Area & area)
{
	if (widget)
	{
		Dim row, column;
		Area cell = area;

		rebound(widget);
		calc_sizes(widget);
		resize(area);

		// Precompute row positions (account for grid line thickness)
		Dim y_pos = area.y_() + m_horizontal_line_thickness;
		for (Dim i = 0; i < m_rows_count; i++)
		{
			m_row_positions[i] = y_pos;
			y_pos += (m_heights[i].m_placed & 0xFFFFFFFC0);
			if (i < m_rows_count - 1)
			{
				y_pos += m_horizontal_line_thickness;
			}
		}
		// Add bottom border (for calculation completeness)
		y_pos += m_horizontal_line_thickness;

		// Precompute column positions (account for grid line thickness)
		Dim* col_widths = new Dim[m_columns_count];
		Dim x_pos = area.x_() + m_vertical_line_thickness;
		for (Dim i = 0; i < m_columns_count; i++)
		{
			m_col_positions[i] = x_pos;
			col_widths[i] = m_widths[i].m_placed & 0xFFFFFFFC0;
			x_pos += col_widths[i];
			if (i < m_columns_count - 1)
			{
				x_pos += m_vertical_line_thickness;
			}
		}
		// Add right border (for calculation completeness)
		x_pos += m_vertical_line_thickness;

		// Iterate through all widgets directly (single traversal)
		Widget * current = widget->m_children;
		while(current)
		{
			row = current->row();
			column = current->column();

			// Only place if widget is in a valid cell
			if (row < m_rows_count && column < m_columns_count)
			{
				cell.x_(m_col_positions[column]);
				cell.y_(m_row_positions[row]);
				cell.width_(col_widths[column]);
				cell.height_(m_heights[row].m_placed & 0xFFFFFFFC0);
				current->place(cell, true);
			}
			current = current->m_next;
		}

		delete[] col_widths;
	}
}

/** Recalculates the boundaries based on content */
void Cells::rebound(Widget * widget)
{
	Dim rows_count, columns_count;
	rows_count = columns_count = UNDEFINED_CELL;

	if(widget)
	{
		Widget * current = widget->m_children;
		while (current)
		{
			if (current->column() != UNDEFINED_CELL && current->row() != UNDEFINED_CELL)
			{
				if (rows_count    == UNDEFINED_CELL || current->row() >= rows_count)
				{
					rows_count = current->row()+1;
				}
				if (columns_count == UNDEFINED_CELL || current->column() >= columns_count)
				{
					columns_count = current->column()+1;
				}
			}
			current = current->m_next;
		}
	}

	if (rows_count != UNDEFINED_CELL && columns_count != UNDEFINED_CELL)
	{
		reallocate(rows_count, columns_count);
	}
	else
	{
		reallocate(1, 1);
	}
}
