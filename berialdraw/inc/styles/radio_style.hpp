#pragma once
namespace berialdraw
{
	/** The RadioStyle class customizes the visual properties of radio buttons, including the radio box,
	indicator, colors, padding and other styling elements. */
	class RadioStyle : public Style
	{
	public:
		/** Constructor */
		RadioStyle();

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Copy operator */
		RadioStyle& operator=(const RadioStyle& other);

		/** Set properties with another */
		void set(const RadioStyle & other);

		/** Create new radio style */
		static Style * create();

		/** Get the radio group name */
		const String & group() const { return m_group; }

		/** Set the radio group name */
		void group(const String & g);



		// Radio box properties

		/** Get the radio box size */
		const Size & radio_size() const { return m_radio_size; }
		
		/** Set the radio box size */
		void radio_size(const Size & s);

		/** Set the radio box size with width and height in pixels */
		void radio_size(Dim w, Dim h);

		/** Set the radio box size with a precision of 64th of a pixel */
		void radio_size_(Dim w, Dim h);

		/** Get the radio padding in pixels */
		Dim radio_padding() const { return m_radio_padding >> 6; }

		/** Set the radio padding in pixels */
		void radio_padding(Dim pad);


		// Radio indicator properties

		/** Get the radio indicator color */
		uint32_t radio_color() const { return m_radio_color; }

		/** Set the radio indicator color */
		void radio_color(uint32_t col);

		/** Get the radio indicator sketch (VectorScript) */
		const String & radio_sketch() const { return m_radio_sketch; }

		/** Set the radio indicator sketch (VectorScript) */
		void radio_sketch(const String & sketch);

	protected:
/// @cond DOXYGEN_IGNORE
		Size m_radio_size;	Dim m_radio_padding = 2<<6;		uint32_t m_radio_color = 0xFF000000;
		String m_radio_sketch;
		String m_group;
/// @endcond
	}; 
}
