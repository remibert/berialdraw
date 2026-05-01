#!/usr/bin/python3
import sys
import subprocess

import shutil
import os

	
def execute(command):
	print("> " + command)
	pipe = subprocess.Popen(command, stdout=sys.stdout, stderr=sys.stderr, shell=True)
	lines = pipe.communicate()[0]

if __name__ == "__main__":
	if os.path.exists("html"):
		shutil.rmtree("html")
	#~ execute(r'"Z:\Source\Tools\Tetra\doxygen\doxygen.exe" doxygen.dox')
	execute(r'doxygen doxygen.dox')

