#pragma once

namespace berialdraw
{
	/** The ComboStyle class defines options specific to combo widgets. */
	class ComboStyle : public Style
	{
	public:
		/** Create a combo style with default values. */
		ComboStyle();

		/** Serialize the combo style to JSON.
		@param it JSON iterator receiving the serialized properties */
		void serialize(JsonIterator & it) override;

		/** Restore the combo style from JSON.
		@param it JSON iterator containing the serialized properties */
		void unserialize(JsonIterator & it) override;

		/** Copy another combo style.
		@param other Style to copy
		@return This style */
		ComboStyle& operator=(const ComboStyle& other);

		/** Copy properties from another combo style.
		@param other Style to copy */
		void set(const ComboStyle & other);

		/** Create a combo style instance.
		@return The created style */
		static Style * create();

		/** Check whether the text field is editable. */
		bool editable() const { return m_editable; }

		/** Enable or disable text editing.
		@param state True to allow editing */
		void editable(bool state);

		/** Get the width of the dropdown button in pixels. */
		uint16_t button_width() const { return m_button_width; }

		/** Set the width of the dropdown button in pixels.
		@param pixels Button width */
		void button_width(uint16_t pixels);

		/** Get the maximum popup height in pixels; zero means no explicit limit. */
		uint16_t max_popup_height() const { return m_max_popup_height; }

		/** Set the maximum popup height in pixels; zero means no explicit limit.
		@param pixels Maximum popup height */
		void max_popup_height(uint16_t pixels);

		/** Get the property name used to load combo styles. */
		const char* property_name() const override { return "combo"; }

	protected:
		/// @cond DOXYGEN_IGNORE
		bool m_editable = true;
		uint16_t m_button_width = 24;
		uint16_t m_max_popup_height = 160;
		/// @endcond
	};
}