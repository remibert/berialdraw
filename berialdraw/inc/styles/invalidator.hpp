#ifndef Invalidator_hpp_INCLUDED
#define Invalidator_hpp_INCLUDED

namespace berialdraw
{
	class Widget;
	class Style;
	class Shape;
	class Path;

	/** Intended to save the elements to be refreshed */
	class Invalidator
	{
	public:
		enum Status
		{
			NOTHING = 0,
			REPAINT = 1,
			REPLACE = 2,
			RESIZE  = 4,
		};

		/** Constructor */
		Invalidator();

		/** Destructor */
		~Invalidator();

		/** Add dirty object */
		void dirty(Widget * object, enum Status status);

		/** Add dirty object */
		void dirty(Style * object, enum Status status);

		/** Add dirty object */
		void dirty(Shape * object, enum Status status);


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

		/** List of all object required to refresh */
		struct InvalidatorItem
		{
			enum Status status;
			size_t size;
			Widget * widget;
			Shape * shape;
		};
		Vector<struct InvalidatorItem> m_widgets;
/// @endcond
	};
}
#endif
