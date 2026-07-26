#pragma once
namespace berialdraw
{
	/** The PaddingStyle class defines the padding within widgets.*/
	class PaddingStyle : public Style
	{
	public:
		/** Constructor */
		PaddingStyle();

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it) override;

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it) override;

		/** Copy operator */
		PaddingStyle& operator=(const PaddingStyle& other);

		/** Set properties with another */
		void set(const PaddingStyle & other);

		/** Create new text style */
		static Style * create();



		/** Get the padding */
		const Margin & padding() const { return m_padding; }
		
		/** Set the padding */
		void padding(const Margin & m);
		
		/** Set the padding in pixels */
		void padding(Dim top, Dim left, Dim bottom, Dim right);
		
		/** Set the padding with a precision of 64th of a pixel */
		void padding_q6(Dim top, Dim left, Dim bottom, Dim right);

		/** Set the padding */
		void padding(Dim horizontal, Dim vertical);

		/** Set the padding */
		void padding(Dim value);


		/** Get the property name for this style */
		const char* property_name() const override { return "padding"; }

	protected:
/// @cond DOXYGEN_IGNORE
		Margin m_padding = {0,0,0,0};
/// @endcond 
	};
}
