#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
ScreenCrc::ScreenCrc()
{
	m_crcs = 0;
	m_filename = 0;
}

/** Destructor */
ScreenCrc::~ScreenCrc()
{
	close();
}

/** Add screen crc */
void ScreenCrc::add(const String & filename, uint32_t crc_value)
{
	if (m_crcs)
	{
		String scale;
		scale.print("scale_%d",UIManager::renderer()->scale());

		if (m_crcs->operator[](filename)[scale].exist())
		{
			if ((int)crc_value != (int)m_crcs->operator[](filename)[scale])
			{
				bd_printf("%s the screen image CRC does not match the reference \n",filename.c_str());
			}
		}
		m_crcs->operator[](filename)[scale] = crc_value;
	}
}

/** Open screen crc file */
void ScreenCrc::open(const String & crc_filename)
{
	if (crc_filename.size())
	{
		if (m_crcs)
		{
			close();
		}

		delete m_crcs;
		m_crcs = new Json;
		m_filename = new String(crc_filename);

		File file;
		if (file.open(*m_filename,"rb") != -1)
		{
			try
			{
				m_crcs->unserialize(file);
			}
			catch(...)
			{
			}
		}
	}
}

/** Close screen crc file */
void ScreenCrc::close()
{
	if (m_crcs && m_filename)
	{
		File file;
		if (file.open(*m_filename,"w") >= 0)
		{
			m_crcs->serialize(file,1);
		}
		delete m_crcs;
		delete m_filename;
		m_crcs = 0;
		m_filename = 0;
	}
}