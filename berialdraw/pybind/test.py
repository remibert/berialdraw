import time
import atexit
import sys
import os
def cleanup_on_exit():
	print("end")
	time.sleep(1)

atexit.register(cleanup_on_exit)

sys.path.insert(0, r"Z:\tmp\pyberialdraw\x64\Debug")
from pyberialdraw import *

device = DeviceScreen("Sample python")

#UIManager.init(device, 480, 800, Framebuf.ARGB8888, 2, "./resources;../resources")
UIManager.init(device, 480, 800, Framebuf.ARGB8888, 2, "zip://resources.zip/resources")
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

class Dialog:
	def __init__(self):
		self.window = Window()
		self.scroll = ScrollView(self.window)
		self.layout = Column(self.scroll)
		self.label = Label(self.layout)
		self.label.text = "hello"

		self.first_name = Edit(self.layout)
		self.first_name.text = ""
		self.first_name.place_holder = "Test first name"
		self.first_name.on_key_down = self.on_key_pressed
		self.id = 1234

		self.last_name = Edit(self.layout)
		self.last_name.text = ""
		self.last_name.place_holder = "Test last name"
		self.last_name.on_key_down = self.on_key_pressed

		self.age = Edit(self.layout)
		self.age.text = "Age"
		self.age.place_holder = ""
		self.age.on_key_down = self.on_key_pressed

		self.slider = Slider(self.layout)
		self.slider.on_click = lambda widget, event: print(f"Slider clicked at {event.position}")
		self.slider.on_key_down = self.on_key_pressed

		self.sliderv = Slider(self.layout)
		self.sliderv.on_click = lambda widget, event: print(f"Slider clicked at {event.position}")
		self.sliderv.on_key_down = self.on_key_pressed
		self.sliderv.extend = Extend.EXTEND_HEIGHT
		self.sliderv.min_size = (100,100)

		self.button = Button(self.layout)
		self.button.text = "hello\nworld"
		self.button.margin = (20,10)
		self.button.on_click = self.on_click_button
		self.button.on_key_down = self.on_key_pressed

		self.switch = Switch(self.layout)
		self.switch.on_click = lambda widget, event: print(f"Switch clicked")
		self.switch.on_key_down = self.on_key_pressed
		
	def on_click_button(self, widget, event):
		print(f"Click! Button '{widget.text}' at position {event.position}")

	def on_key_pressed(self, widget, event):
		print(f"Key on {widget.classname} {key_to_str(event.key)} {event.state} {event.modifier}")


class Dialog2:
	def __init__(self):
		self.window = Window()
		self.window.color = Color.RED
		self.first_name = Edit(self.window)
		self.first_name.text = "afa"
		#UIManager.notifier().log()
		script = [
			{'type':'key','key':8592,'state':'down'},
			{'type':'key','key':8592,'state':'up'  },
			{'type':'key','key':8594,'state':'down'},
			{'type':'key','key':8594,'state':'up'  },
			{'type':'key','key':8594,'state':'down'},
			{'type':'key','key':8594,'state':'up'  }
		]
		UIManager.notifier().play_script(script)

dlg = Dialog()
print("hello")
UIManager.desktop().mainloop()
