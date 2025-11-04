#!/usr/bin/env python3
"""
Test script for macOS - automatically finds and uses the compiled pyberialdraw library
"""
import time
import atexit
import sys
import os
import shutil

def find_pyberialdraw_lib():
    """Find the compiled pyberialdraw library"""
    possible_names = ['libpyberialdraw.so', 'pyberialdraw.so', 'libpyberialdraw.dylib']
    search_paths = [
        os.path.dirname(os.path.abspath(__file__)),  # Same directory as script
        os.path.join(os.path.dirname(os.path.abspath(__file__)), 'build'),
        os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'build'),
        '/Users/remi/Downloads/build',  # Your specific build path
        os.getcwd(),  # Current working directory
    ]
    
    for path in search_paths:
        if not os.path.exists(path):
            continue
        for name in possible_names:
            lib_path = os.path.join(path, name)
            if os.path.exists(lib_path):
                return lib_path, path
    return None, None

def setup_module():
    """Setup the pyberialdraw module for import"""
    lib_path, lib_dir = find_pyberialdraw_lib()
    
    if not lib_path:
        print("Error: Could not find pyberialdraw library!")
        print("Searched in:")
        search_paths = [
            os.path.dirname(os.path.abspath(__file__)),
            os.path.join(os.path.dirname(os.path.abspath(__file__)), 'build'),
            os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'build'),
            '/Users/remi/Downloads/build',
            os.getcwd(),
        ]
        for path in search_paths:
            print(f"  {path}")
        return False
    
    print(f"Found library: {lib_path}")
    
    # Copy library to current directory with correct name if needed
    current_dir = os.path.dirname(os.path.abspath(__file__))
    target_name = 'pyberialdraw.so'
    target_path = os.path.join(current_dir, target_name)
    
    if lib_path != target_path:
        print(f"Copying {lib_path} to {target_path}")
        shutil.copy2(lib_path, target_path)
    
    # Add to Python path
    sys.path.insert(0, current_dir)
    
    return True

# Setup the module
if not setup_module():
    sys.exit(1)

# Import the module
try:
    from pyberialdraw import *
    print("Successfully imported pyberialdraw module!")
except ImportError as e:
    print(f"Failed to import pyberialdraw: {e}")
    sys.exit(1)

def cleanup_on_exit():
    print("Cleaning up...")
    time.sleep(1)

atexit.register(cleanup_on_exit)

# Your original test code starts here
device = DeviceScreen("Sample python")

UIManager.init(device, 480, 880, Framebuf.ARGB8888, 2, "/tmp/berialdraw")  # Use Unix path

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

print("Starting main loop...")
UIManager.desktop().mainloop()