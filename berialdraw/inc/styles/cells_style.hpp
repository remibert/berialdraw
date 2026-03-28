#pragma once
namespace berialdraw
{
	/** The CellsStyle class is a collection container for CellStyle objects. */
	class CellsStyle : public Style
	{
	public:
		/** Constructor */
		CellsStyle();

		/** Destructor */
		virtual ~CellsStyle();

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it) override;

		/** Copy operator */
		CellsStyle& operator=(const CellsStyle& other);

		/** Set properties with another */
		void set(const CellsStyle & other);

		/** Create new cells style */
		static Style * create();

		/** Returns the number of cells in the collection */
		inline uint32_t count() const
		{
			return m_cells.size();
		}

		/** Indicates if index is within bounds */
		inline bool inbound(int32_t index) const
		{
			return m_cells.inbound(index);
		}

		/** Get cell style at the given index */
		inline CellStyle & operator[](int32_t index)
		{
			return m_cells[index];
		}

		/** Get cell style at the given index (const) */
		inline const CellStyle operator[](int32_t index) const
		{
			return m_cells[index];
		}

		/** Add a new cell style to the collection */
		inline void push_back(const CellStyle & cell)
		{
			m_cells.push_back(cell);
		}

		/** Insert a cell style at the given index */
		inline void insert(int32_t index, const CellStyle & cell)
		{
			m_cells.insert(index, cell);
		}

		/** Remove a cell style at the given index */
		inline void remove(int32_t index)
		{
			m_cells.remove(index);
		}

		/** Erase all cells from the collection */
		inline void clear()
		{
			m_cells.clear();
		}

		/** Tests if the collection is empty */
		inline bool empty() const
		{
			return m_cells.empty();
		}

		/** Reserve space for n cells */
		inline void reserve(uint32_t n)
		{
			m_cells.reserve(n);
		}

		/** Get the property name for this style */
		const char* property_name() const override { return "cells"; }

	protected:
/// @cond DOXYGEN_IGNORE
		Vector<CellStyle> m_cells;
/// @endcond
	};
}
