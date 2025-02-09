#ifndef Invalidator_hpp_INCLUDED
#define Invalidator_hpp_INCLUDED

namespace berialdraw
{
	class Widget;
	class Style;
	class Shape;
	class Window;

	/** Intended to save the elements to be refreshed */
	class Invalidator
	{
	public:
		enum Status
		{
			NOTHING = 0,
			REDRAW  = 1,
			GEOMETRY = 2,
			ALL     = GEOMETRY|REDRAW
		};

		/** Constructor */
		Invalidator();

		/** Destructor */
		~Invalidator();

		/** Add dirty object */
		void dirty(Widget * object, enum Status status);

		/** Add dirty object */
		void dirty(Style * object, enum Status status);

		/** Remove dirty object */
		void undirty(Widget * object, enum Status status);

		/** Check if object is dirty */
		bool is_dirty(Widget * object);

		/** Check if something must be refreshed */
		bool is_dirty();

		/** Clear all dirties object */
		void clear(void * object);

		/** Add Shape in the list */
		void add(Widget * widget, Shape * shape, size_t size);

		/** Remove shape from the list */
		void remove(Shape * shape);

		/** Add widget in the list */
		void add(Widget * widget, size_t size);

		/** Remove widget from the list */
		void remove(Widget * widget);

		/** Updates parents that a child widget is invalidate */
		void update();

	protected:
/// @cond DOXYGEN_IGNORE
		/** Add dirty object */
		void dirty(void * object, enum Status status);

		/** Add dirty object */
		void undirty(void * object, enum Status status);

		/** Return the index in table or -1 if not found */
		int32_t search(void * object);

		/** List of all object required to refresh */
		struct InvalidatorItem
		{
			unsigned int status:4;
			unsigned int window:1;
			uint16_t size;
			Widget * widget;
			Shape  * shape;
		};
		Vector<struct InvalidatorItem> m_widgets;
/// @endcond
	};
}
#endif
