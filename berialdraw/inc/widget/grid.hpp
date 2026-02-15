#pragma once
namespace berialdraw
{
	class TableView;

	/** The Grid class arranges child widgets in a grid, enabling precise alignment and consistent spacing. 
	It is ideal for forms, tables, or structured UI designs. */
	class Grid : public Widget
	{
	friend class TableView;
	/** @image html sample_grid_1.svg "example" width=200px height=200px
	@example sample_grid.cpp*/
	public:
		/** Create widget */
		Grid(Widget * parent);

		/** Destroy widget */
		virtual ~Grid();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		/** Get the number of rows */
		Dim row_count() const;

		/** Get the number of columns */
		Dim column_count() const;

#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
		static void test6();
#endif
	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		Grid& operator=(const Grid& other) = delete;
		
		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout);

		/** Return the size of content without marges */
		virtual Size content_size();

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position);

		Cells m_cells;
/// @endcond
	};
}
