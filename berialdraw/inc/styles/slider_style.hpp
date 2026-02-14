#pragma once
namespace berialdraw
{
	/** The SliderStyle class defines the visual appearance of sliders, including the track color, 
	handle shape and size. */
	class SliderStyle : public Style
	{
	public:
		/** Constructor */
		SliderStyle();

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Copy operator */
		SliderStyle& operator=(const SliderStyle& other);

		/** Set properties with another */
		void set(const SliderStyle & other);

		/** Create new shape style */
		static Style * create();


		/** Get the track color */
		uint32_t track_color() const { return m_track_color; }

		/** Set the track color */
		void track_color(uint32_t col);

		/** Set the track color with alpha */
		void track_color(uint32_t col, uint8_t alpha);


		/** Get the handle color */
		uint32_t handle_color() const { return m_handle_color; }

		/** Set the handle color */
		void handle_color(uint32_t col);

		/** Set the handle color with alpha */
		void handle_color(uint32_t col, uint8_t alpha);


		/** Get the handle size */
		Dim handle_size() const { return m_handle_size>>6; }

		/** Set the handle size in pixels */
		void handle_size(Dim size_);

		/** Set the handle size with a precision of 64th of a pixel */
		void handle_size_(Dim size_);


		/** Get the track size */
		Dim track_size() const { return m_track_size>>6; }

		/** Set the track size in pixels */
		void track_size(Dim size_);

		/** Set the track size with a precision of 64th of a pixel */
		void track_size_(Dim size_);


		/** Get the value of slider */
		int32_t value() const { return m_value; }

		/** Set the value of slider */
		void value(int32_t val);


		/** Get the min value of slider */
		int32_t min_value() const { return m_min_value; }

		/** Set the min value of slider */
		void min_value(int32_t val);


		/** Get the max value of slider */
		int32_t max_value() const { return m_max_value; }

		/** Set the max value of slider */
		void max_value(int32_t val);


		/** Get the step value of slider */
		uint32_t step_value() const { return m_step_value; }

		/** Set the step value of slider */
		void step_value(uint32_t val);

	protected:
/// @cond DOXYGEN_IGNORE
		void check_slider();
		uint32_t m_track_color  = 0xFF7D7D7D;
		Dim      m_track_size   = 5<<6;
		uint32_t m_handle_color = 0xFFFFFFFF;
		Dim      m_handle_size  = 20<<6;
		int32_t  m_value        = 0;
		int32_t  m_max_value    = 100;
		int32_t  m_min_value    = 0;
		uint32_t  m_step_value   = 1;
/// @endcond DOXYGEN_IGNORE
	}; 
}
