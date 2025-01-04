#define FT_MAKE_OPTION_SINGLE_OBJECT

#include "../freetype/include/freetype/config/ftstdlib.h"

#undef FT_FILE
#undef ft_fclose
#undef ft_fopen
#undef ft_fread
#undef ft_fseek
#undef ft_ftell
#undef ft_scalloc
#undef ft_sfree
#undef ft_smalloc
#undef ft_srealloc

#define ft_scalloc   bd_calloc
#define ft_sfree     bd_free
#define ft_smalloc   bd_malloc
#define ft_srealloc  bd_realloc
#define FT_FILE      bd_FILE
#define ft_fclose    bd_fclose
#define ft_fopen     bd_fopen
#define ft_fread     bd_fread
#define ft_fseek     bd_fseek
#define ft_ftell     bd_ftell

#include "inc/tool/bd_system.h"

#include "../freetype/src/smooth/ftgrays.c"
#include "../freetype/src/smooth/ftsmooth.c"
#include "../freetype/src/base/ftsystem.c"

void * FtSystemMalloc(size_t size)
{
	return bd_malloc(size);
}

void FtSystemFree(void * block)
{
	bd_free(block);
}

void * FtSystemCalloc(size_t count, size_t size)
{
	return bd_calloc(count, size);
}

void * FtSystemRealloc(void * block, size_t size)
{
	return bd_realloc(block, size);
}

void* FtSystemFtMalloc(FT_Memory  memory, long       size)
{
	return FtSystemMalloc(size);
}

void FtSystemFtFree(FT_Memory  memory, void* block)
{
	FtSystemFree(block);
}

void* FtSystemFtRealloc(FT_Memory  memory, long       cur_size, long       new_size, void* block)
{
	return FtSystemRealloc(block, new_size);
}

struct  FT_MemoryRec_ FtSystemAllocator =
{
	0,
	FtSystemFtMalloc,
	FtSystemFtFree,
	FtSystemFtRealloc
};

FT_DEFINE_RASTER_FUNCS(
	ft_grays_vector_raster,
	FT_GLYPH_FORMAT_OUTLINE,
	(FT_Raster_New_Func)     gray_raster_new,       /* raster_new      */
	(FT_Raster_Reset_Func)   gray_raster_reset,     /* raster_reset    */
	(FT_Raster_Set_Mode_Func)gray_raster_set_mode,  /* raster_set_mode */
	(FT_Raster_Render_Func)  gray_raster_render,    /* raster_render   */
	(FT_Raster_Done_Func)    gray_raster_done       /* raster_done     */
)

static void draw_vector(int y, int count, const FT_Span * spans, void* worker)
{
	if(worker)
	{
		Renderer * master = (Renderer*)worker;
		master->paint(y, count, spans);
	}
}
