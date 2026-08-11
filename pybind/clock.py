import sys
import os
import time
from datetime import datetime
import threading
import atexit

def cleanup_on_exit():
	print("Horloge terminée")

atexit.register(cleanup_on_exit)

# Adapter le chemin selon votre plateforme
# Windows: sys.path.insert(0, r"Z:\tmp\pyberialdraw\x64\Debug")
# macOS/Linux: sys.path.insert(0, "/path/to/build/pyberialdraw")
#~ sys.path.insert(0, r"Z:\tmp\pyberialdraw\x64\Debug")

try:
	from pyberialdraw import *
except ImportError:
	print("Erreur: Impossible d'importer pyberialdraw")
	print("Vérifiez que le chemin sys.path est correct")
	sys.exit(1)


class Clock:
	"""Horloge numérique avec mise à jour toutes les millisecondes"""
	
	def __init__(self):
		# Créer le device et UIManager
		print("[INIT] Démarrage du Clock")
		self.device = DeviceScreen("Clock")
		self.device.position = (100, 100)
		print("[INIT] UIManager.init()")
		UIManager.init(self.device, 1080, 200, Framebuf.ARGB8888, 2, "zip://resources.zip/resources")
		UIManager.style = "pearl"
		UIManager.appearance = "light"
		UIManager.palette = PALETTE_BLUE
		
		# Fenêtre principale
		self.window = Window()
		self.window.color = Color.WHITE
		
		# Layout principal (centré)
		self.layout = Column(self.window)
		self.layout.extend = Extend.EXTEND_ALL
		

		# Label pour afficher l'heure (TRÈS GRAND)
		self.time_label = Label(self.layout)
		self.time_label.text = "00:00:00:000"
		self.time_label.font_size = 80  # Police énorme
		self.time_label.extend = Extend.EXTEND_ALL
		self.time_label.color = Color.BLUE
		self.time_label.margin = (20, 20)
		
		# Flag pour arrêter la mise à jour
		self.running = True
		#~ self.window.on_key_down = self.on_key_pressed
		
		# Démarrer le thread de mise à jour
		print("[INIT] Lancement du thread de mise à jour")
		self.update_thread = threading.Thread(target=self.update_loop, daemon=True)
		self.update_thread.start()
		print(f"[INIT] Thread lancé: {self.update_thread.is_alive()}")
	
	def on_key_pressed(self, widget, event):
		"""Gestion des touches clavier"""
		# ESC (key code 27 ou 9216)
		if event.key == 27 or event.key == 9216:
			print("ESC pressé - Arrêt de l'horloge")
			self.running = False
			UIManager.desktop().stop()
	
	def update_loop(self):
		"""Boucle de mise à jour toutes les millisecondes"""
		print("[THREAD] update_loop démarré")
		while self.running:
			# Obtenir l'heure actuelle
			now = datetime.now()
			
			# Formater: HH:MM:SS:ms
			hours = f"{now.hour:02d}"
			minutes = f"{now.minute:02d}"
			seconds = f"{now.second:02d}"
			milliseconds = f"{now.microsecond // 1000:03d}"
			
			time_str = f"{hours}:{minutes}:{seconds}:{milliseconds}"
			
			# Mettre à jour le label
			self.time_label.text = time_str
	
			# Attendre 1ms avant la prochaine mise à jour
			time.sleep(0.01)
		print("[THREAD] update_loop terminé")
	
	def run(self):
		"""Lancer la boucle principale"""
		print("[RUN] Avant mainloop()")
		UIManager.desktop().mainloop()
		print("[RUN] Après mainloop() (devrait pas arriver ici si bloquant)")


# Créer et lancer l'horloge
if __name__ == "__main__":
	print("[MAIN] Création du Clock")
	clock = Clock()
	print("[MAIN] Appel de clock.run()")
	clock.run()
	print("[MAIN] clock.run() terminé")
