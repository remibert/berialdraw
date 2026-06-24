#pragma once
namespace berialdraw
{
	enum class Align : uint8_t
	{
		CENTER              = 0x00                        ,  ///< Placed in the center
		ALIGN_LEFT          = 0x01                        ,  ///< Placed in horizontal on the left
		ALIGN_RIGHT         = 0x02                        ,  ///< Placed in horizontal on the right
		ALIGN_TOP           = 0x04                        ,  ///< Placed in vertical on the top
		ALIGN_BOTTOM        = 0x08                        ,  ///< Placed in vertical on the bottom
		ALIGN_TOP_LEFT      = ALIGN_TOP    | ALIGN_LEFT   ,  ///< Placed on the top left corner
		ALIGN_TOP_RIGHT     = ALIGN_TOP    | ALIGN_RIGHT  ,  ///< Placed on the top right corner
		ALIGN_BOTTOM_LEFT   = ALIGN_BOTTOM | ALIGN_LEFT   ,  ///< Placed on the bottom left corner
		ALIGN_BOTTOM_RIGHT  = ALIGN_BOTTOM | ALIGN_RIGHT  ,  ///< Placed on the bottom right corner
		ALIGN_HORIZONTAL    = ALIGN_LEFT   | ALIGN_RIGHT  ,  ///< Placed horizontal center
		ALIGN_VERTICAL      = ALIGN_TOP    | ALIGN_BOTTOM ,  ///< Placed vertical center
		ALIGN_DEFAULT       = 0x0F                           ///< Not initialized (Center)
	};

	/** Serialize the align to JSON.
	@param name Name of the JSON field.
	@param it JSON iterator. */
	void serialize(const char * name, JsonIterator & it, Align value);

	/** Unserialize the align from JSON.
	@param name Name of the JSON field.
	@param it JSON iterator.*/
	void unserialize(const char * name, JsonIterator & it, Align & value);

	inline Align operator|(Align a, Align b)
	{
		return static_cast<Align>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline Align operator&(Align a, Align b)
	{
		return static_cast<Align>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	inline Align& operator|=(Align& a, Align b)
	{
		a = a | b;
		return a;
	}

	inline bool has_flag(Align value, Align flag)
	{
		return (value & flag) == flag;
	}

	inline Align operator>>(Align a, int shift)
	{
		return static_cast<Align>(static_cast<uint8_t>(a) >> shift);
	}
}
