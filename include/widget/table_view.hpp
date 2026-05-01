#pragma once
namespace berialdraw
{
	/** The TableView class provides a scrollable table container for displaying widgets in a grid layout.
	Each cell can contain any widget, making it highly flexible for custom content. */
	class TableView : public ScrollableContent, public GridStyle, public TableViewStyle
	{
	public:
		/** Create widget */
		TableView(Widget * parent);

		/** Destroy widget */
		virtual ~TableView();

		/** Set a widget in a specific cell */
		void set_widget(uint16_t row, uint16_t column, Widget* widget);

		/** Get the widget at a specific cell */
		Widget* get_widget(uint16_t row, uint16_t column);

		/** Remove the widget at a specific cell */
		void remove_widget(uint16_t row, uint16_t column);

		/** Clear all widgets from the table */
		void clear();

		/** Get the number of rows */
		Dim row_count() const;

		/** Get the number of columns */
		Dim column_count() const;

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		/** Load table data from JSON stream (can throw JsonUnserializeError)
		@param stream TextStream containing JSON data */
		void load(TextStream& stream);

		/** Save table data to JSON stream
		@param stream TextStream to write JSON data */
		void save(TextStream& stream);

		/** Return the size of content without marges */
		virtual Size content_size();

		/** Get the actual parent for child widgets (redirect to internal grid) */
		virtual Widget * get_parent_for_child()
		{
			return m_grid;
		}

#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
		static void test6();
		static void test7();
		static void test8();
		static void test9();
#endif
	protected:
/// @cond DOXYGEN_IGNORE
		/** Load JSON data from stream */
		void load_json_from_stream(TextStream& stream);

		/** Populate Json object with table data */
		void populate_json_from_data(Json& json);

		/** Remove operator = */
		TableView& operator=(const TableView& other) = delete;

		/** Paint on screen memory the content of this widget (adds table-specific rendering) */
		virtual void paint(const Region & parent_region);

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position);

		/** Paint alternating row backgrounds */
		void paint_row_backgrounds();

		/** Paint grid lines and borders */
		void paint_grid_lines();

		/** Paint top border line */
		void paint_top_border_line();

		/** Paint left border line */
		void paint_left_border_line();

		/** Paint horizontal grid lines */
		void paint_horizontal_lines();

		/** Paint vertical grid lines */
		void paint_vertical_lines();

		Grid* m_grid;
		Size m_content_size;
		uint8_t m_table_view_modified;
/// @endcond
	};
}
