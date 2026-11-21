#include "berialdraw_imp.hpp"

using namespace berialdraw;

struct Language::Catalog
{
	struct StringLess
	{
		bool operator()(const String & left, const String & right) const
		{
			return strcmp(left.c_str(), right.c_str()) < 0;
		}
	};

	String locale;
	std::map<String, String, StringLess> messages;
};

// Load the default language catalog.
Language::Language()
{
	CatalogLoadStatus status = load_catalog(String("en-US"), m_default_catalog);
	if (status == CatalogLoadStatus::LOADED)
	{
		m_locale = "en-US";
	}
	else
	{
		bd_printf("Language: cannot load default catalog 'en-US'\n");
	}
}

// Destroy the language manager and its loaded catalogs.
Language::~Language()
{
}

// Select a locale and load its catalog.
bool Language::select(const char * locale)
{
	bool result = false;
	if (m_default_catalog && is_valid_locale(locale))
	{
		String requested_locale(locale);
		std::unique_ptr<Catalog> catalog;
		CatalogLoadStatus status = CatalogLoadStatus::MISSING;

		if (requested_locale == "en-US")
		{
			m_active_catalog.reset();
			m_locale = requested_locale;
			result = true;
		}
		else
		{
			status = load_catalog(requested_locale, catalog);

			if (status == CatalogLoadStatus::MISSING)
			{
				const char * separator = strchr(locale, '-');
				if (separator != nullptr)
				{
					String generic_locale(locale, (size_t)(separator - locale));
					status = load_catalog(generic_locale, catalog);
				}
			}

			if (status == CatalogLoadStatus::LOADED)
			{
				m_active_catalog = std::move(catalog);
				m_locale = requested_locale;
				result = true;
			}
			else if (status == CatalogLoadStatus::MISSING)
			{
				m_active_catalog.reset();
				m_locale = requested_locale;
				result = true;
			}
		}
	}
	else if (!is_valid_locale(locale))
	{
		bd_printf("Language: invalid locale '%s'\n", locale == nullptr ? "(null)" : locale);
	}
	else
	{
		bd_printf("Language: default catalog is unavailable\n");
	}
	return result;
}

// Return the selected locale identifier.
const String & Language::locale() const
{
	return m_locale;
}

// Translate a message identifier using the selected and default catalogs.
String Language::translate(const char * id) const
{
	String result(id == nullptr ? "" : id);
	if (id != nullptr && id[0] != '\0')
	{
		bool found = false;
		if (m_active_catalog)
		{
			auto translated = m_active_catalog->messages.find(String(id));
			if (translated != m_active_catalog->messages.end())
			{
				result = translated->second;
				found = true;
			}
		}

		if (!found && m_default_catalog)
		{
			auto translated = m_default_catalog->messages.find(String(id));
			if (translated != m_default_catalog->messages.end())
			{
				result = translated->second;
				found = true;
			}
		}
	}
	return result;
}

// Validate a locale identifier before using it as a resource filename.
bool Language::is_valid_locale(const char * locale)
{
	bool result = false;
	if (locale != nullptr)
	{
		size_t length = strlen(locale);
		size_t subtag_length = 0;
		bool valid = length > 0 && length <= 35;
		bool first_subtag = true;

		for (size_t index = 0; index < length && valid; index++)
		{
			char character = locale[index];
			bool is_alpha = (character >= 'A' && character <= 'Z') ||
				(character >= 'a' && character <= 'z');
			bool is_digit = character >= '0' && character <= '9';

			if (character == '-')
			{
				valid = subtag_length > 0 && subtag_length <= 8 &&
					(!first_subtag || subtag_length >= 2);
				first_subtag = false;
				subtag_length = 0;
			}
			else if (is_alpha || is_digit)
			{
				valid = subtag_length < 8 && (!first_subtag || is_alpha);
				subtag_length++;
			}
			else
			{
				valid = false;
			}
		}

		result = valid && subtag_length > 0 &&
			(!first_subtag || subtag_length >= 2);
	}
	return result;
}

// Load and validate a JSON language catalog.
Language::CatalogLoadStatus Language::load_catalog(const String & locale, std::unique_ptr<Catalog> & catalog)
{
	CatalogLoadStatus result = CatalogLoadStatus::FAILED;
	String filename;
	filename.print("$(ui.languages)/%s.json", locale.c_str());

	if (!File::exists(filename.c_str()))
	{
		result = CatalogLoadStatus::MISSING;
	}
	else
	{
		File file;
		if (file.open(filename.c_str(), "r") == 0)
		{
			Json document;
			bool parsed = false;
			try
			{
				document.unserialize(file);
				parsed = true;
			}
			catch (const JsonUnserializeError & error)
			{
				bd_printf("Language: invalid JSON in '%s' at %d:%d\n",
					filename.c_str(), error.m_line, error.m_column);
			}

			int close_result = file.close();
			if (parsed && close_result == 0)
			{
				JsonIterator root(document);
				JsonIterator locale_value = root["locale"];
				JsonIterator messages = root["messages"];
				String document_locale = locale_value;
				bool valid = root.type() == JsonType::OBJECT &&
					locale_value.type() == JsonType::STRING &&
					document_locale == locale &&
					messages.type() == JsonType::OBJECT;

				if (valid)
				{
					std::unique_ptr<Catalog> loaded_catalog = std::make_unique<Catalog>();
					loaded_catalog->locale = document_locale;

					int message_index = 0;
					for (messages.first(); messages.exist(); messages.next(), message_index++)
					{
						String id = messages.key();
						if (id.size() == 0 || messages.type(message_index) != JsonType::STRING)
						{
							valid = false;
						}
						else
						{
							String value = messages.value();
							loaded_catalog->messages[id] = value;
						}
					}

					if (valid)
					{
						catalog = std::move(loaded_catalog);
						result = CatalogLoadStatus::LOADED;
					}
					else
					{
						bd_printf("Language: invalid catalog structure in '%s'\n", filename.c_str());
					}
				}
				else
				{
					bd_printf("Language: invalid catalog metadata in '%s'\n", filename.c_str());
				}
			}
			else if (parsed == false)
			{
				result = CatalogLoadStatus::FAILED;
			}
			else
			{
				bd_printf("Language: cannot close catalog '%s'\n", filename.c_str());
			}
		}
		else
		{
			bd_printf("Language: cannot read catalog '%s'\n", filename.c_str());
		}
	}
	return result;
}
