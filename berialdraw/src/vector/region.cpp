#include "berialdraw_imp.hpp"

using namespace berialdraw;

const Region::RegionBox Region::EMPTY_BOX = { 0, 0, 0, 0 };
const Region::RegionData Region::EMPTY_DATA = { 0, 0 };
const Region::RegionData Region::BROKEN_DATA = { 0, 0 };

Region::RegionBox *Region::g_empty_box = (Region::RegionBox *)&Region::EMPTY_BOX;
Region::RegionData *Region::g_empty_data = (Region::RegionData *)&Region::EMPTY_DATA;
Region::RegionData *Region::g_broken_data = (Region::RegionData *)&Region::BROKEN_DATA;

Region::RegionData * Region::alloc_data (int32_t n)
{
	int32_t sz = get_alloc_size (n);
	if (!sz)
	{
		return NULL;
	}
	return (RegionData * )bd_malloc (sz);
}

bool Region::rect_alloc (RegionBoxes * & region, int32_t n)
{
	RegionData *data;

	if (!region->data)
	{
		n++;
		region->data = alloc_data (n);

		if (!region->data)
		{
			return region_break (region);
		}

		region->data->num_rects = 1;
		*box_pointer (region) = region->extents;
	}
	else if (!region->data->size)
	{
		region->data = alloc_data (n);

		if (!region->data)
		{
			return region_break (region);
		}

		region->data->num_rects = 0;
	}
	else
	{
		int32_t data_size;

		if (n == 1)
		{
			n = region->data->num_rects;
			if (n > 500)
			{
				n = 250;
			}
		}

		n += region->data->num_rects;
		data_size = get_alloc_size (n);

		if (!data_size)
		{
			data = NULL;
		}
		else
		{
			data = (RegionData *) bd_realloc (region->data, get_alloc_size (n));
		}
	
		if (!data)
		{
			return region_break (region);
		}
	
		region->data = data;
	}
 
	region->data->size = n;

	return true;
}

void Region::rect_allocate(RegionBoxes *region, int32_t n)
{
	if (!(region)->data ||(((region)->data->num_rects + (n)) > (region)->data->size))
	{
		if (!rect_alloc (region, n))
		{
			throw std::runtime_error("");
		}
	}
}

void Region::new_rect(RegionBoxes *region, RegionBox * & next_rect, Coord nx1, Coord ny1, Coord nx2, Coord ny2)
{
	if (region)
	{
		if (!region->data || (region->data->num_rects == region->data->size))
		{
			if (!rect_alloc (region, 1))
			{
				throw std::runtime_error("");
			}
			next_rect = get_next_box (region);
		}
		next_rect->x1 = nx1;
		next_rect->y1 = ny1;
		next_rect->x2 = nx2;
		next_rect->y2 = ny2;
		next_rect++;
		region->data->num_rects++;
	}
}

bool Region::is_equal (RegionBoxes *reg1, RegionBoxes *reg2)
{
	int32_t i;
	RegionBox *rects1;
	RegionBox *rects2;

	if (reg1->extents.x1 != reg2->extents.x1)
	{
		return false;
	}
 
	if (reg1->extents.x2 != reg2->extents.x2)
	{
		return false;
	}
 
	if (reg1->extents.y1 != reg2->extents.y1)
	{
		return false;
	}
 
	if (reg1->extents.y2 != reg2->extents.y2)
	{
		return false;
	}
 
	if (num_rects (reg1) != num_rects (reg2))
	{
		return false;
	}

	rects1 = region_rects (reg1);
	rects2 = region_rects (reg2);
 
	for (i = 0; i != num_rects (reg1); i++)
	{
		if (rects1[i].x1 != rects2[i].x1)
		{
			return false;
		}
	
		if (rects1[i].x2 != rects2[i].x2)
		{
			return false;
		}
	
		if (rects1[i].y1 != rects2[i].y1)
		{
			return false;
		}
	
		if (rects1[i].y2 != rects2[i].y2)
		{
			return false;
		}
	}
	return true;
}

void Region::create (RegionBoxes *region)
{
	region->extents = *g_empty_box;
	region->data = g_empty_data;
}

void Region::init_rect (RegionBoxes *	region, Coord x, Coord y, Dim width, Dim height)
{
	region->extents.x1 = x;
	region->extents.y1 = y;
	region->extents.x2 = x + width;
	region->extents.y2 = y + height;
	region->data = NULL;

	if (!is_good_rect (&region->extents))
	{
		if (!is_bad_rect (&region->extents))
		{
			create (region);
		}
	}
}

void Region::init_with_extents (RegionBoxes *region, RegionBox *extents)
{
	if (!is_good_rect (extents))
	{
		if (is_bad_rect (extents))
		{
			return;
		}
		create (region);
		return;
	}
	region->extents = *extents;
	region->data = NULL;
}

