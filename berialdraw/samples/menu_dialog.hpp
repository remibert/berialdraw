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

		/** Create button in menu */
		Button * create_button(const String & text);

		/** Bind event on a method */
		template<class CLASS, class EVENT> Button* bind(const String & text, CLASS * object, void (CLASS::*method)(Widget * , const EVENT & ))
		{
			Button * result = 0;
			if (object && method)
			{
				result = create_button(text);
				UIManager::notifier()->bind(new MethodCaller<CLASS,EVENT>(object, method, result));            // Binds the click event to the `on_menu_click` handler
			}
			return result;
		}

		/** Bind event on a function */
		template<class EVENT> Button* bind(const String & text, void (*function)(Widget *, const EVENT &))
		{
			Button * result = 0;
			if (function)
			{
				result = create_button(text);
				UIManager::notifier()->bind(new FunctionCaller<EVENT>(function, result));            // Binds the click event to the `on_menu_click` handler
			}
			return result;
		}

	protected:
		/** Callback on click on menu item */
		void on_menu_click(Widget * widget, const ClickEvent & evt);

		Column * m_column = 0;
	};
}
#endif