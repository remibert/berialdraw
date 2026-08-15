#pragma once

namespace berialdraw
{
	/** The List class provides a scrollable list container for displaying items.
	Supports flexible item creation via text, decorations, and custom configuration callbacks.
	
	PYTHON-STYLE NEGATIVE INDEXING:
	Like Python lists, negative indices count from the end:
	
	    Items:     [A, B, C, D, E]
	    Forward:    0  1  2  3  4    (0=first, 4=last)
	    Backward:  -5 -4 -3 -2 -1    (-1=last, -5=first)
	
	Examples:
	  at(0)   → A (first)
	  at(2)   → C (middle)
	  at(-1)  → E (last)
	  at(-2)  → D (second-to-last)
	  at(-5)  → A (same as at(0))
	  at(10)  → E (out of bounds → clamped to last)
	
	Python equivalent:
	  Python:  mylist[0], mylist[2], mylist[-1], mylist[-2]
	  C++:     list->at(0), list->at(2), list->at(-1), list->at(-2)
	
	Same applies to remove() and insert():
	  remove(-1)       → remove last
	  insert(0, text)  → insert at beginning
	  insert(-1, text) → insert before last */
	class List : public ScrollableContent
	{
	public:
		/** Create widget */
		List(Widget * parent);

		/** Destroy widget */
		virtual ~List();

		/** Create new raw list item at specific index (for advanced customization before adding) */
		ListItem* new_item(int index = UNDEFINED_INDEX);

		/** Append item with text only */
		ListItem* append(const String & text);

		/** Append item with custom configuration via callback function */
		ListItem* append(std::function<void(ListItem*)> config);


		/** Prepend item with text only */
		ListItem* prepend(const String & text);

		/** Prepend item with custom configuration via callback function */
		ListItem* prepend(std::function<void(ListItem*)> config);

		
		/** Insert item at specific index with text only.
		@param index Item position (supports negative indexing)
		@return Pointer to ListItem, or nullptr if index is out of bounds */
		ListItem* insert(int index, const String & text);

		/** Insert item at specific index with custom configuration via callback.
		@param index Item position (supports negative indexing)
		@return Pointer to ListItem, or nullptr if index is out of bounds */
		ListItem* insert(int index, std::function<void(ListItem*)> config);

		
		/** Get item at index (Python-style indexing).
		@param index Item position (supports negative indexing)
		@return Pointer to ListItem, or nullptr if index is out of bounds */
		ListItem* at(int index) const;

		/** Array access operator for list items (Python-style indexing).
		@param index Item position (supports negative indexing)
		@return Pointer to ListItem, or nullptr if index is out of bounds */
		ListItem* operator[](int index) const;

		/** Get number of items in the list */
		size_t count() const;

		/** Check if list is empty */
		bool is_empty() const { return count() == 0; }

		/** Remove item at index (Python-style indexing).
		@param index Position to remove (supports negative indexing) */
		void remove(int index);

		/** Remove all items from the list */
		void clear() override;


		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it) override;

		/** Get the style cascade mode for this widget */
		virtual StyleCascadeMode style_cascade_mode() const override;

		/** Copy all styles of the list */
		void copy(const List& list);

		/** Copy all styles of the list */
		void copy(const List* list);


#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
		static void test6();
		static void test7();
#endif

	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		List& operator=(const List& other) = delete;

		/**
		 * Normalize index to positive range [0, size()-1]
		 * Converts Python-style negative indices to positive indices.
		 * Example with 5 items: -1 → 4, -2 → 3, -5 → 0
		 * Out-of-bounds indices are clamped to valid range.
		 */
		int normalize_index(int index) const;

		/** Internal column container holding all items */
		Column * m_column;
/// @endcond
	};
}
