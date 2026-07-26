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

/** Copy all styles of the scroll_view */
void ScrollView::copy(const ScrollView& scroll_view)
{
	ScrollableContent::copy(*(ScrollableContent*)(&scroll_view));
}

/** Copy all styles of the scroll_view */
void ScrollView::copy(const ScrollView* scroll_view)
{
	if (scroll_view)
	{
		copy(scroll_view);
	}
}

/** Serialize the content of widget into json */
void ScrollView::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	ScrollableContent::serialize(it);
}

/** Unserialize the content of widget from json */
void ScrollView::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

StyleCascadeMode ScrollView::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}
