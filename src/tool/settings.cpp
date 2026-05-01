#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Settings implementation - inherits from Json
Settings::Settings() : Json()
{
	// Create an empty root object
	unserialize("{}");
}

Settings::Settings(const char * buffer) : Json(buffer)
{
	// Constructor with initial buffer
}

Settings::~Settings()
{
	// Json destructor handles cleanup
}

Settings::Settings(const Settings & settings) : Json()
{
	String stream;
	const_cast<Settings&>(settings).serialize(stream, -1);
	unserialize(stream.c_str());
}

Settings& Settings::operator=(const Settings& other)
{
	if (this != &other)
	{
		String stream;
		const_cast<Settings&>(other).serialize(stream, -1);
		unserialize(stream.c_str());
	}
	return *this;
}

String Settings::resolve(const String & template_str)
{
	String result(template_str);
	String resolved(template_str);
	String var_name;
	int32_t pos = 0;
	bool found;
	int32_t count = 0;

	do
	{
		found = false;
		
		// Iterate through all variables found in template_str
		do
		{
			pos = resolved.search_var(var_name, pos);
			if (pos != INT32_MAX)
			{
				// Parse dotted notation (e.g., "toto.titi.tata")
				JsonIterator it(*this);
			
				// Split variable name by dots and navigate through nested structure
				int32_t dot_pos = 0;
				int32_t next_dot = 0;
				String current_key;
				bool var_found = true;
			
				while (dot_pos < (int32_t)var_name.count() && var_found)
				{
					// Find next dot or end of string
					next_dot = var_name.find(".", dot_pos);
					if (next_dot == INT32_MAX)
					{
						next_dot = var_name.count();
					}
				
					// Extract key between dot_pos and next_dot
					var_name.slice(dot_pos, next_dot, current_key);
				
					// Navigate to nested value
					it = it[current_key.c_str()];
				
					// Check if the path exists
					if (!it.exist())
					{
						var_found = false;
						break;
					}
				
					// Move to next segment
					dot_pos = next_dot + 1;
				}
			
				// If we found the complete path, replace it
				if (var_found && it.exist())
				{
					String value_str = (String)it;

					// Create placeholder and replace in result
					String placeholder("$(");
					placeholder += var_name;
					placeholder += ")";
					result.replace(placeholder.c_str(), value_str.c_str());
					found = true;
				}
			
				// Move past the current variable for next search
				pos += 2; // Move past $(
			}
		}
		while (pos != INT32_MAX);

		resolved = result;
		pos = 0;
		count ++;
	}
	while (found == true && count < 3);
	
	return result;
}

String Settings::resolve(const char * template_str)
{
	if (template_str)
	{
		String str(template_str);
		return resolve(str);
	}
	return String("");
}

