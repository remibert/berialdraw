#pragma once
namespace berialdraw
{
	class Widget;

	/** Selection event */
	class SelectEvent : public Event
	{
	public:
		/** Status of selection */
		enum Status
		{
			SELECT_START,    ///< Start of selection 
			SELECT_PROGRESS, ///< Selection is in progress
			SELECT_END,      ///< End of selection
		};

		/** Constructor. 
		@param status selection status
		@param position selection position
		@param widget widget with selection in progress */
		SelectEvent(enum Status status, const Point & position, Widget * widget=0);

		/** Destructor. */
		virtual ~SelectEvent();

		/**  To string.
		 * @param str String to append to. */
		virtual void to_string(String & str) const;

		/** Get type. */
		static uint32_t type_id();

		/** Return the selection status */
		enum Status status() const;

		/** Get the position of selection */
		const Point & position() const;

		/** Return the widget */
		Widget * widget() const;

	private:
/// @cond DOXYGEN_IGNORE
		Point m_position; ///< Selection position.
		Widget * m_widget = 0;
		Status  m_status = Status::SELECT_START; ///< Selection status
/// @endcond
	};
};
