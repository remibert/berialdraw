#ifndef Validator_hpp_INCLUDED
#define Validator_hpp_INCLUDED

namespace berialdraw
{
	/** Abstract interface class to validate keyboard input on an input field */
	class Validator
	{
	public:

		/** Validation result */
		enum ValidResult
		{
			REFUSED, ///< The text entered does not match what is expected.
			PLAUSIBLE, ///< The text entered is incomplete but is acceptable.
			ACCEPTED,  ///< The text entered matches exactly what is expected.
		};

		/** Destructor */
		virtual ~Validator(){}

		/** Abstract function to check if the input field is correct */
		virtual ValidResult validate(const String & text) = 0;

		/** Abstract to get the display string */
		virtual void display(const String & text, String & display) = 0;

		/** Get the current separator or null if the field is not fully filled */
		virtual wchar_t separator(const String & text) = 0;

	};
}
#endif
