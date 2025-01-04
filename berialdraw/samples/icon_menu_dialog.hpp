#ifndef IconMenuDialog_hpp_INCLUDED
#define IconMenuDialog_hpp_INCLUDED

namespace berialdraw
{
	/** Dialog box to display a menu with a list of icons, a title and a button to cancel */
	class IconMenuDialog : public Dialog
	{
	public:
		/** Create dialog */
		IconMenuDialog();

		/** Add choice in menu */
		Icon * add_choice(const String & text, const String & icon_filename);

		/** Create icon in menu */
		Icon * create_icon(const String & text, const String & icon_filename);

		/** Bind event on a method */
		template<class CLASS, class EVENT> Icon* bind(const String & text, const String & icon_filename, 
			CLASS * object, void (CLASS::*method)(Widget * , const EVENT & ))
		{
			Icon * result = 0;
			if (object && method)
			{
				UIManager::notifier()->bind(new MethodCaller<CLASS,EVENT>(object, method, create_icon(text, icon_filename)));            // Binds the click event to the `on_menu_click` handler
			}
			return result;
		}

		/** Bind event on a function */
		template<class EVENT> Icon* bind(const String & text, const String & icon_filename, void (*function)(Widget *, const EVENT &))
		{
			Icon * result = 0;
			if (function)
			{
				UIManager::notifier()->bind(new FunctionCaller<EVENT>(function, create_icon(text, icon_filename)));            // Binds the click event to the `on_menu_click` handler
			}
			return result;
		}

	protected:
		/** Callback on click on menu item */
		void on_menu_click(Widget * widget, const ClickEvent & evt);

		/** Row layout of icon menu */
		Row * m_row = 0;

		/** Hue for automatic color of icons */
		uint32_t m_color = Color::FIRST_THEME;
	};
}
#endif