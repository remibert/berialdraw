#ifndef ProgressBarStyle_hpp_INCLUDED
#define ProgressBarStyle_hpp_INCLUDED

namespace berialdraw
{
	/** The ProgressBarStyle class specifies the appearance of progress bars, including the bar's color, 
	track color, thickness. */
	class ProgressBarStyle : public Style
	{
	public:
		/** Constructor */
		ProgressBarStyle();
	
		/** Destructor */
		~ProgressBarStyle(){}

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Set properties with another */
		void set(const ProgressBarStyle & other);

		/** Indicates if the window must be refreshed */
		bool is_dirty();

		/** Create new shape style */
		static Style * create();


		/** Get the track color */
		uint32_t track_color() const;

		/** Set the track color */
		void track_color(uint32_t col);

		/** Set the track color with alpha */
		void track_color(uint32_t col, uint8_t alpha);


		/** Get the fill color */
		uint32_t fill_color() const;

		/** Set the fill color */
		void fill_color(uint32_t col);

		/** Set the fill color with alpha */
		void fill_color(uint32_t col, uint8_t alpha);


		/** Get the fill size */
		Dim fill_size() const;

		/** Set the fill size in pixels */
		void fill_size(Dim size_);

		/** Set the fill size with a precision of 64th of a pixel */
		void fill_size_(Dim size_);


		/** Get the track size */
		Dim track_size() const;

		/** Set the track size in pixels */
		void track_size(Dim size_);

		/** Set the track size with a precision of 64th of a pixel */
		void track_size_(Dim size_);


		/** Get the value of progress_bar */
		int32_t value() const;

		/** Set the value of progress_bar */
		void value(int32_t val);

		/** Get the min value of progress_bar */
		int32_t min_value() const;

		/** Set the min value of progress_bar */
		void min_value(int32_t val);

		/** Get the max value of progress_bar */
		int32_t max_value() const;

		/** Set the max value of progress_bar */
		void max_value(int32_t val);

		/** Get the step value of progress_bar */
		uint32_t step_value() const;

		/** Set the step value of progress_bar */
		void step_value(uint32_t val);

	protected:
/// @cond DOXYGEN_IGNORE
		void check_progress_bar();
		uint32_t m_track_color  = 0xFF7D7D7D;
		Dim      m_track_size   = 5<<6;
		uint32_t m_fill_color = 0xFFFFFFFF;
		Dim      m_fill_size  = 20<<6;
		int32_t  m_value        = 0;
		int32_t  m_max_value    = 100;
		int32_t  m_min_value    = 0;
		uint32_t  m_step_value   = 1;
/// @endcond
	}; 
}
#endif
