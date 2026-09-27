#pragma once
namespace berialdraw
{
	/** Dialog box to display a menu with a list of options, a title and a button to cancel */
	class MenuDialog : public Dialog
	{
	public:
		/** Create dialog */
		MenuDialog();

		/** Create item in menu */
		ListItem * create_menu(const String & text, const String & icon_filename = "");

		/** Bind event on a method */
		template<class CLASS, class EVENT> ListItem* bind(const String& text, const String& icon_filename,
			CLASS* object, void (CLASS::* method)(Widget*, const EVENT&))
		{
			ListItem* result = 0;
			if (object && method)
			{
				result = create_menu(text, icon_filename);
				UIManager::notifier()->bind(new MethodCaller<CLASS, EVENT>(object, method, result));            // Binds the click event to the `on_menu_click` handler
			}
			return result;
		}

		/** Bind event on a function */
		template<class EVENT> ListItem* bind(const String& text, const String& icon_filename, void (*function)(Widget*, const EVENT&))
		{
			ListItem* result = 0;
			if (function)
			{
				result = create_menu(text, icon_filename);
				UIManager::notifier()->bind(new FunctionCaller<EVENT>(function, result));            // Binds the click event to the `on_menu_click` handler
			}
			return result;
		}


	protected:
		/** Callback on click on menu item */
		void on_menu_click(Widget * widget, const ClickEvent & evt);

		List* m_list = 0;
	};
}
