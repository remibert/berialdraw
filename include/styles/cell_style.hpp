#pragma once
namespace berialdraw
{
	/** The CellStyle class specifies the appearance of individual cells. */
	class CellStyle : public Style
	{
	public:
		/** Constructor */
		CellStyle();

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it) override;

		/** Copy operator */
		CellStyle& operator=(const CellStyle& other);

		/** Set properties with another */
		void set(const CellStyle & other);

		/** Create new cell style */
		static Style * create();


		/** Get the row selector */
		inline uint16_t row_selector() const
		{
			return m_row_selector;
		}

		/** Set the row selector */
		void row_selector(uint16_t row);


		/** Get the column selector */
		inline uint16_t column_selector() const
		{
			return m_column_selector;
		}

		/** Set the column selector */
		void column_selector(uint16_t col);


		/** Get the property name for this style */
		const char* property_name() const override { return "cell"; }

	protected:
/// @cond DOXYGEN_IGNORE
		uint16_t m_row_selector = 0;
		uint16_t m_column_selector = 0;
/// @endcond
	};
}
