import time
import atexit
import sys
import os
def cleanup_on_exit():
	print("end")
	#time.sleep(1)

atexit.register(cleanup_on_exit)

sys.path.insert(0, r"Z:\tmp\pyberialdraw\x64\Debug")
from pyberialdraw import *

device = DeviceScreen("Sample python")
device.position = (100,100)
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
		#UIManager.clipboard = "toto-"

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
			["Nom", "Prénom", "Âge", "Ville"],
			["Dupont", "Jean", "28", "Paris"],
			["Martin", "Marie", "35", "Lyon"],
			["Bernard", "Pierre", "42", "Marseille"],
			["Thomas", "Sophie", "31", "Toulouse"],
			["Robert", "Luc", "55", "Nice"],
		]
		self.table.load(table_data)
		self.table.on_click = self.on_table_row_clicked

	def on_click_button(self, widget, event):
		print(f"Click! Button '{widget.text}' at position {event.position}")
		

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

class Dialog3:
	def __init__(self):
		self.window = Window()
		self.layout = Column(self.window)
		
		# Titre
		self.title = Label(self.layout)
		self.title.text = "Table View Example"
		
		# Créer une table view
		self.table = TableView(self.layout)
		self.table.extend = Extend.EXTEND_ALL
		
		# Définir les colonnes
		self.table.columns = ["Nom", "Prénom", "Âge", "Ville"]
		
		# Ajouter les données du tableau
		data = [
			["Dupont", "Jean", "28", "Paris"],
			["Martin", "Marie", "35", "Lyon"],
			["Bernard", "Pierre", "42", "Marseille"],
			["Thomas", "Sophie", "31", "Toulouse"],
			["Robert", "Luc", "55", "Nice"],
			["Richard", "Anne", "27", "Nantes"],
			["Petit", "Claude", "48", "Strasbourg"],
			["Durand", "Isabelle", "33", "Montpellier"],
		]
		
		for row in data:
			self.table.add_row(row)
		
		# Événements
		self.table.on_click = self.on_table_row_clicked
		self.table.on_key_down = self.on_key_pressed
		
		# Bouton pour afficher les détails de la sélection
		self.info_label = Label(self.layout)
		self.info_label.text = "Cliquez sur une ligne"

	def on_table_row_clicked(self, widget, event):
		print(f"Row clicked at position {event.position}")
		self.info_label.text = f"Sélection: {event.position}"

	def on_key_pressed(self, widget, event):
		print(f"Key on table: {key_to_str(event.key)}")


dlg = Dialog()
# Décommenter l'une des lignes suivantes pour tester Dialog2 ou Dialog3:
# dlg = Dialog2()
# dlg = Dialog3()
print("hello")
UIManager.desktop().mainloop()