void Region::destroy(RegionBoxes & reg)
{
	if (reg.data && reg.data->size)
	{
		bd_free (reg.data);
		reg.data = 0;
	}
}

bool Region::region_break (RegionBoxes *region)
{
	destroy (*region);
	region->extents = *g_empty_box;
	region->data = g_broken_data;
	return false;
}

bool Region::copy(RegionBoxes *dst, RegionBoxes *src)
{
	if (dst == src)
		return true;

	dst->extents = src->extents;

	if (!src->data || !src->data->size)
	{
		destroy (*dst);
		dst->data = src->data;
		return true;
	}
 
	if (!dst->data || (dst->data->size < src->data->num_rects))
	{
		destroy (*dst);

		dst->data = alloc_data (src->data->num_rects);

		if (!dst->data)
		{
			return region_break (dst);
		}

		dst->data->size = src->data->num_rects;
	}

	dst->data->num_rects = src->data->num_rects;
	memmove ((char *)box_pointer (dst), (char *)box_pointer (src), dst->data->num_rects * sizeof(RegionBox));
	return true;
}

int32_t Region::coalesce (RegionBoxes * region, int32_t prev_start, int32_t cur_start)
{
	RegionBox *prev_box;
	RegionBox *cur_box;
	int32_t numRects;
	Coord y2;

	numRects = cur_start - prev_start;

	if (!numRects) return cur_start;

	prev_box = get_box (region, prev_start);
	cur_box = get_box (region, cur_start);
	if (prev_box->y2 != cur_box->y1) return cur_start;

	y2 = cur_box->y2;

	do
	{
		if ((prev_box->x1 != cur_box->x1) || (prev_box->x2 != cur_box->x2))
		{
			return (cur_start);
		}
	
		prev_box++;
		cur_box++;
		numRects--;
	}
	while (numRects);

	numRects = cur_start - prev_start;
	region->data->num_rects -= numRects;

	do
	{
		prev_box--;
		prev_box->y2 = y2;
		numRects--;
	}
	while (numRects);

	return prev_start;
}

bool Region::append_non (RegionBoxes * region, RegionBox * r, RegionBox * r_end, Coord y1, Coord y2)
{
	RegionBox *next_rect;
	int32_t new_rects;

	new_rects = (int32_t)(r_end - r);

	if (!(region)->data || (((region)->data->num_rects + new_rects) > (region)->data->size))
	{
		if (!rect_alloc (region, new_rects))
		{
			throw std::runtime_error("");
		}
	}

	next_rect = get_next_box (region);
	region->data->num_rects += new_rects;

	do
	{
		next_rect->x1 = r->x1;
		next_rect->y1 = y1;
		next_rect->x2 = r->x2;
		next_rect->y2 = y2;
		next_rect++;
		r++;
	}
	while (r != r_end);

	return true;
}

void Region::find_band(RegionBox * r, RegionBox * & r_band_end, RegionBox * & r_end, Coord & ry1)
{
	ry1 = r->y1;
	r_band_end = r + 1;
	while ((r_band_end != r_end) && (r_band_end->y1 == ry1))
	{
		r_band_end++;
	}
}

void Region::append_region(RegionBoxes * new_reg, RegionBox * r, RegionBox * r_end)
{
	int32_t new_rects;
	if ((new_rects = (int32_t)(r_end - r)))
	{
		rect_allocate (new_reg, new_rects);
		memmove ((char *)get_next_box (new_reg), (char *)r, new_rects * sizeof(RegionBox));
		new_reg->data->num_rects += new_rects;
	}
}


