#pragma once
namespace berialdraw
{
	/** Manage the screen CRC to check test regressions */
	class ScreenCrc
	{
	public:
		/** Constructor */
		ScreenCrc();

		/** Destructor */
		~ScreenCrc();

		/** Add screen crc */
		void add(const String & screen_filename, uint32_t crc_value);

		/** Open screen crc file */
		void open(const String & out_filename, const String & ref_filename);

		/** Close screen crc file */
		void close();
	protected:
/// @cond DOXYGEN_IGNORE
		Json        * m_crcs = 0;
		String      * m_out_filename = 0;
/// @endcond 
	};
}
