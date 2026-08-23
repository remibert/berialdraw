#pragma once
namespace berialdraw
{
	/** The ListStyle class specifies the appearance and behavior of list widgets */
	class ListStyle : public Style
	{
	public:
		/** Constructor */
		ListStyle();

		/** Serialize the content of list into json */
		void serialize(JsonIterator & it) override;

		/** Unserialize the content of list from json */
		void unserialize(JsonIterator & it) override;

		/** Copy operator */
		ListStyle& operator=(const ListStyle& other);

		/** Set properties with another */
		void set(const ListStyle & other);

		/** Create new list style */
		static Style * create();


		/** Get the selection mode */
		inline ListSelectionMode selection_mode() const
		{
			return (ListSelectionMode)m_selection_mode;
		}

		/** Set the selection mode */
		void selection_mode(ListSelectionMode mode);


		/** Get the property name for this style */
		const char* property_name() const override { return "list"; }

	protected:
/// @cond DOXYGEN_IGNORE
		unsigned int m_selection_mode : 2;  // 0=no, 1=single, 2=multi
/// @endcond 
	};
}
