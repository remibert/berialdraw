import glob
import os.path
import os
import subprocess
import platform

os_name = platform.system()
if os_name == "Darwin":
	INKSCAPE="/Applications/Inkscape.app/Contents/MacOS/inkscape"
elif os_name == "Linux":
	INKSCAPE="inkscape"
elif os_name == "Windows":
	INKSCAPE=r"Z:\Portable\Graphique\Inkscape\bin\inkscape.exe"


def get_lines(filename):
	result = []
	if os.path.exists(filename):
		lines = open(filename,"r").readlines()
		for line in lines:
			result.append(line.strip())
	return result

def to_png(filename):
	name = os.path.splitext(filename)[0]
	png_filename = name + ".png"

	modified = True
	if os.path.exists(png_filename):
		if os.path.getmtime(png_filename) > os.path.getmtime(filename):
			modified = False
			
	command = [INKSCAPE,'--export-type=png','%s'%filename,'--export-filename=%s'%png_filename]

	if modified:
		print(command)
		result = subprocess.run(command, capture_output=True, text=True)

def get_ref_filename(name):
	directory, filename = os.path.split(name)
	base = os.path.split(directory)[0]
	return os.path.join(base, "ref", filename)

def check_test():
	for out in glob.glob("out/*.svg"):
		ref = get_ref_filename(out)
		if get_lines(out) != get_lines(ref):
			print("! '%s' != '%s'"%(ref,out))
			to_png(out)
			to_png(ref)

if __name__ == "__main__":
	check_test()
