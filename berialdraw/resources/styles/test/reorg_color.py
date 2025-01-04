import os
import fnmatch

def search(directory, patterns):
	matching_files = []
	for root, _, files in os.walk(directory):
		for pattern in patterns:
			for file in fnmatch.filter(files, pattern):
				matching_files.append(os.path.join(root, file))
	return matching_files

def replace(line, start, end, shift):
	found = False
	before = line
	for i in range(end,start-1,-1):
		if "0x%08X"%i in line:
			found = True
			line = line.replace("0x%08X"%i, "0x%08X"%(i+shift))

	if found:
		print(before.strip())
		print(line.strip())
	return line

def scan_file(filename, start, end, shift):
	lines = open(filename).readlines()
	lines_modified = []
	for line in lines:
		lines_modified.append(replace(line, start, end, shift))
	open(filename,"w").writelines(lines_modified)

def main():
	directory_to_search = "."
	patterns_to_search = ["*.json", "*.hpp"]
	filenames = search(directory_to_search, patterns_to_search)

	for filename in filenames:
		print(filename)
		scan_file(filename, 0x00000001, 0x0000002A, 1)
		print("")
		#~ break
		#~ print(file)
		
if __name__ == "__main__":
	main()
