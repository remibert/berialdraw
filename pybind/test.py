import time
import sys
import os

sys.path.insert(0, r"Z:\tmp\pyberialdraw\x64\Release")
try:
	from pyberialdraw import *
except ImportError as err:
	import time
	print(f"Import error: {err}")
	time.sleep(1)

device = DeviceScreen("Sample python")

device.position = (100,100)

UIManager.init(device, 480, 800, Framebuf.ARGB8888, 2, "./resources;../resources")
#UIManager.init(device, 480, 800, Framebuf.ARGB8888, 2, "zip://resources.zip/resources")

UIManager.style = "pearl"
UIManager.appearance = "light"
UIManager.palette = PALETTE_LIME

# Helper function to safely convert key to string
def key_to_str(key):
	if isinstance(key, str):
		return key
	try:
		if 32 <= key <= 126:
			return chr(key)
		else:
			return f"[U+{key:04X}]"
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

		self.radio1 = Radio(self.layout)
		self.radio1.text = "Radio1"
		self.radio1.group = "group1"

		self.radio2 = Radio(self.layout)
		self.radio2.text = "Radio2"
		self.radio2.group = "group1"
		self.radio2.checked = True

		self.checkbox = Checkbox(self.layout)
		self.checkbox.text = "Checkbox"
		self.checkbox.checked = True

		self.switch = Switch(self.layout)
		self.switch.text = "Switch"
		self.switch.checked = True
		self.switch.on_click = lambda widget, event: print(f"Switch clicked")
		self.switch.on_key_down = self.on_key_pressed

		# Table View Example
		self.table_label = Label(self.layout)
		self.table_label.text = "Table View Example"

		self.table = TableView(self.layout)
		self.table.extend = Extend.EXTEND_ALL
		self.table.min_size = (400, 150)
		
		# Charger les données du tableau avec la méthode load()
		table_data = [
			["Name", "First Name", "Age", "City"],
			["Dupont", "Jean", "28", "Paris"],
			["Martin", "Marie", "35", "Lyon"],
			["Bernard", "Pierre", "42", "Marseille"],
			["Thomas", "Sophie", "31", "Toulouse"],
			["Robert", "Luc", "55", "Nice"],
		]
		self.table.load(table_data)
		self.table.on_click = self.on_table_row_clicked

		# Picture View Example
		self.picture_label = Label(self.layout)
		self.picture_label.text = "Picture Example"

		self.png_picture = Picture(self.layout)
		self.png_picture.filename = "$(ui.images)/house.png"
		self.png_picture.fit_mode = ImageFitMode.FIT
		self.png_picture.margin = (10, 10)

		self.jpg_picture = Picture(self.layout)
		self.jpg_picture.filename = "$(ui.images)/filleperle.jpg"
		self.jpg_picture.fit_mode = ImageFitMode.FIT
		self.jpg_picture.margin = (10, 10)

		# List Example
		self.list_label = Label(self.layout)
		self.list_label.text = "List Widget Example"

		self.my_list = List(self.layout)
		#~ self.my_list.extend = Extend.EXTEND_WIDTH
		#~ self.my_list.min_size = (400, 150)

		#~ # Add single items
		#~ self.my_list.append("Item 1")
		#~ self.my_list.append("Item 2")

		#~ # Add multiple items with a list
		#~ self.my_list.append(["Item 3", "Item 4", "Item 5"])

		#~ # Add items with custom configuration
		#~ self.my_list.append(lambda item: (
			#~ item.set_text("Item 6 (custom)"),
			#~ item.set_color(0xFF6B6B)
		#~ ))

		#~ # Add items at specific positions
		#~ self.my_list.insert(0, "First Item")
		#~ self.my_list.insert(-1, "Before Last")

		#~ # Access items using array syntax
		#~ first_item = self.my_list[0]
		#~ last_item = self.my_list[-1]
		#~ second_to_last = self.my_list[-2]

		#~ # Access using at()
		#~ item_at_2 = self.my_list.at(2)

		#~ # Get list info
		#~ total_items = self.my_list.count()
		#~ is_empty = self.my_list.is_empty()

		#~ print(f"List has {total_items} items")
		#~ print(f"Is empty: {is_empty}")

		#~ # Connect event handler
		#~ self.my_list.on_click = self.on_list_item_clicked

	def on_click_button(self, widget, event):
		print(f"Click! Button '{widget.text}' at position {event.position}")
		# Add single items
		self.my_list.append("Item 1")
		self.my_list.append("Item 2")

	def on_list_item_clicked(self, widget, event):
		print(f"List item clicked at position {event.position}")
		

	def on_key_pressed(self, widget, event):
		classname_hex = ''.join(f'U+{ord(c):04X}' if ord(c) > 127 else c for c in widget.classname)
		state_hex = ''.join(f'U+{ord(c):04X}' if ord(c) > 127 else c for c in str(event.state))
		modifier_hex = ''.join(f'U+{ord(c):04X}' if ord(c) > 127 else c for c in str(event.modifier))
		print(f"Key on {classname_hex} {key_to_str(event.key)} {state_hex} {modifier_hex}")

	def on_table_row_clicked(self, widget, event):
		print(f"Table row clicked at position {event.position}")


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
# Décommenter l'une des lignes suivantes pour tester Dialog2 ou Dialog3:
# dlg = Dialog2()
# dlg = Dialog3()
UIManager.desktop().mainloop()
