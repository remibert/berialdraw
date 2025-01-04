#ifndef berialdraw_imp_hpp_INCLUDED
#define berialdraw_imp_hpp_INCLUDED
/// @cond DOXYGEN_IGNORE
#include <assert.h>
#include <stddef.h>
#include <wchar.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <new>
#if defined(WIN32)
	#define Strnicmp _strnicmp
#else
	#define Strnicmp strncasecmp
	#include <dirent.h>
	#include <sys/stat.h>
#endif

// Freetype includes
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftbitmap.h>
#include <freetype/ftcache.h>
#include <freetype/ftsizes.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <ftglyph.h>
#include <ftoutln.h>
#include <fttrigon.h>

#include "berialdraw.hpp"

#include "font/glyph.hpp"
#include "font/glyphs.hpp"
#include "font/_font.hpp"
#include "font/font.hpp"

#include "font/font_face.hpp"

#include "shape/renderer.hpp"
#include "shape/outline.hpp"

#include "vector/region.hpp"

#include "tool/tools.hpp"
#include "tool/item.hpp"
#include "tool/item_collection.hpp"
#include "tool/item_variant.hpp"
#include "tool/item_string.hpp"
#include "tool/item_int.hpp"
#include "tool/item_bool.hpp"
#include "tool/item_null.hpp"
#include "tool/item_array.hpp"
#include "tool/item_object.hpp"
#include "tool/bd_system.h"


namespace berialdraw
{
	inline int32_t min(int32_t X,int32_t Y)
	{
		return  ((X) < (Y) ? (X) : (Y));
	}
	inline int32_t max(int32_t X, int32_t Y) 
	{
		return ((X) > (Y) ? (X) : (Y));
	}
}

#undef SIZE_MAX
/// @endcond
#endif // berialdraw_imp_hpp_INCLUDED