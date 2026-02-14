#pragma once
namespace berialdraw
{
	/** The SwitchStyle class customizes the visual properties of switches, including the background, 
	toggle handle. */
	class SwitchStyle : public Style
	{
	public:
		/** Constructor */
		SwitchStyle();
	
		/** Destructor */	
		~SwitchStyle(){}

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Copy operator */
		SwitchStyle& operator=(const SwitchStyle& other);

		/** Set properties with another */
		void set(const SwitchStyle & other);

		/** Create new shape style */
		static Style * create();



		/** Get the on track color */
		uint32_t on_track_color() const { return m_on_track_color; }

		/** Set the on track color */
		void on_track_color(uint32_t col);

		/** Set the on track color with alpha */
		void on_track_color(uint32_t col, uint8_t alpha);


		/** Get the off track color */
		uint32_t off_track_color() const { return m_off_track_color; }

		/** Set the off track color */
		void off_track_color(uint32_t col);

		/** Set the off track color with alpha */
		void off_track_color(uint32_t col, uint8_t alpha);


		/** Get the thumb color */
		uint32_t thumb_color() const { return m_thumb_color; }

		/** Set the thumb color */
		void thumb_color(uint32_t col);

		/** Set the thumb color with alpha */
		void thumb_color(uint32_t col, uint8_t alpha);


		/** Get the size */
		const Size & switch_size() const { return m_switch_size; }
		
		/** Set the switch_size */
		void switch_size(const Size & s);

		/** Set the switch size with width and height in pixels */
		void switch_size(Dim w, Dim h);

		/** Set the switch size with a precision of 64th of a pixel */
		void switch_size_(Dim w, Dim h);


		/** Get the thumb_padding in pixels */
		Dim thumb_padding() const { return m_thumb_padding>>6; }

		/** Set the thumb padding in pixels */
		void thumb_padding(Dim pad);

	protected:
/// @cond DOXYGEN_IGNORE
		Size m_switch_size;
		uint32_t m_on_track_color = 0xFF00DF00;
		uint32_t m_off_track_color = 0xFFDFDFDF;
		uint32_t m_thumb_color   = 0xFFFFFFFF;
		Dim      m_thumb_padding = 1;
/// @endcond
	}; 
}
