"""Python port of samples/main.cpp"""
from pyberialdraw import *
from samples.samples import sample_icon_menu, sample_menu

ZOOM = 2
SCREEN_WIDTH = 480 * ZOOM
SCREEN_HEIGHT = 480 * ZOOM


def main():
    # Initializes the UI manager with a new DeviceScreen named "Samples BerialDraw",
    # setting the screen width, height, pixel format, and zoom size
    # IMPORTANT: Keep device reference to prevent garbage collection
    device = DeviceScreen("Samples BerialDraw")
    UIManager.init(device, SCREEN_WIDTH, SCREEN_HEIGHT,
                   Framebuf.ARGB8888, ZOOM, "../resources;./resources")

    # Starts the user event logger, useful if you want to script user interface tests
    # UIManager.notifier().log()

    # Execute menu with all samples
    sample_icon_menu()
    sample_menu()

    # Terminates use of the library, destroys all objects and frees the allocated memory
    UIManager.deinit()


if __name__ == "__main__":
    main()
