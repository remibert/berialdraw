#include "berialdraw_imp.hpp"
using namespace berialdraw;

/// @cond DOXYGEN_IGNORE

// Helper class to receive and count timer events
class TimerEventCounter
{
public:
	uint32_t m_count;
	uint32_t m_last_timer_id;
	uint32_t m_last_elapsed_ticks;
	
	TimerEventCounter() : m_count(0), m_last_timer_id(0), m_last_elapsed_ticks(0)
	{
	}
	
	// Callback method for timer events
	void on_timeout(Widget* widget, const TimerEvent& evt)
	{
		m_count++;
		m_last_timer_id = evt.timer_id();
		m_last_elapsed_ticks = evt.elapsed_ticks();
	}
};

/// @endcond

void Timer::test1()
{
	// Create a window for timer attachment
	Window window;
	window.position(10, 10);
	window.size(200, 100);
	window.color(Color::WHITE);

	// Create a recurring timer
	Timer* timer = new Timer(&window);
	timer->interval(100);
	timer->recurring(true);
	
	// Verify initial state
	assert(!timer->is_running());
	
	// Start timer
	timer->start();
	assert(timer->is_running());
	
	// Render window
	UIManager::desktop()->dispatch();
	
	// Stop timer
	timer->stop();
	assert(!timer->is_running());
}

void Timer::test2()
{
	// Create a window for timer attachment
	Window window;
	window.position(10, 10);
	window.size(200, 100);
	window.color(Color::LIGHT_GRAY);

	// Create a counter to track timer events
	TimerEventCounter counter;
	
	// Create a one-shot timer (50ms interval)
	Timer* timer = new Timer(&window);
	timer->interval(50);
	timer->recurring(false);
	
	// Bind the timer to our callback
	timer->bind(&counter, &TimerEventCounter::on_timeout);
	
	// Timer should not be running initially
	assert(!timer->is_running());
	assert(counter.m_count == 0);
	
	// Start timer
	timer->start();
	assert(timer->is_running());
	
	// Dispatch to process pending events
	while (counter.m_count <= 0)
	{
		UIManager::desktop()->dispatch();
	}
	
	// One-shot timer should have fired once
	assert(counter.m_count == 1);
	assert(counter.m_last_timer_id == timer->timer_id());
}

void Timer::test3()
{
	// Create a window for timer attachment
	Window window;
	window.position(10, 10);
	window.size(200, 100);
	window.color(Color::LIGHT_BLUE);

	// Create a counter to track recurring timer events
	TimerEventCounter counter;
	
	// Create a recurring timer with short interval
	Timer* timer = new Timer(&window);
	timer->interval(10);  // 10ms for fast firing
	timer->recurring(true);
	
	// Bind the timer to our callback
	timer->bind(&counter, &TimerEventCounter::on_timeout);
	
	// Verify initial state
	assert(!timer->is_running());
	assert(counter.m_count == 0);
	
	// Start timer
	timer->start();
	assert(timer->is_running());
	
	// Dispatch multiple times to allow timer to fire several times
	for (int i = 0; i < 5; i++)
	{
		UIManager::desktop()->dispatch();
	}
	
	// Timer should have fired at least once
	assert(counter.m_count > 0);
	
	// Stop timer
	timer->stop();
	assert(!timer->is_running());
	
	uint32_t final_count = counter.m_count;
	
	// Dispatch again - should not increment
	UIManager::desktop()->dispatch();
	assert(counter.m_count == final_count);
}

void Timer::test4()
{
	// Create a window
	Window window;
	window.position(10, 10);
	window.size(300, 150);
	window.color(Color::MINT_CREAM);

	// Create counters for multiple timers
	TimerEventCounter counter1;
	TimerEventCounter counter2;
	
	// Create first recurring timer
	Timer* timer1 = new Timer(&window);
	timer1->interval(20);   // 20ms
	timer1->recurring(true);
	timer1->bind(&counter1, &TimerEventCounter::on_timeout);
	
	// Create second recurring timer with different interval
	Timer* timer2 = new Timer(&window);
	timer2->interval(50);   // 50ms
	timer2->recurring(true);
	timer2->bind(&counter2, &TimerEventCounter::on_timeout);
	
	// Start both timers
	timer1->start();
	timer2->start();
	assert(timer1->is_running());
	assert(timer2->is_running());
	
	// Dispatch multiple times to fire both timers
	for (int i = 0; i < 10; i++)
	{
		UIManager::desktop()->dispatch();
	}

	//UIManager::desktop()->mainloop();
	
	// Both timers should have fired (timer1 more frequently than timer2)
	assert(counter1.m_count > 0);
	assert(counter2.m_count > 0);
	assert(counter1.m_count >= counter2.m_count);
	
	// Verify event data
	assert(counter1.m_last_timer_id == timer1->timer_id());
	assert(counter2.m_last_timer_id == timer2->timer_id());
	
	// Stop both timers
	timer1->stop();
	timer2->stop();
	assert(!timer1->is_running());
	assert(!timer2->is_running());
}

void Timer::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test4();
		test3();
		test2();
		test1();
	}
}
