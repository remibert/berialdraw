#pragma once
namespace berialdraw
{
	/** Class defines keyboard mapping */
	class Mapping
	{
	public:
		/** Constructor */
		Mapping();

		/** Get mapping name */
		const String & name() const;

		/** Get mapping */
		const String & mapping() const;

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

	protected:
/// @cond DOXYGEN_IGNORE
		String m_name;
		String m_mapping;
/// @endcond
	};

	/** Class defines the list of all mapping of keyboard */
	class Mappings  : public Style
	{
	public:
		/** Constructor */
		Mappings();

		/** Destructor */
		~Mappings();

		/** Select mapping according to its mapping id */
		const Mapping * select(const String & id) const;

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it) override;

		/** Create new mappings */
		static Style * create();

		/** Get the property name for this style */
		const char* property_name() const override { return "mappings"; }

	protected:
/// @cond DOXYGEN_IGNORE
		Vector<Mapping*> m_mappings;
/// @endcond
	};
}
