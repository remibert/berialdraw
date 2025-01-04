#ifndef Invalidator_hpp_INCLUDED
#define Invalidator_hpp_INCLUDED

namespace berialdraw
{
	/** Intended to save the elements to be refreshed */
	class Invalidator
	{
	public:
		/** Constructor */
		Invalidator();

		/** Destructor */
		~Invalidator();

		/** Add dirty object */
		void dirty(void * object);

		/** Check if object is dirty */
		bool is_dirty(void * object);

		/** Clear all dirties object */
		void clear(void * object);

	protected:
/// @cond DOXYGEN_IGNORE
		/** List of all object required to refresh */
		Vector<void*> m_dirties;
/// @endcond
	};
}
#endif
