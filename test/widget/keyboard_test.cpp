#include "berialdraw_imp.hpp"
using namespace berialdraw;

class KeyboardTest1
{
public:
	KeyboardTest1() : 
		m_window(),
		m_grid(&m_window),
		m_label(&m_grid),
		m_keyboard(&m_grid)
	{
		m_window.size(480,240);
		m_window.position(0,0);
		m_keyboard.cell(1,0);
		m_label.cell(0,0);
		m_label.size_policy(SizePolicy::SHRINK_HEIGHT);
		m_label.text("");
		m_label.id(123);
		m_label.align(Align::ALIGN_LEFT);
		m_keyboard.bind(this, &KeyboardTest1::on_key);
	}

	void on_key(Widget * widget, const KeyEvent & evt)
	{
		if (evt.state() == KeyEvent::KEY_DOWN)
		{
			if (evt.key() == (wchar_t)ReservedKey::KEY_BACKSPACE)
			{
				m_string.remove(-1);
			}
			else
			{
				m_string += evt.key();
			}
		}
		m_label.text(m_string);
		m_received = true;
	}

	bool received()
	{
		bool result = m_received;
		m_received = false;
		return result;
	}

	const String & get_string()
	{
		return m_string;
	}

	Window   m_window;
	Grid     m_grid;
	Keyboard m_keyboard;
	Label    m_label;
	String   m_string;
	bool     m_received = true;
};

void Keyboard::test1()
{
	// MemoryLeakTracer::stat();
	KeyboardTest1 t1;
	const struct Key
	{
		Coord x;
		Coord y;
		TouchEvent::TouchState state;
	}
	
	events [] =
	{
		{33 ,208,TouchEvent::TOUCH_DOWN},
		{33 ,208,TouchEvent::TOUCH_UP  },
		{36 ,211,TouchEvent::TOUCH_DOWN},
		{36 ,211,TouchEvent::TOUCH_UP  },
		{22 ,155,TouchEvent::TOUCH_DOWN},
		{22 ,155,TouchEvent::TOUCH_UP  },
		{292,157,TouchEvent::TOUCH_DOWN},
		{292,157,TouchEvent::TOUCH_UP  },
		{22 ,155,TouchEvent::TOUCH_DOWN},
		{22 ,155,TouchEvent::TOUCH_UP  },
		{406,46 ,TouchEvent::TOUCH_DOWN},
		{406,46 ,TouchEvent::TOUCH_UP  },
		{342,157,TouchEvent::TOUCH_DOWN},
		{342,157,TouchEvent::TOUCH_UP  },
		{309,104,TouchEvent::TOUCH_DOWN},
		{309,104,TouchEvent::TOUCH_UP  },
		{407,47 ,TouchEvent::TOUCH_DOWN},
		{407,47 ,TouchEvent::TOUCH_UP  },
		{311,48 ,TouchEvent::TOUCH_DOWN},
		{311,48 ,TouchEvent::TOUCH_UP  },
		{163,53 ,TouchEvent::TOUCH_DOWN},
		{163,53 ,TouchEvent::TOUCH_UP  },
		{33 ,207,TouchEvent::TOUCH_DOWN},
		{33 ,207,TouchEvent::TOUCH_UP  },
		{146,159,TouchEvent::TOUCH_DOWN},
		{146,159,TouchEvent::TOUCH_UP  },
		{36 ,205,TouchEvent::TOUCH_DOWN},
		{36 ,205,TouchEvent::TOUCH_UP  },
		{212,206,TouchEvent::TOUCH_DOWN},
		{212,206,TouchEvent::TOUCH_UP  },
		{218,49 ,TouchEvent::TOUCH_DOWN},
		{218,49 ,TouchEvent::TOUCH_UP  },
		{312,51 ,TouchEvent::TOUCH_DOWN},
		{312,51 ,TouchEvent::TOUCH_UP  },
		{225,213,TouchEvent::TOUCH_DOWN},
		{225,213,TouchEvent::TOUCH_UP  },
		{240,158,TouchEvent::TOUCH_DOWN},
		{240,158,TouchEvent::TOUCH_UP  },
		{22 ,103,TouchEvent::TOUCH_DOWN},
		{22 ,103,TouchEvent::TOUCH_UP  },
		{72 ,103,TouchEvent::TOUCH_DOWN},
		{72 ,103,TouchEvent::TOUCH_UP  },
		{218,209,TouchEvent::TOUCH_DOWN},
		{218,209,TouchEvent::TOUCH_UP  },
		{290,158,TouchEvent::TOUCH_DOWN},
		{290,158,TouchEvent::TOUCH_UP  },
		{360,58 ,TouchEvent::TOUCH_DOWN},
		{360,58 ,TouchEvent::TOUCH_UP  },
		{118,47 ,TouchEvent::TOUCH_DOWN},
		{118,47 ,TouchEvent::TOUCH_UP  },
		{339,153,TouchEvent::TOUCH_DOWN},
		{339,153,TouchEvent::TOUCH_UP  },
		{23 ,156,TouchEvent::TOUCH_DOWN},
		{23 ,156,TouchEvent::TOUCH_UP  },
		{32 ,210,TouchEvent::TOUCH_DOWN},
		{32 ,210,TouchEvent::TOUCH_UP  },
		{211,155,TouchEvent::TOUCH_DOWN},
		{211,155,TouchEvent::TOUCH_UP  },
		{447,153,TouchEvent::TOUCH_DOWN},
		{447,153,TouchEvent::TOUCH_UP  },
		{218,209,TouchEvent::TOUCH_DOWN},
		{218,209,TouchEvent::TOUCH_UP  },
		{209,154,TouchEvent::TOUCH_DOWN},
		{209,154,TouchEvent::TOUCH_UP  },
		{  0,  0,TouchEvent::TOUCH_DOWN},
	};

	UIManager::desktop()->dispatch();
	int i = 0;
	uint32_t length = 0;

	UIManager::desktop()->dispatch();
	// MemoryLeakTracer::stat(__FUNCTION__);
	
	while (t1.get_string() != "Bonjour, tu vas bien ?")
	{
		if (events[i].x != 0 && events[i].y != 0)
		{
			UIManager::notifier()->touch(events[i].x, events[i].y, events[i].state); i++;
		}
		UIManager::desktop()->dispatch();
	}
	UIManager::desktop()->dispatch();
}

void Keyboard::test2()
{
}

void Keyboard::test3()
{
}

void Keyboard::test4()
{
}

void Keyboard::test5()
{
}

void Keyboard::test6()
{
}

void Keyboard::test7()
{
}

void Keyboard::test8()
{
}

void Keyboard::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test7();
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
