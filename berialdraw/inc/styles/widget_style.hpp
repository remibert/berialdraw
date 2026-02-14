#pragma once

namespace berialdraw
{
	/** The WidgetStyle class serves as a base for all widget-specific style definitions. */
	class WidgetStyle : public Style
	{
	public:
		/** Constructor */
		WidgetStyle();

		/** Destructor */
		~WidgetStyle(){}

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Copy operator */
		WidgetStyle& operator=(const WidgetStyle& other);

		/** Set properties with another */
		void set(const WidgetStyle & other);

		/** Create new widget style */
		static Style * create();


		/** Get the row in the grid */
		inline uint16_t row() const
		{
			return m_row;
		}

		/** Get the column in the grid */
		inline uint16_t column() const
		{
			return m_column;
		}

		/** Set the row in the grid */
		void row(uint16_t r);

		/** Set the column in the grid */
		void column(uint16_t c);

		/** Set the cell in the grid */
		void cell(uint16_t r, uint16_t c);


		/** Get the cell size policy */
		inline SizePolicy size_policy() const
		{
			return (SizePolicy)m_size_policy;
		}

		/** Set the cell size policy */
		void size_policy(SizePolicy v);


		/** Get the minsize */
		inline const Size & min_size() const
		{
			return m_min_size;
		}
		
		/** Set the minsize */
		void min_size(const Size & s);

		/** Set the minsize with width and height in pixels */
		void min_size(Dim w, Dim h=0);

		/** Set the minsize with a precision of 64th of a pixel */
		void min_size_(Dim w, Dim h=0);


		/** Get the max size */
		inline const Size & max_size() const
		{
			return m_max_size;
		}
		
		/** Set the max size */
		void max_size(const Size & s);

		/** Set the max size with width and height in pixels */
		void max_size(Dim w, Dim h=0);

		/** Set the max size with a precision of 64th of a pixel */
		void max_size_(Dim w, Dim h=0);


		/** Get the id */
		uint16_t id() const { return (uint16_t)m_id; }

		/** Set the id */
		void id(uint16_t v);


		/** Get the extend */
		Extend extend() const { return (Extend)m_extend; }

		/** Set the extend */
		void extend(Extend v);


		/** Get the pressed */
		bool pressed() const { return m_pressed; }

		/** Set the pressed */
		void pressed(bool v);


		/** Get the pressable */
		bool pressable() const { return m_pressable; }

		/** Set the pressable */
		void pressable(bool v);


		/** Get the checked */
		bool checked() const { return m_checked; }

		/** Set the checked */
		void checked(bool v);


		/** Get the focusable */
		bool focusable() const { return m_focusable; }

		/** Set the focusable */
		void focusable(bool v);


		/** Get the selectable (used for the SelectEvent) */
		bool selectable() const { return m_selectable; }

		/** Set the selectable (used for the SelectEvent) */
		void selectable(bool v);


		/** Get the focused */
		bool focused() const { return m_focused; }

		/** Set the focused */
		void focused(bool v);



#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
#endif


#define UNDEFINED_CELL 0x7FFF
	protected:
/// @cond DOXYGEN_IGNORE
		Size m_min_size = {0,0};
		Size m_max_size = {Size::MAX_SIZE,Size::MAX_SIZE};

		uint16_t m_row    = UNDEFINED_CELL;
		uint16_t m_column = UNDEFINED_CELL;
		uint16_t m_id = 0xFFFF;
		unsigned int m_pressed   :1;
		unsigned int m_pressable :1;
		unsigned int m_checked   :1;

		unsigned int m_focused   :1;
		unsigned int m_focusable :1;

		unsigned int m_selectable:1;

		unsigned int m_edited    :1;
		unsigned int m_refocus   :1;

		unsigned int m_size_policy:3;
		unsigned int m_extend     :2;
		unsigned int m_flow_place :1;
		unsigned int m_flow_in_children:1;
		unsigned int m_flow       :1;
/// @endcond 
	};
}
