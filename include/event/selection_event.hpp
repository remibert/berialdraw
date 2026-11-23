#pragma once
namespace berialdraw
{
	class Widget;

	/** Event sent when a widget's selected indices change. */
	class SelectionEvent : public Event
	{
	public:
		/** Create a selection change event. */
		SelectionEvent(const Vector<int32_t> & previous_indices, const Vector<int32_t> & indices,
			Widget * widget = 0);

		/** Destroy the selection change event. */
		virtual ~SelectionEvent();

		/** Return the previously selected indices. */
		const Vector<int32_t> & previous_indices() const;

		/** Return the current selected indices. */
		const Vector<int32_t> & indices() const;

		/** Return the event source widget. */
		Widget * widget() const override;

		/** Return the event type identifier. */
		static uint32_t type_id();

		/** Serialize the event for logging. */
		void to_string(String & str) const override;

	private:
		Vector<int32_t> m_previous_indices;
		Vector<int32_t> m_indices;
		Widget * m_widget = 0;
	};
}
