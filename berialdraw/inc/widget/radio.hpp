#pragma once
namespace berialdraw
{
	/** The RadioGroup class manages a group of radio buttons where only one can be selected at a time.
	When a radio button in the group is selected, all others are automatically deselected. */
	class RadioGroup
	{
	public:
		/** Constructor */
		RadioGroup();

		/** Destructor */
		~RadioGroup();

		/** Add a radio button to the group */
		void add_radio(class Radio * radio);

		/** Remove a radio button from the group */
		void remove_radio(class Radio * radio);

		/** Select a specific radio button in the group, deselecting others */
		void select(class Radio * radio);

		/** Get the currently selected radio button */
		class Radio * selected() const;

	protected:
/// @cond DOXYGEN_IGNORE
		std::vector<class Radio *> m_radios;
		class Radio * m_selected = nullptr;
/// @endcond
	};

	/** The Radio class represents a selectable radio button control used for mutually exclusive selections.
	Radio buttons are grouped together, and only one can be selected at a time within the same group. */
	class Radio: public Widget, public BorderStyle, public RadioStyle
	{
		friend class RadioGroup;
	/** @image html sample_radio_1.svg "example" width=200px height=200px
	@example sample_radio.cpp*/
	public:
		/** Create widget */
		Radio(Widget * parent);

		/** Destroy widget */
		virtual ~Radio();

		/** Set the radio group for this button */
		void set_group(RadioGroup * group);

		/** Get the radio group for this button */
		RadioGroup * get_group() const;

		/** Check if radio button is selected */
		bool is_selected() const;

		/** Serialize the content of widget into json */
		virtual void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		virtual void unserialize(JsonIterator & it);

		/** Return the size of content without marges */
		virtual Size content_size();

		/** Copy all styles of the radio button */
		void copy(const Radio & radio_);

		/** Copy all styles of the radio button */
		void copy(const Radio * radio_);
#ifdef _DEBUG
		static void test();
		static void test1();
#endif
	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		Radio& operator=(const Radio& other) = delete;

		/** Call back on key */
		void on_key(Widget * widget, const KeyEvent & evt);

		/** Call back on click */
		void on_click(Widget * widget, const ClickEvent & evt);

		/** Paint on screen memory the content of this widget */
		virtual void paint(const Region & parent_region);

		/** Place all widget in area */
		virtual void place(const Area & area, bool in_layout);

		/** Get the widget hovered */
		virtual Widget * hovered(const Region & parent_region, const Point & position);

		RadioGroup * m_group = nullptr;
		bool m_selected = false;
/// @endcond
	};
}
