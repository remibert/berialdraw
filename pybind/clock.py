import sys
import os
import time
from datetime import datetime
import threading

sys.path.insert(0, r"Z:\tmp\pyberialdraw\x64\Release")

try:
	from pyberialdraw import *
except ImportError:
	print("Erreur: Impossible d'importer pyberialdraw")
	print("Vérifiez que le chemin sys.path est correct")
	sys.exit(1)


class Clock:
	def __init__(self):
		self.device = DeviceScreen("Clock")
		self.device.position = (100, 100)
		#~ UIManager.init(self.device, 1080, 200, Framebuf.ARGB8888, 2, "zip://resources.zip/resources")
		UIManager.init(self.device, 1080, 200, Framebuf.ARGB8888, 2, "./resources;../resources")
		UIManager.style = "pearl"
		UIManager.appearance = "light"
		UIManager.palette = PALETTE_BLUE
		
		self.window = Window()
		self.window.color = Color.WHITE
		
		self.layout = Column(self.window)
		self.layout.extend = Extend.EXTEND_ALL
		
		self.time_label = Label(self.layout)
		self.time_label.text = "00:00:00:000"
		self.time_label.font_size = 80
		self.time_label.extend = Extend.EXTEND_ALL
		self.time_label.color = Color.BLUE
		
		self.timer = Timer(self.window)
		self.timer.interval = 10
		self.timer.recurring = True
		self.timer.on_timeout = self.on_timer_tick
		self.timer.start()


		self.running = True
		#~ self.update_thread = threading.Thread(target=self.update_loop, daemon=True)
		#~ self.update_thread.start()

	def on_timer_tick(self, widget, event):
		self.time_label.text = self.get_time()
	
	def get_time(self):
		now = datetime.now()
		return f"{now.hour:02d}:{now.minute:02d}:{now.second:02d}:{now.microsecond // 1000:03d}"
	
	def run(self):
		"""Lancer la boucle principale"""
		UIManager.desktop().mainloop()
		self.running = False

	def update_loop(self):
		"""Boucle de mise à jour toutes les millisecondes"""
		while self.running:
			print(self.get_time())

if __name__ == "__main__":
	clock = Clock()
	clock.run()
