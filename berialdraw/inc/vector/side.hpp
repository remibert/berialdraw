#ifndef side_hpp_INCLUDED
#define side_hpp_INCLUDED

namespace berialdraw
{
	/** Define the side visible */
	enum Sides  : uint8_t
	{
		NO_SIDE                = 0x00,                                        ///< No side visible
		TOP_SIDE               = 0x01,                                        ///< Top side visible
		RIGHT_SIDE             = 0x02,                                        ///< Right side visible
		BOTTOM_SIDE            = 0x04,                                        ///< Bottom side visible
		LEFT_SIDE              = 0x08,                                        ///< Left side visible

		INNER_AREA             = 0x10,                                        ///< Keep the inner part of the rectangles without sides
		RECTANGULAR_EXTREMITY  = 0x20,                                        ///< The extremity will be rectangular
		ROUNDED_EXTREMITY      = 0x40,

		ALL_SIDES              = (TOP_SIDE|RIGHT_SIDE|BOTTOM_SIDE|LEFT_SIDE), ///< All sides visible
	};

	constexpr Sides operator|(Sides a, Sides b) 
	{
		return static_cast<Sides>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}
		
	
	constexpr bool is_set(Sides value, Sides flag) 
	{
		return (static_cast<uint8_t>(value) & static_cast<uint8_t>(flag)) != 0;
	}

	/** Unserialize the content of side from json */
	void unserialize(JsonIterator & it, Sides & side);
	
	/** Serialize the content of side into json */
	void serialize(JsonIterator & it, Sides side);

}
#endif