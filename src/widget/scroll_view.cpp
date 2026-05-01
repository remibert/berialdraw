#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** ScrollView Implementation */

ScrollView::ScrollView(Widget * parent):
	ScrollableContent("scroll_view", parent, sizeof(ScrollView))
{
}

ScrollView::~ScrollView()
{
}

/** Serialize the content of widget into json */
void ScrollView::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	ScrollViewStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void ScrollView::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	ScrollViewStyle::unserialize(it);
}

