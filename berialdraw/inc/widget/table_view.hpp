#pragma once
namespace berialdraw
{
	/** The TableView class provides a scrollable table container for displaying widgets in a grid layout.
	Each cell can contain any widget, making it highly flexible for custom content. */
	class TableView : public Widget, public TableViewStyle
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

#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
#endif
	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		TableView& operator=(const TableView& other) = delete;

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region);
		
		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout);

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position);

		ScrollView* m_scroll_view;
		Grid* m_grid;
/// @endcond
	};
}
