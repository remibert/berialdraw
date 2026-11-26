#include "berialdraw_imp.hpp"

using namespace berialdraw;

#ifdef _DEBUG
void Widget::test1()
{
	Window window;
	window.position(0, 0);
	window.size(480, 300);
	Column * widget_tree = new Column(&window);
	widget_tree->position(20, 20);
	widget_tree->size(440, 260);

	Json serialized_widget(
		"{'children':["
			"{'type':'label','id':101,'text':'Label from JSON'},"
			"{'type':'edit','id':102,'place_holder':'Edit here'},"
			"{'type':'combo','id':103,'place_holder':'Choose an option'},"
			"{'type':'button','id':104,'text':'Button from JSON'}"
		"]}");
	JsonIterator iterator(serialized_widget);

	assert(widget_tree->unserialize_children(iterator));
	assert(dynamic_cast<Label*>(widget_tree->search(101)) != nullptr);
	assert(dynamic_cast<Edit*>(widget_tree->search(102)) != nullptr);
	assert(dynamic_cast<Combo*>(widget_tree->search(103)) != nullptr);
	assert(dynamic_cast<Button*>(widget_tree->search(104)) != nullptr);
	UIManager::desktop()->dispatch("$(ui.tests)/out/widget1.svg");

	Json unsupported_widget("{'children':[{'type':'unsupported_widget'}]}");
	JsonIterator unsupported_iterator(unsupported_widget);
	Column unsupported_root(nullptr);
	assert(unsupported_root.unserialize_children(unsupported_iterator) == false);
	assert(unsupported_root.children() == nullptr);
}

void Widget::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test1();
	}
}
#endif