bool Region::operate (RegionBoxes * new_reg, RegionBoxes * reg1, RegionBoxes * reg2, overlap_function overlap_func, bool append_non1, bool append_non2)
{
	RegionData *old_data = 0;
	try
	{
		RegionBox *r1;
		RegionBox *r2;
		RegionBox *r1_end;
		RegionBox *r2_end;
		Coord ybot;
		Coord ytop;
		int32_t prev_band;
		int32_t cur_band;
		RegionBox * r1_band_end;
		RegionBox * r2_band_end;
		Coord top;
		Coord bot;
		Coord r1y1;
		Coord r2y1;
		int32_t new_size;
		int32_t numRects;

		if (is_broken_data (reg1) || is_broken_data (reg2))
		{
			return region_break (new_reg);
		}

		r1 = region_rects (reg1);
		new_size = num_rects (reg1);
		r1_end = r1 + new_size;

		numRects = num_rects (reg2);
		r2 = region_rects (reg2);
		r2_end = r2 + numRects;
 

		old_data = (RegionData *)NULL;

		if (((new_reg == reg1) && (new_size > 1)) || ((new_reg == reg2) && (numRects > 1)))
		{
			old_data = new_reg->data;
			new_reg->data = g_empty_data;
		}

		if (numRects > new_size)
		{
			new_size = numRects;
		}

		new_size <<= 1;

		if (!new_reg->data)
		{
			new_reg->data = g_empty_data;
		}
		else if (new_reg->data->size)
		{
			new_reg->data->num_rects = 0;
		}

		if (new_size > new_reg->data->size)
		{
			if (!rect_alloc (new_reg, new_size))
			{
				bd_free (old_data);
				throw std::runtime_error("");
			}
		}
		ybot = std::min (r1->y1, r2->y1);

		prev_band = 0;

		do
		{
			find_band (r1, r1_band_end, r1_end, r1y1);
			find_band (r2, r2_band_end, r2_end, r2y1);

			if (r1y1 < r2y1)
			{
				if (append_non1)
				{
					top = std::max (r1y1, ybot);
					bot = std::min (r1->y2, r2y1);
					if (top != bot)
					{
						cur_band = new_reg->data->num_rects;
						if (!append_non (new_reg, r1, r1_band_end, top, bot))
						{
							throw std::runtime_error("");
						}
						if (cur_band - prev_band == new_reg->data->num_rects - cur_band)
						{
							prev_band = coalesce (new_reg, prev_band, cur_band);
						}
						else
						{
							prev_band = cur_band;
						}
					}
				}
				ytop = r2y1;
			}
			else if (r2y1 < r1y1)
			{
				if (append_non2)
				{
					top = std::max (r2y1, ybot);
					bot = std::min (r2->y2, r1y1);
		
					if (top != bot)
					{
						cur_band = new_reg->data->num_rects;

						if (!append_non (new_reg, r2, r2_band_end, top, bot))
						{
							throw std::runtime_error("");
						}
						if (cur_band - prev_band == new_reg->data->num_rects - cur_band)
						{
							prev_band = coalesce (new_reg, prev_band, cur_band);
						}
						else
						{
							prev_band = cur_band;
						}
					}
				}
				ytop = r1y1;
			}
			else
			{
				ytop = r1y1;
			}

			ybot = std::min (r1->y2, r2->y2);
			if (ybot > ytop)
			{
				cur_band = new_reg->data->num_rects;

				if (!(this->*overlap_func)(new_reg, r1, r1_band_end, r2, r2_band_end, ytop, ybot))
				{
					throw std::runtime_error("");
				} 
				if (cur_band - prev_band == new_reg->data->num_rects - cur_band)
				{
					prev_band = coalesce (new_reg, prev_band, cur_band);
				}
				else
				{
					prev_band = cur_band;
				}
			}

			if (r1->y2 == ybot) r1 = r1_band_end;

			if (r2->y2 == ybot) r2 = r2_band_end;
		}
		while (r1 != r1_end && r2 != r2_end);

		if ((r1 != r1_end) && append_non1)
		{
			find_band (r1, r1_band_end, r1_end, r1y1);
			cur_band = new_reg->data->num_rects;

			if (!append_non (new_reg, r1, r1_band_end, std::max (r1y1, ybot), r1->y2))
			{
				throw std::runtime_error("");
			}
	
			if (cur_band - prev_band == new_reg->data->num_rects - cur_band)
			{
				prev_band = coalesce (new_reg, prev_band, cur_band);
			}
			else
			{
				prev_band = cur_band;
			}
			append_region (new_reg, r1_band_end, r1_end);
		}
		else if ((r2 != r2_end) && append_non2)
		{
			find_band (r2, r2_band_end, r2_end, r2y1);

			cur_band = new_reg->data->num_rects;

			if (!append_non (new_reg, r2, r2_band_end, std::max (r2y1, ybot), r2->y2))
			{
				throw std::runtime_error("");
			}

			if (cur_band - prev_band == new_reg->data->num_rects - cur_band)
			{
				prev_band = coalesce (new_reg, prev_band, cur_band);
			}
			else
			{
				prev_band = cur_band;
			}
			append_region (new_reg, r2_band_end, r2_end);
		}

		bd_free (old_data);

		if (!(numRects = new_reg->data->num_rects))
		{
			destroy (*new_reg);
			new_reg->data = g_empty_data;
		}
		else if (numRects == 1)
		{
			new_reg->extents = *box_pointer (new_reg);
			destroy (*new_reg);
			new_reg->data = (RegionData *)NULL;
		}
		else
		{
			if (((numRects) < (new_reg->data->size >> 1)) && (new_reg->data->size > 50))
			{
				RegionData * new_data;
				int32_t data_size = get_alloc_size (numRects);

				if (!data_size)
				{
					new_data = NULL;
				}
				else
				{
					new_data = (RegionData *) bd_realloc (new_reg->data, data_size);
				}

				if (new_data)
				{
					new_data->size = (numRects);
					new_reg->data = new_data;
				}
			}
		}
	}
	catch(...)
	{
		bd_free (old_data);
		return region_break (new_reg);
	}
	return true;
}

