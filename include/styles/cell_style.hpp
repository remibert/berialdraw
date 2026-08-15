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
		inline Dim row_selector() const
		{
			return m_row_selector;
		}

		/** Set the row selector */
		void row_selector(Dim row);


		/** Get the column selector */
		inline Dim column_selector() const
		{
			return m_column_selector;
		}

		/** Set the column selector */
		void column_selector(Dim col);


		/** Get the property name for this style */
		const char* property_name() const override { return "cell"; }

	protected:
/// @cond DOXYGEN_IGNORE
		Dim m_row_selector = 0;
		Dim m_column_selector = 0;
/// @endcond
	};
}
