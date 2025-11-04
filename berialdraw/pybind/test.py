import time
import atexit
import sys
import os

# Add current directory to Python path for module discovery
current_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, current_dir)

# Try different possible paths for the compiled module
possible_paths = [
    current_dir,  # Current directory
    os.path.join(current_dir, "build"),  # build subdirectory
    os.path.join(current_dir, "..", "build"),  # parent build directory
    r"Z:\tmp\pyberialdraw\x64\Debug",  # Windows debug path
]

# Add all possible paths
for path in possible_paths:
    if os.path.exists(path):
        sys.path.insert(0, path)

try:
    from pyberialdraw import *
    print("Successfully imported pyberialdraw module")
except ImportError as e:
    print(f"Failed to import pyberialdraw: {e}")
    print("Current working directory:", os.getcwd())
    print("Python path:", sys.path[:5])  # Show first 5 paths
    print("Looking for module files:")
    for path in possible_paths:
        if os.path.exists(path):
            files = [f for f in os.listdir(path) if 'pyberialdraw' in f]
            if files:
                print(f"  {path}: {files}")
    sys.exit(1)

def cleanup_on_exit():
	print("end")
	time.sleep(1)

atexit.register(cleanup_on_exit)

device = DeviceScreen("Sample python")

UIManager.init(device, 480, 880, Framebuf.ARGB8888, 2, "B:/berialdraw")

UIManager.style = "pearl"
UIManager.appearance = "light"
UIManager.theme = THEME_LIME

# Helper function to safely convert key to string
def key_to_str(key):
	if isinstance(key, str):
		return key
	try:
		return chr(key) if 32 <= key <= 126 else f"[{key}]"
	except:
		return f"[{key}]"

def on_click_button(widget, event):
	print(f"Click! Button '{widget.text}' at position {event.position}")

def on_key_pressed(widget, event):
	print(f"Key on {widget.classname} {key_to_str(event.key)} {event.state} {event.modifier}")


window = Window()
layout = Column(window)
label = Label(layout)
label.text = "hello"

first_name = Edit(layout)
first_name.text = ""
first_name.place_holder = "Test first name"
first_name.on_key_down = on_key_pressed

last_name = Edit(layout)
last_name.text = ""
last_name.place_holder = "Test last name"
last_name.on_key_down = on_key_pressed

age = Edit(layout)
age.text = "Age"
age.place_holder = ""
age.on_key_down = on_key_pressed

slider = Slider(layout)
slider.on_click = lambda widget, event: print(f"Slider clicked at {event.position}")
slider.on_key_down = on_key_pressed

button = Button(layout)
button.text = "world"
button.margin = (20,10)
button.on_click = on_click_button
button.on_key_down = on_key_pressed

switch = Switch(layout)
switch.on_click = lambda widget, event: print(f"Switch clicked")
switch.on_key_down = on_key_pressed

#for i  in range(100):
#	UIManager.desktop().dispatch()
UIManager.desktop().mainloop()

