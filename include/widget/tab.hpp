#pragma once
namespace berialdraw
{
	/** A button used as a tab header, with border and margin rotation by position. */
	class Tab : public Button
	{
	public:
		/** Create a tab button using the dedicated "tab" style. */
		Tab(Widget * parent);

		/** Get the position this tab style is oriented for. */
		TabPosition position() const { return m_position; }

		/** Reapply the base tab style and orient its margin and borders. */
		void position(TabPosition position);

		/** Rotate a margin clockwise for a tab position, preserving Q6 precision. */
		static Margin rotate_margin(const Margin & margin, TabPosition position);

		/** Rotate border sides clockwise for a tab position. */
		static Borders rotate_borders(Borders borders, TabPosition position);

		/** Rotate widget alignment clockwise for a tab position. */
		static Align rotate_align(Align align, TabPosition position);

	private:
		TabPosition m_position = TabPosition::TOP;
	};
}