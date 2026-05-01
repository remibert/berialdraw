#pragma once
namespace berialdraw
{
	/** Class to store data for charts.
	Inherits from BorderStyle and TextStyle. */
	class DataSet : public BorderStyle, public TextStyle
	{
	public:
		/** Default constructor for DataSet. */
		DataSet();
	
		/** Destructor for DataSet. */
		~DataSet();
	
		/** Copy constructor for DataSet.
		@param other Reference to another DataSet object to copy from. */
		DataSet(const DataSet & other);

		/** Clear all data stored */
		void clear();

		/** Add new data in data set */
		void add(int32_t value);

		/** Resize the data set if no enough space */
		void resize(uint32_t new_size);

		/** Return the quantity of data stored */
		inline uint32_t count() const
		{
			return m_count;
		}
		
		/** Get a data at the index */
		int32_t data(uint32_t index) const;

		/** Return the max value in data set */
		int32_t max();

		/** Return the min value in data set */
		int32_t min();
	protected:
/// @cond DOXYGEN_IGNORE
		/** Data set allocated */
		int32_t * m_data = 0;

		/** Current count of data stored */
		uint32_t m_count = 0;

		/** Max data can be stored before resizing */
		uint32_t m_max = 0;
/// @endcond
	};
}