void Region::set_extents (RegionBoxes *region)
{
	RegionBox *current, *last;

	if (!region->data) return;

	if (!region->data->size)
	{
		region->extents.x2 = region->extents.x1;
		region->extents.y2 = region->extents.y1;
		return;
	}

	current = box_pointer (region);
	last = get_last_box (region);
	region->extents.x1 = current->x1;
	region->extents.y1 = current->y1;
	region->extents.x2 = last->x2;
	region->extents.y2 = last->y2;

	while (current <= last)
	{
		if (current->x1 < region->extents.x1)
		region->extents.x1 = current->x1;
		if (current->x2 > region->extents.x2)
		region->extents.x2 = current->x2;
		current++;
	}
}

bool Region::intersect_operation (RegionBoxes *region, RegionBox * r1, RegionBox * r1_end, RegionBox * r2, RegionBox * r2_end, Coord y1, Coord y2)
{
	Coord x1;
	Coord x2;
	RegionBox *  next_rect;

	next_rect = get_next_box (region);
	do
	{
		x1 = std::max (r1->x1, r2->x1);
		x2 = std::min (r1->x2, r2->x2);

		if (x1 < x2) new_rect (region, next_rect, x1, y1, x2, y2);

		if (r1->x2 == x2)
		{
			r1++;
		}

		if (r2->x2 == x2)
		{
			r2++;
		}
	}
	while ((r1 != r1_end) && (r2 != r2_end));

	return true;
}

bool Region::intersect(RegionBoxes * new_reg, RegionBoxes * reg1, RegionBoxes * reg2)
{
	if (is_empty (reg1) || is_empty (reg2) || !extent_check (&reg1->extents, &reg2->extents))
	{
		destroy (*new_reg);
		new_reg->extents.x2 = new_reg->extents.x1;
		new_reg->extents.y2 = new_reg->extents.y1;
		if (is_broken_data (reg1) || is_broken_data (reg2))
		{
			new_reg->data = g_broken_data;
			return false;
		}
		else
		{
			new_reg->data = g_empty_data;
		}
	}
	else if (!reg1->data && !reg2->data)
	{
		new_reg->extents.x1 = std::max (reg1->extents.x1, reg2->extents.x1);
		new_reg->extents.y1 = std::max (reg1->extents.y1, reg2->extents.y1);
		new_reg->extents.x2 = std::min (reg1->extents.x2, reg2->extents.x2);
		new_reg->extents.y2 = std::min (reg1->extents.y2, reg2->extents.y2);

		destroy (*new_reg);
		new_reg->data = (RegionData *)NULL;
	}
	else if (!reg2->data && is_contains (&reg2->extents, &reg1->extents))
	{
		return copy (new_reg, reg1);
	}
	else if (!reg1->data && is_contains (&reg1->extents, &reg2->extents))
	{
		return copy (new_reg, reg2);
	}
	else if (reg1 == reg2)
	{
		return copy(new_reg, reg1);
	}
	else
	{
		if (!operate (new_reg, reg1, reg2, &Region::intersect_operation, false, false))
		{
			return false;
		}
		set_extents (new_reg);
	}
	return(true);
}

void Region::merge_rect(RegionBoxes * & region, RegionBox * & r, RegionBox * & next_rect, Coord & x1, Coord & y1, Coord & x2, Coord & y2)
{
	if (r->x1 <= x2)
	{
		if (x2 < r->x2)
		{
			x2 = r->x2;
		}
	}
	else
	{
		new_rect (region, next_rect, x1, y1, x2, y2);
		x1 = r->x1;
		x2 = r->x2;
	}
	r++;
}


bool Region::union_operation (RegionBoxes *region, RegionBox * r1, RegionBox * r1_end, RegionBox * r2, RegionBox * r2_end, Coord y1, Coord y2)
{
	RegionBox *next_rect;
	Coord x1;
	Coord x2;

	next_rect = get_next_box (region);

	if (r1->x1 < r2->x1)
	{
		x1 = r1->x1;
		x2 = r1->x2;
		r1++;
	}
	else
	{
		x1 = r2->x1;
		x2 = r2->x2;
		r2++;
	}
	while (r1 != r1_end && r2 != r2_end)
	{
		if (r1->x1 < r2->x1)
		{
			merge_rect (region, r1, next_rect, x1, y1, x2, y2);
		}
		else
		{
			merge_rect (region, r2, next_rect, x1, y1, x2, y2);
		}
	}

	if (r1 != r1_end)
	{
		do
		{
			merge_rect (region, r1, next_rect, x1, y1, x2, y2);
		}
		while (r1 != r1_end);
	}
	else if (r2 != r2_end)
	{
		do
		{
			merge_rect (region, r2, next_rect, x1, y1, x2, y2);
		}
		while (r2 != r2_end);
	}
	new_rect (region, next_rect, x1, y1, x2, y2);

	return true;
}

