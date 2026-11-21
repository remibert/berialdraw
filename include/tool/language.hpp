#pragma once
namespace berialdraw
{
	/** Manages the active UI language and resolves message identifiers. */
	class Language
	{
	public:
		/** Load the default language catalog. */
		Language();

		/** Destroy the language manager and its loaded catalogs. */
		virtual ~Language();

		/** Select a locale and load its catalog.
		@param locale Canonical BCP 47 locale identifier
		@return True if the locale or a fallback catalog was selected */
		bool select(const char * locale);

		/** Return the selected locale identifier. */
		const String & locale() const;

		/** Translate a message identifier using the selected and default catalogs.
		@param id Stable message identifier
		@return Translated text, or the identifier if no translation exists */
		String translate(const char * id) const;

	private:
		enum class CatalogLoadStatus
		{
			MISSING,
			LOADED,
			FAILED
		};

		struct Catalog;

		static bool is_valid_locale(const char * locale);
		static CatalogLoadStatus load_catalog(const String & locale, std::unique_ptr<Catalog> & catalog);

		std::unique_ptr<Catalog> m_default_catalog;
		std::unique_ptr<Catalog> m_active_catalog;
		String m_locale;
	};
}
