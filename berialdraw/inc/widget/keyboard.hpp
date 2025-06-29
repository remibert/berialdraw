#ifndef Keyboard_hpp_INCLUDED
#define Keyboard_hpp_INCLUDED
namespace berialdraw
{
	/** The Keyboard class represents a virtual keyboard for user input. 
	It supports customizable layouts, key mappings, and is often used in touch-based interfaces. */
	class Keyboard: public Widget, public TextStyle, public BorderStyle
	{
	/** @image html sample_keyboard_1.svg "example" width=200px height=200px
	@example sample_keyboard.cpp*/
	public:
		/** Create widget */
		Keyboard(Widget * parent);

		/** Destroy widget */
		virtual ~Keyboard();

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		/** Copy all styles of the keyboard */
		void copy(const Keyboard & keyboard);

	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		Keyboard& operator=(const Keyboard& other) = delete;

		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout);

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position);

		/** Internal grid for all keys */
		Grid * m_grid = 0;

		/** Informations on keys styles */
		const Keys   * m_keys = 0;

		/** Informations on all mappings */
		const Mappings * m_mappings = 0;

		/** Current mapping selected */
		const Mapping * m_mappingSelected = 0;

		/** Create a mapping according to its name */
		void create(const String & name);

		/** Call back on click key */
		void on_touch_key(Widget * widget, const TouchEvent & evt);
/// @endcond

#ifdef _DEBUG
	public:
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
		static void test6();
		static void test7();
		static void test8();
#endif
	};
}
#endif