bool Region::intersect_rect (RegionBoxes *dest, RegionBoxes *source, Coord x, Coord y, Dim width, Dim height)
{
	RegionBoxes region;

	region.data = NULL;
	region.extents.x1 = x;
	region.extents.y1 = y;
	region.extents.x2 = x + width;
	region.extents.y2 = y + height;

	return intersect (dest, source, &region);
}

bool Region::union_rect (RegionBoxes *dest, RegionBoxes *source, Coord x, Coord y, Dim width, Dim height)
{
	RegionBoxes region;

	region.extents.x1 = x;
	region.extents.y1 = y;
	region.extents.x2 = x + width;
	region.extents.y2 = y + height;

	if (!is_good_rect (&region.extents))
	{
		if (is_bad_rect (&region.extents))
		{
			return false;
		}
		return copy(dest, source);
	}
	region.data = NULL;
	return unite (dest, source, &region);
}

bool Region::unite (RegionBoxes *new_reg, RegionBoxes *reg1, RegionBoxes *reg2)
{
	if (reg1 == reg2)
	{
		return copy (new_reg, reg1);
	}

	if (is_empty (reg1))
	{
		if (is_broken_data (reg1))
		{
			return region_break (new_reg);
		}

		if (new_reg != reg2)
		{
			return copy (new_reg, reg2);
		}
		return true;
	}

	if (is_empty (reg2))
	{
		if (is_broken_data (reg2))
		{
			return region_break (new_reg);
		}

		if (new_reg != reg1)
		{
			return copy (new_reg, reg1);
		}
		return true;
	}

	if (!reg1->data && is_contains (&reg1->extents, &reg2->extents))
	{
		if (new_reg != reg1)
		{
			return copy (new_reg, reg1);
		}
		return true;
	}

	if (!reg2->data && is_contains (&reg2->extents, &reg1->extents))
	{
		if (new_reg != reg2)
		{
			return copy (new_reg, reg2);
		}
		return true;
	}

	if (!operate (new_reg, reg1, reg2, &Region::union_operation, true, true))
	{
		return false;
	}

	new_reg->extents.x1 = std::min (reg1->extents.x1, reg2->extents.x1);
	new_reg->extents.y1 = std::min (reg1->extents.y1, reg2->extents.y1);
	new_reg->extents.x2 = std::max (reg1->extents.x2, reg2->extents.x2);
	new_reg->extents.y2 = std::max (reg1->extents.y2, reg2->extents.y2);

	return true;
}

bool Region::subtract_operation (RegionBoxes * region, RegionBox *r1, RegionBox *r1_end,RegionBox * r2,RegionBox * r2_end,Coord y1, Coord y2)
{
	RegionBox * next_rect;
	Coord x1;

	x1 = r1->x1;
	next_rect = get_next_box (region);

	do
	{
		if (r2->x2 <= x1)
		{
			r2++;
		}
		else if (r2->x1 <= x1)
		{
			x1 = r2->x2;
			if (x1 >= r1->x2)
			{
				r1++;
				if (r1 != r1_end)
				{
					x1 = r1->x1;
				}
			}
			else
			{
				r2++;
			}
		}
		else if (r2->x1 < r1->x2)
		{
			new_rect (region, next_rect, x1, y1, r2->x1, y2);

			x1 = r2->x2;
			if (x1 >= r1->x2)
			{
				r1++;
				if (r1 != r1_end)
				{
					x1 = r1->x1;
				}
			}
			else
			{
				r2++;
			}
		}
		else
		{
			if (r1->x2 > x1)
			{
				new_rect (region, next_rect, x1, y1, r1->x2, y2);
			}

			r1++;

			if (r1 != r1_end)
			{
				x1 = r1->x1;
			}
		}
	}
	while ((r1 != r1_end) && (r2 != r2_end));

	while (r1 != r1_end)
	{
		new_rect (region, next_rect, x1, y1, r1->x2, y2);

		r1++;
		if (r1 != r1_end)
		{
			x1 = r1->x1;
		}
	}
	return true;
}

bool Region::subtract (RegionBoxes *reg_d, RegionBoxes *reg_m, RegionBoxes *reg_s)
{
	if (is_empty (reg_m) || is_empty (reg_s) || !extent_check (&reg_m->extents, &reg_s->extents))
	{
		if (is_broken_data (reg_s))
		{
			return region_break (reg_d);
		}
		return copy (reg_d, reg_m);
	}
	else if (reg_m == reg_s)
	{
		destroy (*reg_d);
		reg_d->extents.x2 = reg_d->extents.x1;
		reg_d->extents.y2 = reg_d->extents.y1;
		reg_d->data = g_empty_data;
		return true;
	}

	if (!operate (reg_d, reg_m, reg_s, &Region::subtract_operation, true, false))
	{
		return false;
	}
	set_extents (reg_d);
	return true;
}

