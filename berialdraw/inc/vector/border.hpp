#pragma once
namespace berialdraw
{
	/** Defines the border configuration options for a rectangle */
	enum Borders : uint16_t
	{
		NO_BORDER                  = 0x00,  ///< No border visible

		TOP_BORDER                 = 0x01,  ///< Top border visible
		RIGHT_BORDER               = 0x02,  ///< Right border visible
		BOTTOM_BORDER              = 0x04,  ///< Bottom border visible
		LEFT_BORDER                = 0x08,  ///< Left border visible

		INNER_AREA                 = 0x10,  ///< Keep the inner area of the rectangle even when borders are hidden

		RIGHT_ANGLE_END            = 0x20,  ///< Ends of visible borders will be right-angled
		ROUNDED_END                = 0x40,  ///< Ends of visible borders will be rounded

		RIGHT_ANGLE_WITHOUT_BORDER = 0x80,  ///< Corners of hidden borders will be right-angled
		ALL_BORDERS                = (TOP_BORDER | RIGHT_BORDER | BOTTOM_BORDER | LEFT_BORDER), ///< All borders visible
	};


	constexpr Borders operator|(Borders a, Borders b) 
	{
		return static_cast<Borders>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}
		
	
	constexpr bool is_set(Borders value, Borders flag) 
	{
		return (static_cast<uint8_t>(value) & static_cast<uint8_t>(flag)) != 0;
	}

	/** Unserialize the content of border from json */
	void unserialize(JsonIterator & it, Borders & border);
	
	/** Serialize the content of border into json */
	void serialize(JsonIterator & it, Borders border);

}
