#pragma once
namespace berialdraw
{
	/** Dialog box to display a menu with a list of icons, a title and a button to cancel */
	class Dialog
	{
	public:
		/** Create dialog */
		Dialog();

		/** Add title menu */
		void title(const String & title);

		/** Add close button */
		void add_back_button(const String & text);

		/** Execute dialog menu and return the string selected */
		const String & exec();
	
	protected:
		/** Callback on click on menu item */
		void on_menu_quit(Widget * widget, const ClickEvent & evt);

		Window   m_window;
		String   m_selected;
		Pane   * m_header = 0;
		Widget * m_main_layout = 0;
		Widget * m_content = 0;
	};
}
