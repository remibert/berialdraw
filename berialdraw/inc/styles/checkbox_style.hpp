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
		const Size & checkbox_size() const;
		
		/** Set the checkbox box size */
		void checkbox_size(const Size & s);

		/** Set the checkbox box size with width and height in pixels */
		void checkbox_size(Dim w, Dim h);

		/** Set the checkbox box size with a precision of 64th of a pixel */
		void checkbox_size_(Dim w, Dim h);

		/** Get the checkbox padding in pixels */
		Dim check_padding() const;

		/** Set the checkbox padding in pixels */
		void check_padding(Dim pad);


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
		Size m_checkbox_size;
		Dim m_check_padding = 2<<6;
		uint32_t m_check_color = 0xFF000000;
		String m_check_sketch;
/// @endcond
	}; 
}
