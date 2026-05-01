#pragma once
namespace berialdraw
{
	/** StyleItem stores a named set of style properties in JSON format.
	Ultra-lightweight container: just a name and JSON properties string.
	The JSON is parsed and applied via unserialize(). */
	class StyleItem
	{
	public:
		/** Constructor */
		StyleItem();

		/** Constructor with name */
		StyleItem(const String& name);

		/** Constructor with name and properties */
		StyleItem(const String& name, const String& json_properties);

		/** Constructor with name and compiled Style pointer */
		StyleItem(const String& name, Style* style);

		/** Copy constructor */
		StyleItem(const StyleItem& other);

		/** Move constructor */
		StyleItem(StyleItem&& other) noexcept;

		/** Destructor */
		virtual ~StyleItem();

		/** Get the style name */
		const String& name() const { return m_name; }

		/** Set the style name */
		void name(const String& n) { m_name = n; }

		/** Get the properties as a Json (returns by value with move semantics) */
		Json properties() const;

		/** Get the compiled Style pointer (non-owning) */
		Style* style() const { return m_style; }

		/** Set the compiled Style pointer (non-owning) */
		void style(Style* s) { m_style = s; }

		/** Serialize the StyleItem to JSON */
		void serialize(JsonIterator & it);

		/** Unserialize the StyleItem from JSON */
		void unserialize(JsonIterator & it);

	protected:
		/// @cond DOXYGEN_IGNORE
		String m_name;
		String m_properties;
		Style* m_style = nullptr;
		/// @endcond
	};
}