bool Region::inverse (RegionBoxes *new_reg, RegionBoxes *reg1, RegionBox * inv_rect)
{
	RegionBoxes inv_reg;
 
	if (is_empty (reg1) || !extent_check (inv_rect, &reg1->extents))
	{
		if (is_broken_data (reg1))
		{
			return region_break (new_reg);
		}
	
		new_reg->extents = *inv_rect;
		destroy (*new_reg);
		new_reg->data = (RegionData *)NULL;
		return true;
	}

	inv_reg.extents = *inv_rect;
	inv_reg.data = (RegionData *)NULL;
	if (!operate (new_reg, &inv_reg, reg1, &Region::subtract_operation, true, false))
	{
		return false;
	}

	set_extents (new_reg);
	return true;
}

const Region::RegionBox * Region::find_box_for_y (const RegionBox *begin, const RegionBox *end, Coord y) const
{
	const RegionBox *mid;

	if (end == begin)
	{
		return end;
	}

	if (end - begin == 1)
	{
		if (begin->y2 > y)
		{
			return begin;
		}
		else
		{
			return end;
		}
	}

	mid = begin + (end - begin) / 2;
	if (mid->y2 > y)
	{
		return find_box_for_y (begin, mid, y);
	}
	else
	{
		return find_box_for_y (mid, end, y);
	}
}

Region::Overlap Region::is_contains_rectangle (const RegionBoxes * region, RegionBox *  prect) const
{
	const RegionBox *  pbox;
	const RegionBox *  pbox_end;
	int32_t part_in, part_out;
	int32_t numRects;
	Coord x, y;

	numRects = num_rects (region);

	if (!numRects || !extent_check (&region->extents, prect))
	{
		return(Region::OUT);
	}

	if (numRects == 1)
	{
		if (is_contains (&region->extents, prect))
		{
			return Region::IN;
		}
		else
		{
			return Region::PART;
		}
	}

	part_out = false;
	part_in = false;

	x = prect->x1;
	y = prect->y1;

	for (pbox = box_pointer (region), pbox_end = pbox + numRects; pbox != pbox_end; pbox++)
	{
		if (pbox->y2 <= y)
		{
			if ((pbox = find_box_for_y (pbox, pbox_end, y)) == pbox_end)
			{
				break;
			}
		}

		if (pbox->y1 > y)
		{
			part_out = true;
			if (part_in || (pbox->y1 >= prect->y2))
			{
				break;
			}
			y = pbox->y1;
		}

		if (pbox->x2 <= x)
		{
			continue;
		}

		if (pbox->x1 > x)
		{
			part_out = true;
			if (part_in)
			{
				break;
			}
		}

		if (pbox->x1 < prect->x2)
		{
			part_in = true;
			if (part_out)
			{
				break;
			}
		}

		if (pbox->x2 >= prect->x2)
		{
			y = pbox->y2;
			if (y >= prect->y2)
			{
				break;
			}
			x = prect->x1;
		}
		else
		{
			part_out = true;
			break;
		}
	}

	if (part_in)
	{
		if (y < prect->y2)
		{
			return Region::PART;
		}
		else
		{
			return Region::IN;
		}
	}
	else
	{
		return Region::OUT;
	}
}

