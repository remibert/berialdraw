#include "berialdraw_imp.hpp"

using namespace berialdraw;

Desktop::Desktop()
{
	m_quit = 0;
	m_already_in_dispatch = 0;
}

Desktop::~Desktop()
{
	remove_all();
}

void Desktop::add(Window * window)
{
	m_windows.insert(0,window);
}

/** Places the specified window at the top of its parents canvas stack, above all of its sibling window. */
void Desktop::back(Window * window)
{
	for(uint32_t i = 0; i < m_windows.size();)
	{
		if (m_windows[i] == 0)
		{
			m_windows.remove(i);
		}
		else if(m_windows[i] == window)
		{
			m_windows.remove(i);
			m_windows.push_back(window);
			break;
		}
		else
		{
			i++;
		}
	}
}

/** Places the specified window at the bottom of its parents canvas stack, below all of its sibling window.*/
void Desktop::front(Window * window)
{
	for(uint32_t i = 0; i < m_windows.size(); )
	{
		if (m_windows[i] == 0)
		{
			m_windows.remove(i);
		}
		else if(m_windows[i] == window)
		{
			m_windows.remove(i);
			m_windows.insert(0,window);
			break;
		}
		else
		{
			i++;
		}
	}
}

void Desktop::remove(Window * window)
{
	uint32_t size = m_windows.size();
	for(uint32_t i = 0; i < size; i++)
	{
		if(m_windows[i] == window)
		{
			if(m_windows[i]->is_allocated())
			{
				delete window;
			}
			m_windows.remove(i);
			break;
		}
	}
	size = m_windows.size();
	for(uint32_t i = 0; i < size; i++)
	{
		UIManager::invalidator()->dirty(m_windows[i], Invalidator::REPAINT);
	}
}

void Desktop::remove_all()
{
	for(uint32_t i = 0; i < m_windows.size(); i++)
	{
		if (m_windows[i])
		{
			if(m_windows[i]->is_allocated())
			{
				delete m_windows[i];
				m_windows[i] = 0;
			}
			else
			{
				m_windows[i]->clear();
			}
		}
	}
	m_windows.clear();
}

void Desktop::mainloop()
{
	m_quit = 0;
	while(dispatch() && m_quit == 0)
	{
	}
}

void Desktop::quit()
{
	m_quit = 1;
}

/** Indicates if the mainloop is running */
bool Desktop::is_loop()
{
	return m_quit == 0;
}

bool Desktop::dispatch(const char * snapshot)
{
	bool result;
	
	// Check is test script started
	if (UIManager::notifier()->is_script_started())
	{
		// Goto next step script
		snapshot = UIManager::notifier()->next_step_script();
	}

	// Get event from screen device
	result = UIManager::device()->dispatch();

	// If the screen must be refressed
	if (UIManager::invalidator()->is_dirty() || snapshot != 0)
	{
		// Refresh the display
		refresh(snapshot);
	}

	// Notify all user event to application
	UIManager::notifier()->dispatch();
	return result;
}


void Desktop::refresh(const char * snapshot)
{
	UIManager::renderer()->clear();

	if (m_windows.size())
	{
		// If snapshot required
		if (snapshot && snapshot[0] != '\0')
		{
			// Set svg exporter
			UIManager::exporter(new SvgOut(snapshot));
		}

		// Place all widgets
		for(uint32_t i = 0; i < m_windows.size(); i++)
		{
			Window * window = m_windows[i];
			if (window)
			{
				if(UIManager::invalidator()->is_dirty(window))
				{
					window->place();

					// Flow replacement detected
					if (window->flow_replacement())
					{
						//window->replace_all();
						window->place();
					}
				}
			}
		}

		// Paint all widgets
		for(int32_t i = m_windows.size()-1; i >= 0; i--)
		{
			Window * window = m_windows[i];
			if (window)
			{
				Region region;

				// Create region to clip paint
				for(int32_t j = 0; j < i; j++)
				{
					Window * fore_window = m_windows[j];
					if (fore_window)
					{
						region.unite(fore_window->backclip());
					}
				}

				// Paint window
				window->paint(region);

				// Remove all dirty and invalidate state
				window->clean_all();
			}
		}
	}

	if(UIManager::renderer())
	{
		UIManager::device()->refresh();
		UIManager::invalidator()->clear(0);
#ifdef WIN32
		UIManager::device()->dispatch();
#endif
	}
	UIManager::exporter(0);
	//usleep(10000);
}

/** Get the front window */
Window * Desktop::front()
{
	Window * result = 0;
	if (m_windows.size() > 0)
	{
		result = m_windows[0];
	}
	return result;
}

/** Get the widget hovered */
Widget * Desktop::hovered(const Point & position)
{
	Widget * result = 0;
	Region region;

	// Search window with hovered
	for(uint32_t window_id = 0; window_id < m_windows.size(); window_id++)
	{
		Window * window = m_windows[window_id];
		if (window)
		{
			// Get the widget hovered
			result = m_windows[window_id]->hovered(region, position);

			if (result)
			{
				break;
			}
			// Add the area of this window to region
			region.unite(window->area());
		}
	}
	return result;
}

