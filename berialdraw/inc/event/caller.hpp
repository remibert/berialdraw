#ifndef Caller_hpp_INCLUDED
#define Caller_hpp_INCLUDED

namespace berialdraw
{
	class Event;
	class Widget;

	/** Abstract class to call event callback */
	class Callable
	{
	public:
		/** Call callback registered on event
		@param widget widget handle
		@param evt event handle */
		virtual void call(Widget * widget, Event * evt) = 0;

		/** Compare if the current object is the same than this object
		@param object instance to compare with the current instance 
		@return true if equal else returns false */
		virtual bool operator==(const Callable & object) = 0;

		/** Gets the event type
		@return event type */
		virtual uint32_t type() const = 0;

		/** Gets the widget handle 
		@return the widget handle */
		virtual Widget * widget() const = 0;
		
		/** Destructor */
		virtual ~Callable() 
		{
		}
	};


	/** Class which links the object to the method with callback */
	template<class CLASS, class EVENT> class MethodCaller : public Callable
	{
	public:
		/** Constructor on method
		@param object handle of the objet
		@param method method call back pointer
		@param widget widget attached with this method */
		MethodCaller(CLASS* object, void (CLASS::*method)(Widget * widget, const EVENT &evt), Widget * widget)
		{
			m_object = object;
			m_method = method;
			m_widget = widget;
			m_type   = EVENT::type_id();
		}

		/** Destructor of this instance */
		virtual ~MethodCaller()
		{
		}

		/** Raise an event. Call the callback attached to the instance of the class.
		@param widget handle of the widget  
		@param evt pointer on event received */
		virtual void call (Widget * widget, Event * evt)
		{
			(m_object->*m_method)(widget, *dynamic_cast<EVENT*>(evt));
		}

		/** Compare if the current object is the same than this object
		@param object instance to compare with the current instance
		@return true if equal else returns false */
		virtual bool operator==(const Callable & object)
		{
			const MethodCaller * obj = dynamic_cast<const MethodCaller*>(&object);
			if (obj)
			{
				if (obj->m_object == m_object && obj->m_method == m_method && obj->m_type == EVENT::type_id())
				{
					return true;
				}
			}
			return false;
		}

		/** Gets the event type
		@return event type */
		virtual uint32_t type() const
		{
			return EVENT::type_id();
		}

		/** Gets the widget handle 
		@return the widget handle */
		virtual Widget * widget() const
		{
			return m_widget;
		}
			
///< @cond DOXYGEN_IGNORE
	protected:
		/** Instance of widget */
		Widget * m_widget = 0;

		/** Instance of the object */
		CLASS* m_object = 0;

		/** Method pointer to call event callback */
		void (CLASS::*m_method)(Widget * widget, const EVENT & evt);

		/** Identier of event */
		uint32_t m_type;
///< @endcond 
	};


	/** Class which links the object to the function with callback */
	template<class EVENT> class FunctionCaller : public Callable
	{
	public:
		/** Constructor on function
		@param function function call back pointer
		@param widget handle of the widget */
		FunctionCaller(void (*function)(Widget * widget, const EVENT &evt), Widget * widget)
		{
			m_function = function;
			m_widget = widget;
			m_type   = EVENT::type_id();
		}

		/** Destructor of this instance */
		virtual ~FunctionCaller()
		{
		}

		/** Raise an event. Call the callback attached to the instance of the class.
		@param widget pointer on widget
		@param evt pointer on event received */
		virtual void call (Widget * widget, Event * evt)
		{
			m_function(widget, *dynamic_cast<EVENT*>(evt));
		}

		/** Compare if the current object is the same than this object
		@param object instance to compare with the current instance
		@return true if equal else returns false */
		virtual bool operator==(const Callable & object)
		{
			const FunctionCaller * obj = dynamic_cast<const FunctionCaller*>(&object);
			if (obj)
			{
				if (obj->m_function == m_function && obj->m_type == EVENT::type_id())
				{
					return true;
				}
			}
			return false;
		}

		/** Gets the event type
		@return event type */
		virtual uint32_t type() const
		{
			return EVENT::type_id();
		}

		/** Gets the widget handle 
		@return the widget handle */
		virtual Widget * widget() const
		{
			return m_widget;
		}
			
///< @cond DOXYGEN_IGNORE
	protected:
		/** Instance of widget */
		Widget * m_widget = 0;

		/** function pointer to call event callback */
		void (*m_function)(Widget * widget, const EVENT & evt);

		/** Identier of event */
		uint32_t m_type;
///< @endcond
	};

}

#endif