void Region::translate (RegionBoxes *region, Coord x, Coord y)
{
	Coord x1, x2, y1, y2;
	int32_t nbox;
	RegionBox * pbox;

	region->extents.x1 = x1 = region->extents.x1 + x;
	region->extents.y1 = y1 = region->extents.y1 + y;
	region->extents.x2 = x2 = region->extents.x2 + x;
	region->extents.y2 = y2 = region->extents.y2 + y;
 
	if (((x1 - INT32_MIN) | (y1 - INT32_MIN) | (INT32_MAX - x2) | (INT32_MAX - y2)) >= 0)
	{
		if (region->data && (nbox = region->data->num_rects))
		{
			for (pbox = box_pointer (region); nbox--; pbox++)
			{
				pbox->x1 += x;
				pbox->y1 += y;
				pbox->x2 += x;
				pbox->y2 += y;
			}
		}
		return;
	}

	if (((x2 - INT32_MIN) | (y2 - INT32_MIN) | (INT32_MAX - x1) | (INT32_MAX - y1)) <= 0)
	{
		region->extents.x2 = region->extents.x1;
		region->extents.y2 = region->extents.y1;
		destroy (*region);
		region->data = g_empty_data;
		return;
	}

	if (x1 < INT32_MIN)
	{
		region->extents.x1 = INT32_MIN;
	}
	else if (x2 > INT32_MAX)
	{
		region->extents.x2 = INT32_MAX;
	}

	if (y1 < INT32_MIN)
	{
		region->extents.y1 = INT32_MIN;
	}
	else if (y2 > INT32_MAX)
	{
		region->extents.y2 = INT32_MAX;
	}

	if (region->data && (nbox = region->data->num_rects))
	{
		RegionBox * pbox_out;

		for (pbox_out = pbox = box_pointer (region); nbox--; pbox++)
		{
			pbox_out->x1 = x1 = pbox->x1 + x;
			pbox_out->y1 = y1 = pbox->y1 + y;
			pbox_out->x2 = x2 = pbox->x2 + x;
			pbox_out->y2 = y2 = pbox->y2 + y;

			if (((x2 - INT32_MIN) | (y2 - INT32_MIN) | (INT32_MAX - x1) | (INT32_MAX - y1)) <= 0)
			{
				region->data->num_rects--;
				continue;
			}

			if (x1 < INT32_MIN)
			{
				pbox_out->x1 = INT32_MIN;
			}
			else if (x2 > INT32_MAX)
			{
				pbox_out->x2 = INT32_MAX;
			}

			if (y1 < INT32_MIN)
			{
				pbox_out->y1 = INT32_MIN;
			}
			else if (y2 > INT32_MAX)
			{
				pbox_out->y2 = INT32_MAX;
			}

			pbox_out++;
		}

		if (pbox_out != pbox)
		{
			if (region->data->num_rects == 1)
			{
				region->extents = *box_pointer (region);
				destroy (*region);
				region->data = (RegionData *)NULL;
			}
			else
			{
				set_extents (region);
			}
		}
	}
}

void Region::reset (RegionBoxes *region, RegionBox *box)
{
	region->extents = *box;
	destroy (*region);
	region->data = NULL;
}

void Region::clear (RegionBoxes *region)
{
	destroy (*region);
	region->extents = *g_empty_box;
	region->data = g_empty_data;
}

bool Region::is_contains_point (RegionBoxes * region, Coord x, Coord y, RegionBox * box)
{
	const RegionBox *pbox, *pbox_end;
	int32_t numRects;

	numRects = num_rects (region);

	if (!numRects || !is_in_box (&region->extents, x, y))
	{
		return(false);
	}

	if (numRects == 1)
	{
		if (box)
		{
			*box = region->extents;
		}

		return(true);
	}

	pbox = box_pointer (region);
	pbox_end = pbox + numRects;

	pbox = find_box_for_y (pbox, pbox_end, y);

	for (;pbox != pbox_end; pbox++)
	{
		if ((y < pbox->y1) || (x < pbox->x1))
		{
			break;
		}

		if (x >= pbox->x2)
		{
			continue;
		}

		if (box)
		{
			*box = *pbox;
		}

		return true;
	}

	return false;
}

Region::Region()
{
	create(&m_region);
}

Region::Region(const Area & area)
{
	init_rect(&m_region, area.position().x_(), area.position().y_(), area.size().width_(), area.size().height_());
}

Region::Region(const Region & other)
{
	create(&m_region);
	copy(&m_region,(RegionBoxes*)&other.m_region);
}

Region::~Region()
{
	destroy (m_region);
}

Region& Region::operator=(const Region& other)
{
	if(this != &other)
	{
		destroy(m_region);
		create(&m_region);
		copy(&m_region,(RegionBoxes*)&other.m_region);
	}
	return *this;
}

bool Region::intersect(const Region & reg)
{
	bool result;
	RegionBoxes new_region;
	create(&new_region);
	result = intersect(&new_region,(RegionBoxes*)&m_region,(RegionBoxes*)&reg.m_region);
	destroy(m_region);
	m_region = new_region;
	return result;
}

bool Region::unite(const Region & reg)
{
	bool result;
	RegionBoxes new_region;
	create(&new_region);
	result = unite(&new_region,(RegionBoxes*)&m_region,(RegionBoxes*)&reg.m_region);
	destroy(m_region);
	m_region = new_region;
	return result;
}

bool Region::subtract(const Region & reg)
{
	bool result;
	RegionBoxes new_region;
	create(&new_region);
	result = subtract(&new_region,(RegionBoxes*)&m_region,(RegionBoxes*)&reg.m_region);
	destroy(m_region);
	m_region = new_region;
	return result;
}


void Region::dump(const String & name, const RegionBoxes & reg) 
{
	bd_printf("%s : \n",name.c_str());
	bd_printf(" extents x1=%d y1=%d x2=%d y2=%d\n",
		reg.extents.x1>>6,
		reg.extents.y1>>6,
		reg.extents.x2>>6,
		reg.extents.y2>>6);

	if (reg.data)
	{
		for (int32_t i = 0; i < reg.data->num_rects; i++)
		{
			const Region::RegionBox * box = get_box(&reg, i);
			bd_printf("   box x1=%d y1=%d x2=%d y2=%d\n",
				box->x1>>6,
				box->y1>>6,
				box->x2>>6,
				box->y2>>6);
		}
	}
}


