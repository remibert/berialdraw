#include "berialdraw.hpp"
#include "samples/samples.hpp"

// Screen resolution
const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 480;

using namespace berialdraw;

#if defined(WIN32)
int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int i_cmd_show)
#else
int main(int argc, char* args[])
#endif
{
	// Initializes the UI manager with a new DeviceScreen named "Samples BerialDraw", 
	// setting the screen width 480, height 480, pixel format, and zoom size
	UIManager::init(new DeviceScreen("Samples BerialDraw"), SCREEN_WIDTH, SCREEN_HEIGHT, Framebuf::ARGB8888, 2);

	// Execute menu with all samples
	//sample_icon_menu();
	sample_menu();

	// Terminates use of the library, destroys all objects and frees the allocated memory
	UIManager::deinit();

	return 0;
}