#ifndef LineChart_hpp_INCLUDED
#define LineChart_hpp_INCLUDED

namespace berialdraw
{
	/** Class representing a line chart.
	Inherits from Shape, BorderStyle, and TextStyle. */
	class LineChart: public Shape, public BorderStyle, public TextStyle
	{
	public:
		/** Constructor for LineChart.
		@param canvas Pointer to the canvas object.  */
		LineChart(Canvas * canvas);

		/** Destructor for LineChart. */
		virtual ~LineChart();

		/** Paints the line chart.
		@param shift Point object representing the shift. */
		virtual void paint(const Point & shift);

		/** Returns the polygon of the lines.
		@return Reference to the Polygon object. */
		virtual const Polygon & polygon() const
		{
			return m_lines.polygon();
		}

		/** Adds a data set to the line chart.
		@param datas Reference to the DataSet object. */
		void add(const DataSet & datas);

		/** Adds a data set to the line chart.
		The data set should be destroyed by the line chart destructor.
		@param datas Pointer to the DataSet object. */
		void add(DataSet * datas);

		/** Clears all data sets stored in the line chart. */
		void clear();

		/** Resizes the data sets stored in the line chart.
		@param max Maximum number of data sets. */
		void resize(uint32_t max);

#ifdef _DEBUG
		/** Unitary test for the LineChart class. */
		static void test();
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		/** Computes the space of the chart in the reserved size.
		@param width Reference to the width.
		@param min Reference to the minimum value.
		@param max Reference to the maximum value. */
		void compute_space(uint32_t & width, int32_t & min, int32_t & max);

		/** Computes the lines of the chart. */
		void compute_lines();

		PolyLines m_lines; ///< PolyLines object representing the lines of the chart.
		DataSet ** m_data_sets = 0; ///< Pointer to an array of DataSet objects.
		uint32_t m_count = 0; ///< Current count of data sets stored.
		uint32_t m_max = 0; ///< Maximum number of data sets that can be stored before resizing.
/// @endcond
	};
}
#endif