#pragma once
namespace berialdraw
{
	/** The PictureStyle class customizes the appearance of Picture widgets.
	It includes properties for the image filename, alpha transparency, and fit mode. */
	class PictureStyle : public Style
	{
	public:
		/** Constructor */
		PictureStyle();

		/** Destructor */
		~PictureStyle();

		/** Create new style instance */
		static Style * create();

		/** Serialize the content of style into json */
		void serialize(JsonIterator & it) override;

		/** Unserialize the content of style from json */
		void unserialize(JsonIterator & it) override;

		// Filename properties

		/** Get the filename value */
		const String & filename() const { return m_filename; }

		/** Set filename value with string */
		void filename(const String & s);

		// Alpha properties

		/** Get the alpha transparency (0 = fully transparent, 255 = fully opaque) */
		uint8_t alpha() const { return m_alpha; }

		/** Set the alpha transparency */
		void alpha(uint8_t a);

		// Fit mode properties

		/** Get the image fit mode */
		ImageFitMode fit_mode() const { return m_fit_mode; }

		/** Set the image fit mode */
		void fit_mode(ImageFitMode mode);

		/** Get the property name for this style */
		const char* property_name() const override { return "picture"; }

	protected:
	/// @cond DOXYGEN_IGNORE
		String m_filename;
		uint8_t m_alpha = 255;
		ImageFitMode m_fit_mode = IMAGE_FIT_ASPECT_FIT;
		bool m_image_modified = true;
	/// @endcond
	};
}
