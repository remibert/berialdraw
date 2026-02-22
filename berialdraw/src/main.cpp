#include "berialdraw.hpp"
#include "samples/samples.hpp"

// Screen resolution
const int ZOOM = 2;
const int SCREEN_WIDTH  = 480*ZOOM;
const int SCREEN_HEIGHT = 480*ZOOM;

using namespace berialdraw; 

int main(int argc, char* args[])
{
	// Start memory leak tracer
	MemoryLeakTracer::start();

	// Stop the debugger while allocating the memory block with the specified id.
	// Very useful for finding a memory leak
	//MemoryLeakTracer::break_at(42327);
	{
		// Execute unitary test
		test_unitary();

		// Start and init berial draw library
		UIManager::init(new DeviceScreen("Test BerialDraw"), SCREEN_WIDTH, SCREEN_HEIGHT, Framebuf::ARGB8888,ZOOM, "../resources;./resources");

		// Select the CRC checks for the screens to verify whether the tests are successful or not.
		UIManager::screen_crc()->open("$(ui.tests)/out/all_crcs.json","$(ui.tests)/ref/all_crcs.json");

		// Starts the user event logger, useful if you want to script user interface tests
		//UIManager::notifier()->log();

		// Execute scripted user interface test every widgets
		test_widget();

		// Execute scripted user interface test on samples
		test_samples();

		// Stop library
		UIManager::deinit();
	}

	// Check hello world sample
	sample_hello_world();

	// Check if memory leak detected
	if (MemoryLeakTracer::count() > 0)
	{
		/** !!! WARNING WITH THE SDL LIBRARY WE DETECTED MANY MEMORY LEAKS,
		AFTER RESEARCH IT WAS NOT POSSIBLE TO CORRECT THEM, BECAUSE THEY ARE IN SDL.
		TO DETECT MEMORY LEAKS YOU HAVE TO RUN THE SOFTWARE IN WIN64.  !!! */

		// Displays all memory blocks that have not been deallocated with their identifiers
		MemoryLeakTracer::show();
		#if defined(WIN32)
			breakpoint();
		#endif
	}
	return 0;
}


#if defined(_WIN32)/* && !defined(_CONSOLE)*/
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int argc = __argc;
	char** argv = __argv;
	return main(argc, argv);
}
#endif

/*
/Users/remi/Downloads/esp32/firmware/micropython-s3/ports/unix/modsocket.c:466:    return mp_vfs_open(n_args, new_args, (mp_map_t *)&mp_const_empty_map); 
/Users/remi/Downloads/esp32/firmware/micropython-s3/py/stream.h:114:mp_uint_t mp_stream_rw(mp_obj_t stream, void *buf, mp_uint_t size, int *errcode, byte flags);
/Users/remi/Downloads/esp32/firmware/micropython-s3/py/builtin.h:44:mp_obj_t mp_vfs_open(uint32_t n_args, const mp_obj_t *args, mp_map_t *kwargs);
/Users/remi/Downloads/esp32/firmware/micropython-s3/extmod/vfs.c:300:mp_obj_t mp_vfs_open(uint32_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
/Users/remi/Downloads/esp32/firmware/micropython-s3/extmod/vfs.h:93:mp_obj_t mp_vfs_open(uint32_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);

mp_obj_t mp_vfs_open(uint32_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

#define MP_STREAM_RW_READ  0
#define MP_STREAM_RW_WRITE 2
#define MP_STREAM_RW_ONCE  1
mp_uint_t mp_stream_rw(mp_obj_t stream, void *buf, mp_uint_t size, int *errcode, byte flags);
#define mp_stream_write_exactly(stream, buf, size, err) mp_stream_rw(stream, (byte *)buf, size, err, MP_STREAM_RW_WRITE)
#define mp_stream_read_exactly(stream, buf, size, err) mp_stream_rw(stream, buf, size, err, MP_STREAM_RW_READ)

*/
