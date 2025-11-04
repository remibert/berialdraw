#pragma once
namespace berialdraw
{
	/** The Row class organizes child widgets in a single horizontal line. 
	Widgets can have flexible or fixed widths, making this layout suitable for toolbars or button groups. */
	class Row : public Widget
	{
	/** @image html sample_row_1.svg "example" width=200px height=200px
	@example sample_row.cpp*/
	public:
		/** Create widget */
		Row(Widget * parent);

		/** Destroy widget */
		virtual ~Row();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		/** Get the flow */
		bool flow() const;

		/** Set the flow */
		void flow(bool v);

#ifndef SWIG
	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		Row& operator=(const Row& other) = delete;
		
		/** Rework all widget to follow this layout linear constraint */
		void linear_place();

		/** Rework all widget to follow this layout flow constraint */
		void flow_place(const Area & area);

		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout);

		/** Return the size of content without marges */
		virtual Size content_size();

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position);

		Cells m_cells;
/// @endcond
#endif
	};
}
