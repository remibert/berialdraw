#pragma once

namespace berialdraw
{
	/** The ScrollbarStyle class specifies the appearance of scrollbars in scrollable widgets. */
	class ScrollbarStyle : public Style
	{
	public:
		/** Constructor */
		ScrollbarStyle();

		/** Serialize the content of style into json */
		void serialize(JsonIterator & it) override;

		/** Unserialize the content of style from json */
		void unserialize(JsonIterator & it) override;

		/** Copy operator */
		ScrollbarStyle& operator=(const ScrollbarStyle& other);

		/** Copy properties from another without invalidation */
		void copy_from(const ScrollbarStyle & other);

		/** Set properties with another */
		void set(const ScrollbarStyle & other);

		/** Create new scrollbar style */
		static Style * create();

		/** Get the property name for this style */
		const char* property_name() const override { return "scrollbar"; }


		/** Get whether the scrollbar is visible */
		bool scrollbar_visible() const { return m_scrollbar_visible; }

		/** Set whether the scrollbar is visible */
		void scrollbar_visible(bool v);


		/** Get the scrollbar thumb color */
		uint32_t scrollbar_thumb_color() const;

		/** Set the scrollbar thumb color */
		void scrollbar_thumb_color(uint32_t c);



		/** Get the scrollbar width in pixels */
		Dim scrollbar_width() const { return m_scrollbar_width >> 6; }

		/** Get the scrollbar width in 64ths of pixels */
		Dim scrollbar_width_() const { return m_scrollbar_width; }

		/** Set the scrollbar width in pixels */
		void scrollbar_width(Dim w);

		/** Set the scrollbar width in 64ths of pixels */
		void scrollbar_width_(Dim w);


		/** Get the scrollbar radius in pixels */
		Dim scrollbar_radius() const { return m_scrollbar_radius >> 6; }

		/** Get the scrollbar radius in 64ths of pixels */
		Dim scrollbar_radius_() const { return m_scrollbar_radius; }

		/** Set the scrollbar radius in pixels */
		void scrollbar_radius(Dim r);

		/** Set the scrollbar radius in 64ths of pixels */
		void scrollbar_radius_(Dim r);


		/** Get the scrollbar margin in pixels */
		Dim scrollbar_margin() const { return m_scrollbar_margin >> 6; }

		/** Get the scrollbar margin in 64ths of pixels */
		Dim scrollbar_margin_() const { return m_scrollbar_margin; }

		/** Set the scrollbar margin in pixels */
		void scrollbar_margin(Dim m);

		/** Set the scrollbar margin in 64ths of pixels */
		void scrollbar_margin_(Dim m);


	protected:
/// @cond DOXYGEN_IGNORE
		uint32_t m_scrollbar_thumb_color = Color::SCROLLBAR_THUMB_COLOR;
		Dim      m_scrollbar_width = 4 << 6;   ///< Default 6 pixels
		Dim      m_scrollbar_radius = 2 << 6;  ///< Default 3 pixels (half of width for round ends)
		Dim      m_scrollbar_margin = 1 << 6;  ///< Default 2 pixels margin
		bool     m_scrollbar_visible = true;
/// @endcond
	};
}