/** Test if a region contains a point (for renderer) */
bool Region::is_inside_scale(Coord x, Coord y, Dim scale) const
{
	if (is_inside((x<<6)/scale, (y<<6)/scale, 1, 1) != Region::OUT)
	{
		return true;
	}
	return false;
}

/** Test if a region contains a rectangle (for rendeder) */
Region::Overlap Region::is_inside_scale(Coord x, Coord y, Dim width, Dim height, Dim scale) const
{
	Dim w = (width <<6) / scale;
	Dim h = (height<<6) / scale;

	if ((width<<6) % scale)
	{
		w+=1;
	}

	// If the scale completely overwrites the value
	if (w == 0 && width != 0)
	{
		// Set the minimal width
		w = 1;
	}
	
	// If the scale completely overwrites the value
	if (h == 0 && height != 0)
	{
		// Set the minimal height
		h = 1;
	}
	return Region::is_inside((x<<6)/(Coord)scale, (y<<6)/(Coord)scale, w, h, true);
}

/** Test if a region contains a point */
Region::Overlap Region::is_inside(const Point & position) const
{
	Region::Overlap result = Region::OUT;
	if(!is_empty(&m_region))
	{
		RegionBox rect;
		rect.x1 = position.x_();
		rect.y1 = position.y_();
		rect.x2 = position.x_() + 64;
		rect.y2 = position.y_() + 64;
		result = is_contains_rectangle (&m_region, &rect);
	}
	return result;
}

/** Test if a region contains a rectangle */
Region::Overlap Region::is_inside(const Point & position, const Size & size) const
{
	Region::Overlap result = Region::OUT;
	if(!is_empty(&m_region))
	{
		RegionBox rect;
		rect.x1 = position.x_();
		rect.y1 = position.y_();
		rect.x2 = position.x_() + size.width_();
		rect.y2 = position.y_() + size.height_();
		result = is_contains_rectangle (&m_region, &rect);
	}
	return result;
}


Region::Overlap Region::is_inside(Coord x, Coord y, Dim width, Dim height, bool pixel) const
{
	Region::Overlap result = Region::OUT;
	if(!is_empty(&m_region))
	{
		RegionBox rect;
		x       = pixel ? x      << 6 : x;
		y       = pixel ? y      << 6 : y;
		width   = pixel ? width  << 6 : width;
		height  = pixel ? height << 6 : height;
		rect.x1 = x;
		rect.y1 = y;
		rect.x2 = x + width;
		rect.y2 = y + height;
		result = is_contains_rectangle (&m_region, &rect);
	}
	return result;
}

void Region::clear()
{
	clear(&m_region);
}


#ifdef _DEBUG
void Region::test1()
{
	Region r1;
	Area a1(50,50,50,50,true);

	r1.unite(a1);
	assert(r1.is_inside(0,0,51,50)  == Region::OUT);
	assert(r1.is_inside(0,0,50,51)  == Region::OUT);
	assert(r1.is_inside(0,0,51,51)  == Region::PART);
	assert(r1.is_inside(0,0,150,150) == Region::PART);
	assert(r1.is_inside(50,50,50,50) == Region::IN);
	assert(r1.is_inside(99,99,50,50) == Region::PART);
	assert(r1.is_inside(99,100,50,50) == Region::OUT);
	assert(r1.is_inside(100,99,50,50) == Region::OUT);
	assert(r1.is_inside(100,100,50,50) == Region::OUT);

	Region r2(r1);
	Area a2(25,25,50,50, true);
	r2.intersect(a2);
	assert(r2.is_inside(0,0,51,50) == Region::OUT);
	assert(r2.is_inside(0,0,50,51) == Region::OUT);
	assert(r2.is_inside(0,0,51,51) == Region::PART);
	assert(r2.is_inside(0,0,150,150) == Region::PART);
	assert(r2.is_inside(25,25,50,50) == Region::PART);
	assert(r2.is_inside(74,74,50,50) == Region::PART);
	assert(r2.is_inside(74,75,50,50) == Region::OUT);
	assert(r2.is_inside(75,74,50,50) == Region::OUT);
	assert(r2.is_inside(75,75,50,50) == Region::OUT);
}


void Region::test2()
{
	Region r1;
	Area a1(50,50,50,50);

	r1.unite(a1);
	assert(r1.is_inside(10,10,10,10) == Region::OUT);
	assert(r1.is_inside(40,40,20,20) == Region::PART);
	assert(r1.is_inside(60,60,10,10) == Region::IN);
}

void Region::test()
{
	test2();
	test1();
}
#endif

