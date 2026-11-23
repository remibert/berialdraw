#pragma once
namespace berialdraw
{
	class Widget;

	/** Event sent when a text value changes. */
	class TextEvent : public Event
	{
	public:
		/** Create a text change event. */
		TextEvent(const String & previous_text, const String & text, Widget * widget = 0);

		/** Destroy the text change event. */
		virtual ~TextEvent();

		/** Return the previous text. */
		const String & previous_text() const;

		/** Return the current text. */
		const String & text() const;

		/** Return the event source widget. */
		Widget * widget() const override;

		/** Return the event type identifier. */
		static uint32_t type_id();

		/** Serialize the event for logging. */
		void to_string(String & str) const override;

	private:
		String m_previous_text;
		String m_text;
		Widget * m_widget = 0;
	};
}
