#ifndef MenuDialog_hpp_INCLUDED
#define MenuDialog_hpp_INCLUDED

namespace berialdraw
{
	/** Dialog box to display a menu with a list of options, a title and a button to cancel */
	class MenuDialog : public Dialog
	{
	public:
		/** Create dialog */
		MenuDialog();

		/** Add choice in menu */
		Button * add_choice(const String & text);

	protected:
		/** Callback on click on menu item */
		void on_menu_click(Widget * widget, const ClickEvent & evt);

		Column * m_column = 0;
	};
}
#endif