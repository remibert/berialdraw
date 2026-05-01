#pragma once
namespace berialdraw
{
	/** The TableViewStyle class specifies the appearance of table views, including grid colors, 
	thickness, header properties, and alternating row colors for improved readability. */
	class TableViewStyle : public Style
	{
	public:
		/** Constructor */
		TableViewStyle();

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it) override;

		/** Copy operator */
		TableViewStyle& operator=(const TableViewStyle& other);

		/** Set properties with another */
		void set(const TableViewStyle & other);

		/** Create new table view style */
		static Style * create();


		/** Get the alternating row color 1 */
		inline uint32_t alternating_row_color1() const
		{
			return m_alternating_row_color1;
		}

		/** Set the alternating row color 1 */
		void alternating_row_color1(uint32_t col);

		/** Set the alternating row color 1 with alpha */
		void alternating_row_color1(uint32_t col, uint8_t alpha);


		/** Get the alternating row color 2 */
		inline uint32_t alternating_row_color2() const
		{
			return m_alternating_row_color2;
		}

		/** Set the alternating row color 2 */
		void alternating_row_color2(uint32_t col);

		/** Set the alternating row color 2 with alpha */
		void alternating_row_color2(uint32_t col, uint8_t alpha);


		/** Get the property name for this style */
		const char* property_name() const override { return "tableview"; }

protected:
/// @cond DOXYGEN_IGNORE
	uint32_t m_alternating_row_color1 = 0xFFFFFFFF;
	uint32_t m_alternating_row_color2 = 0xFFF0F0F0;
/// @endcond
	};
}
