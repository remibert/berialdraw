#ifndef side_hpp_INCLUDED
#define side_hpp_INCLUDED

namespace berialdraw
{
	/** Define the side visible */
	enum Side  : uint8_t
	{
		NO_SIDE                = 0x00,                                        ///< No side visible
		TOP_SIDE               = 0x01,                                        ///< Top side visible
		RIGHT_SIDE             = 0x02,                                        ///< Right side visible
		BOTTOM_SIDE            = 0x04,                                        ///< Bottom side visible
		LEFT_SIDE              = 0x08,                                        ///< Left side visible

		TOP_RIGHT_SIDE         = (TOP_SIDE|RIGHT_SIDE),                       ///< Top and right sides visible
		TOP_LEFT_SIDE          = (TOP_SIDE|LEFT_SIDE),                        ///< Top and left sides visible
		BOTTOM_RIGHT_SIDE      = (BOTTOM_SIDE | RIGHT_SIDE),                  ///< Bottom and right sides visible
		BOTTOM_LEFT_SIDE       = (BOTTOM_SIDE | LEFT_SIDE),                   ///< Bottom and left sides visible
		TOP_BOTTOM_SIDE        = (TOP_SIDE|BOTTOM_SIDE),                      ///< Top and bottom sides visible
		LEFT_RIGHT_SIDE        = (LEFT_SIDE|RIGHT_SIDE),                      ///< Left and right sides visible

		TOP_RIGHT_BOTTOM_SIDE  = (TOP_SIDE|RIGHT_SIDE|BOTTOM_SIDE),           ///< Top, right, bottom sides visible
		RIGHT_BOTTOM_LEFT_SIDE = (RIGHT_SIDE|BOTTOM_SIDE|LEFT_SIDE),          ///< Right, bottom, left sides visible
		BOTTOM_LEFT_TOP_SIDE   = (BOTTOM_SIDE|LEFT_SIDE|TOP_SIDE),            ///< Bottom, left, top sides visible
		LEFT_TOP_RIGHT_SIDE    = (LEFT_SIDE|TOP_SIDE|RIGHT_SIDE),             ///< Left, top, right sides visible

		INNER_AREA             = 0x10,                                        ///< Keep the inner part of the rectangles without sides
		RECTANGULAR_EXTREMITY  = 0x20,                                        ///< The extremity will be rectangular

		ALL_SIDES              = (TOP_SIDE|RIGHT_SIDE|BOTTOM_SIDE|LEFT_SIDE), ///< All sides visible
	};

	constexpr Side operator|(Side a, Side b) 
	{
		return static_cast<Side>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}
		
	
	constexpr bool is_set(Side value, Side flag) 
	{
		return (static_cast<uint8_t>(value) & static_cast<uint8_t>(flag)) != 0;
	}

	/** Unserialize the content of side from json */
	void unserialize(JsonIterator & it, Side & side);
	
	/** Serialize the content of side into json */
	void serialize(JsonIterator & it, Side side);

}
#endif