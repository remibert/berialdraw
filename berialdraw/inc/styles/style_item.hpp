#pragma once
namespace berialdraw
{
	/** StyleItem stores a named set of style properties in JSON format.
	Ultra-lightweight container: just a name and JSON properties string.
	The JSON is parsed and applied selectively via Style::apply_style(). */
	class StyleItem
	{
	public:
		/** Constructor */
		StyleItem();

		/** Constructor with name */
		StyleItem(const String& name);

		/** Constructor with name and properties */
		StyleItem(const String& name, const String& json_properties);

		/** Destructor */
		virtual ~StyleItem();

		/** Get the style name */
		const String& name() const { return m_name; }

		/** Set the style name */
		void name(const String& n) { m_name = n; }

			/** Get the properties as a JsonIterator 
		Parses the JSON string and returns iterator for selective unserialize */
		JsonIterator properties() const;

		/** Serialize the StyleItem to JSON */
		void serialize(JsonIterator & it);

		/** Unserialize the StyleItem from JSON */
		void unserialize(JsonIterator & it);

	protected:
		/// @cond DOXYGEN_IGNORE
		String m_name;
		String m_properties;
		/// @endcond
	};
}

