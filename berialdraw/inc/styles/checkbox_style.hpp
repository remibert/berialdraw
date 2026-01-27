#pragma once
namespace berialdraw
{
	/** The CheckboxStyle class customizes the visual properties of checkboxes, including the checkbox box,
	check mark, colors, padding and other styling elements. */
	class CheckboxStyle : public Style
	{
	public:
		/** Constructor */
		CheckboxStyle();
	
		/** Destructor */	
		~CheckboxStyle(){}

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Copy operator */
		CheckboxStyle& operator=(const CheckboxStyle& other);

		/** Set properties with another */
		void set(const CheckboxStyle & other);

		/** Create new checkbox style */
		static Style * create();



		// Checkbox box properties

		/** Get the checkbox box size */
		const Size & check_box_size() const;
		
		/** Set the checkbox box size */
		void check_box_size(const Size & s);

		/** Set the checkbox box size with width and height in pixels */
		void check_box_size(Dim w, Dim h);

		/** Set the checkbox box size with a precision of 64th of a pixel */
		void check_box_size_(Dim w, Dim h);


		/** Get the checkbox box thickness in pixels */
		Dim check_box_thickness() const;

		/** Set the checkbox box thickness in pixels */
		void check_box_thickness(Dim thickness);


		/** Get the checkbox box radius in pixels */
		Dim check_box_radius() const;

		/** Set the checkbox box radius in pixels */
		void check_box_radius(Dim radius);


		/** Get the checkbox box padding in pixels */
		Dim check_box_padding() const;

		/** Set the checkbox box padding in pixels */
		void check_box_padding(Dim pad);

		/** Get the check box color */
		uint32_t check_box_color() const;

		/** Set the check box color */
		void check_box_color(uint32_t col);

		/** Set the check box color with alpha */
		void check_box_color(uint32_t col, uint8_t alpha);


		// Check mark properties

		/** Get the check mark color */
		uint32_t check_color() const;

		/** Set the check mark color */
		void check_color(uint32_t col);

		/** Set the check mark color with alpha */
		void check_color(uint32_t col, uint8_t alpha);


		/** Get the check mark sketch (VectorScript) */
		const String & check_sketch() const;

		/** Set the check mark sketch (VectorScript) */
		void check_sketch(const String & sketch);

	protected:
/// @cond DOXYGEN_IGNORE
		Size m_check_box_size;
		Dim m_check_box_thickness = 2;
		Dim m_check_box_radius = 2;
		Dim m_check_box_padding = 2;
		uint32_t m_check_color = 0xFF000000;
		uint32_t m_check_box_color = 0xFFFFFFF;
		String m_check_sketch;
/// @endcond
	}; 
}
