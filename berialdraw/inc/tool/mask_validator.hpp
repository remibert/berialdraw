#pragma once
namespace berialdraw
{
	/** Mask expression to validate keyboard input on an input field */
	class MaskValidator : public Validator
	{
	public:
		/** Constructor 
		@param mask :
		% followed with : 
			- d:decimal (0-9)
			- a:alphanumeric (A-Z,a-z,0-9)
			- A:alphanumeric (A-Z,0-9)
			- s:string (all characters)
			- l:letter (a-z)
			- L:letter (A-Z)
			- X:hexadecimal (A-F,0-9)
			- x:hexadecimal (a-f,0-9)
		*/
		MaskValidator(String & mask);

		/** Destructor */
		virtual ~MaskValidator();

		/** Check if the input field is correct */
		virtual ValidResult validate(const String & text);

		/** Get the display string */
		virtual void display(const String & text, String & display);

		/** Get the current separator or null if the field is not fully filled */
		virtual wchar_t separator(const String & text);

#ifdef _DEBUG
		// Unitary test
		static void test1();

		// Unitary test
		static void test();
#endif
	protected:
#ifndef SWIG
/// @cond DOXYGEN_IGNORE
		Validator::ValidResult check_char(wchar_t mask_char, wchar_t text_char);
		Validator::ValidResult get_mask_char(uint32_t & pos);
		Validator::ValidResult check_text(const String & text, uint32_t & pos);
		String  & m_mask;
		uint32_t m_length = UINT32_MAX;
		wchar_t m_mask_char;
		bool    m_significant_zero = false;
		bool    m_field_overflow = false;
		int32_t m_min = INT32_MIN;
		int32_t m_max = INT32_MAX;
/// @endcond
#endif
	};
}
