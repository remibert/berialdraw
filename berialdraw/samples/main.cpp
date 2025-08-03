#include "berialdraw.hpp"
#include "samples/samples.hpp"

// Screen resolution
const int ZOOM = 2;
const int SCREEN_WIDTH  = 480*ZOOM;
const int SCREEN_HEIGHT = 480*ZOOM;

using namespace berialdraw;

int main(int argc, char* args[])
{
	// Initializes the UI manager with a new DeviceScreen named "Samples BerialDraw", 
	// setting the screen width 480, height 480, pixel format, and zoom size
	UIManager::init(new DeviceScreen("Samples BerialDraw"), SCREEN_WIDTH, SCREEN_HEIGHT, Framebuf::ARGB8888, ZOOM);

	// Execute menu with all samples
	sample_icon_menu();
	sample_menu();

	// Terminates use of the library, destroys all objects and frees the allocated memory
	UIManager::deinit();

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