#ifndef Region_hpp_INCLUDED
#define Region_hpp_INCLUDED

namespace berialdraw
{
	class Renderer;
	class Desktop;

	/** The Region class is responsible for defining and applying clipping regions.
	Clipping is used to restrict drawing operations to a specific area, 
	ensuring that rendering is performed only within the defined boundaries.*/
	class Region
	{
	public:
		/** Type of overlap */
		typedef enum
		{
			OUT, ///< No point of the rectangle resides within the region
			IN, ///< The region contains the entire rectangle
			PART ///< The region contains part of the rectangle
		} Overlap;

		/** Create empty region */
		Region();

		/** Create region with area */
		Region(const Area & area);

		/** Create new region with a copy of another region */
		Region(const Region & other);

		/** Set the current region with the content of another region */
		Region& operator=(const Region& other);

		/** Destroy region */
		~Region();

		/** Modify the current region with the intersecting areas */
		bool intersect(const Region & reg);

		/** Add the areas at the current region */
		bool unite(const Region & reg);

		/** Substract area to the current region */
		bool subtract(const Region & reg);

		/** Test if a region contains a rectangle */
		Overlap is_inside(Coord x,Coord y,Dim width,Dim height, bool pixel=true) const;

		/** Test if a region contains a point */
		Overlap is_inside(const Point & position) const;

		/** Test if a region contains a rectangle */
		Overlap is_inside(const Point & position, const Size & size) const;

		/** Test if a region contains a point (for renderer) */
		bool is_inside_scale(Coord x,Coord y, Dim scale) const;

		/** Test if a region contains a rectangle (for rendeder) */
		Overlap is_inside_scale(Coord x,Coord y,Dim width,Dim height, Dim scale) const;

		/** Clear the content of the region */
		void clear();

#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
#endif
	protected:
/// @cond DOXYGEN_IGNORE
		struct RegionData 
		{
			int32_t size;
			int32_t num_rects;
		};

		struct RegionRectangle
		{
			Coord x;
			Coord y;
			Dim width;
			Dim height;
		};

		struct RegionBox
		{
			Coord x1, y1, x2, y2;
		};

		struct RegionBoxes
		{
			struct RegionBox  extents;
			struct RegionData *data;
		};

		void dump(const String & name, const RegionBoxes & reg) ;

		typedef bool (Region::*overlap_function) (RegionBoxes * region, RegionBox *r1, RegionBox *r1_end, RegionBox * r2, RegionBox * r2_end, Coord y1, Coord y2);
		bool intersect_operation         (RegionBoxes * region, RegionBox *r1, RegionBox *r1_end, RegionBox * r2, RegionBox * r2_end, Coord y1, Coord y2);
		bool union_operation           (RegionBoxes * region, RegionBox *r1, RegionBox *r1_end, RegionBox * r2, RegionBox * r2_end, Coord y1, Coord y2);
		bool subtract_operation         (RegionBoxes * region, RegionBox *r1, RegionBox *r1_end, RegionBox * r2, RegionBox * r2_end, Coord y1, Coord y2);


		bool append_non        (RegionBoxes * region, RegionBox *r1, RegionBox * r_end, Coord y1, Coord y2);
		inline bool is_good_rect(RegionBox * rect);
		inline bool is_bad_rect(RegionBox * rect);
		inline bool is_broken_data(RegionBoxes * reg);
		inline bool is_empty(const RegionBoxes * reg) const;
		inline int32_t num_rects(const RegionBoxes * reg) const;
		inline int32_t get_size(RegionBoxes * reg);
		inline RegionBox * region_rects(RegionBoxes * reg);
		inline RegionBox * box_pointer(const RegionBoxes * reg) const;
		inline RegionBox * get_box(RegionBoxes * reg, int32_t i);
		inline const RegionBox * get_box(const RegionBoxes * reg, int32_t i);
		inline RegionBox * get_next_box(RegionBoxes * reg);
		inline RegionBox * get_last_box(RegionBoxes * reg);
		inline bool extent_check(const RegionBox * r1, const RegionBox * r2) const;
		inline bool is_in_box(const RegionBox * r, Coord x, Coord y);
		inline bool is_contains(const RegionBox * r1, const RegionBox * r2) const;
		inline int32_t get_alloc_size (int32_t n);
		RegionData * alloc_data (int32_t n);
		void destroy(RegionBoxes & reg);
		bool rect_alloc (RegionBoxes * & region, int32_t n);
		void rect_allocate(RegionBoxes *region, int32_t n);
		void new_rect(RegionBoxes *region, RegionBox * & next_rect, Coord nx1, Coord ny1, Coord nx2, Coord ny2);
		bool is_equal (RegionBoxes *reg1, RegionBoxes *reg2);
		void create (RegionBoxes *region);
		void init_rect (RegionBoxes *	region, Coord x, Coord y, Dim width, Dim height);
		void init_with_extents (RegionBoxes *region, RegionBox *extents);
		bool region_break (RegionBoxes *region);
		bool copy(RegionBoxes *dst, RegionBoxes *src);
		int32_t coalesce (RegionBoxes * region, int32_t prev_start, int32_t cur_start);
		void find_band(RegionBox * r, RegionBox * & r_band_end, RegionBox * & r_end, Coord & ry1);
		void append_region(RegionBoxes * new_reg, RegionBox * r, RegionBox * r_end);
		bool operate (RegionBoxes * new_reg, RegionBoxes * reg1, RegionBoxes * reg2, overlap_function overlap_func, bool append_non1, bool append_non2);
		void set_extents (RegionBoxes *region);

		bool intersect(RegionBoxes * new_reg, RegionBoxes * reg1, RegionBoxes * reg2);

		void merge_rect(RegionBoxes * & region, RegionBox * & r, RegionBox * & next_rect, Coord & x1, Coord & y1, Coord & x2, Coord & y2);
		bool intersect_rect (RegionBoxes *dest, RegionBoxes *source, Coord x, Coord y, Dim width, Dim height);
		bool union_rect (RegionBoxes *dest, RegionBoxes *source, Coord x, Coord y, Dim width, Dim height);

		bool unite (RegionBoxes*new_reg, RegionBoxes*reg1, RegionBoxes*reg2);
		bool subtract (RegionBoxes *reg_d, RegionBoxes *reg_m, RegionBoxes *reg_s);
		bool inverse (RegionBoxes *new_reg, RegionBoxes *reg1, RegionBox *  inv_rect);
		const RegionBox * find_box_for_y (const RegionBox *begin, const RegionBox *end, Coord y) const;
		Region::Overlap is_contains_rectangle (const RegionBoxes * region, RegionBox *   prect) const;
		void translate (RegionBoxes *region, Coord x, Coord y);
		void reset (RegionBoxes *region, RegionBox *box);
		void clear (RegionBoxes *region);
		bool is_contains_point (RegionBoxes * region, Coord x, Coord y, RegionBox * box);

		static const Region::RegionBox EMPTY_BOX;
		static const Region::RegionData EMPTY_DATA;
		static const Region::RegionData BROKEN_DATA;

		static Region::RegionBox *g_empty_box;
		static Region::RegionData *g_empty_data;
		static Region::RegionData *g_broken_data;

		RegionBoxes m_region;
/// @endcond
	};
}
#endif
