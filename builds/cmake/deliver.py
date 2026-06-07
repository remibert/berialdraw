#!/usr/bin/env python3
"""
Delivery script: Copy build artifacts and source files to delivery/ directory
CMake variables are substituted at configure time in the section below
"""
import os
import shutil
import glob
import platform
import subprocess

# Build configuration (substituted by CMake at configure time)
CMAKE_SOURCE_DIR = '@CMAKE_SOURCE_DIR@'
CMAKE_CURRENT_SOURCE_DIR = '@CMAKE_CURRENT_SOURCE_DIR@'
CMAKE_CURRENT_BINARY_DIR = '@CMAKE_CURRENT_BINARY_DIR@'
BD_LIB = '@BD_LIB@'

# List of copy jobs: (source, destination, wildcard, filter_function)
copy_jobs = [
	(CMAKE_SOURCE_DIR + '/resources',              'delivery/resources', '**/*', lambda path, name: 'test' not in path.split(os.sep) and not name.startswith('test_')),
	(CMAKE_CURRENT_SOURCE_DIR + '/include',        'delivery/include', '**/*', None),
	(CMAKE_CURRENT_SOURCE_DIR + '/samples',        'delivery/samples', '**/*', None),
	(CMAKE_CURRENT_BINARY_DIR + '/lib' + BD_LIB + '.a',  'delivery/lib/%s/' % (platform.system().lower().replace('darwin','macos')), None, None),
	(CMAKE_CURRENT_BINARY_DIR + '/samples',        'delivery/app', '*', lambda path, name: os.access(os.path.join(path, name), os.X_OK) and os.path.isfile(os.path.join(path, name))),
	(CMAKE_CURRENT_BINARY_DIR + '/pybind',         'delivery/python', 'pyberialdraw*.so', lambda path, name: 'CMakeFiles' not in path.split(os.sep)),
	(CMAKE_CURRENT_SOURCE_DIR + '/pybind/test.py', 'delivery/python/test.py', None, None),
]

def copy_with_filter(src, dst, wildcard=None, filter_func=None):
	"""Copy files from src to dst with optional filtering"""
	if os.path.isdir(src):
		if wildcard is None:
			wildcard = '**/*'
		for root, dirs, files in os.walk(src):
			# Skip CMakeFiles directory for all jobs
			dirs[:] = [d for d in dirs if d != 'CMakeFiles' and d != '.git']
			rel_root = os.path.relpath(root, src)
			for name in files:
				src_file = os.path.join(root, name)
				rel_path = os.path.normpath(os.path.join(rel_root, name))
				dst_file = os.path.join(dst, rel_path)
				if filter_func is None or filter_func(root, name):
					os.makedirs(os.path.dirname(dst_file), exist_ok=True)
					shutil.copy2(src_file, dst_file)
	elif wildcard:
		for src_file in glob.glob(os.path.join(src, wildcard), recursive=True):
			# Skip any file inside CMakeFiles or .git
			if 'CMakeFiles' in src_file.split(os.sep) or '.git' in src_file.split(os.sep):
				continue
			if os.path.isfile(src_file):
				rel_path = os.path.relpath(src_file, src)
				dst_file = os.path.join(dst, rel_path)
				os.makedirs(os.path.dirname(dst_file), exist_ok=True)
				shutil.copy2(src_file, dst_file)
	else:
		os.makedirs(os.path.dirname(dst), exist_ok=True)
		shutil.copy2(src, dst)

def generate_doxygen_if_needed():
	"""Generate Doxygen documentation into delivery/doc using delivery/include and delivery/samples as input"""
	# All paths are relative to CWD = CMAKE_CURRENT_BINARY_DIR
	doc_index  = os.path.join('delivery', 'doc', 'html', 'index.html')
	input_dirs_include = os.path.abspath('delivery/include')
	input_dirs_samples = os.path.abspath('delivery/samples')

	# Check if documentation is up to date
	needs_regeneration = True
	if os.path.exists(doc_index):
		doc_time = os.path.getmtime(doc_index)
		needs_regeneration = False
		
		# Check include directory
		for root, dirs, files in os.walk(input_dirs_include):
			for name in files:
				if name.endswith(('.hpp', '.h')):
					if os.path.getmtime(os.path.join(root, name)) > doc_time:
						needs_regeneration = True
						break
		
		# Check samples directory
		if not needs_regeneration and os.path.exists(input_dirs_samples):
			for root, dirs, files in os.walk(input_dirs_samples):
				for name in files:
					if name.endswith(('.hpp', '.h', '.cpp')):
						if os.path.getmtime(os.path.join(root, name)) > doc_time:
							needs_regeneration = True
							break

	if not needs_regeneration:
		print('[Delivery] Documentation is up to date, skipping generation')
		return

	if not os.path.exists(input_dirs_include):
		print('[Delivery] Warning: delivery/include not found, skipping documentation')
		return

	print('[Delivery] Generating Doxygen documentation...')
	doxygen_exe = shutil.which('doxygen')
	if not doxygen_exe:
		print('[Delivery] Warning: Doxygen not installed, skipping documentation')
		return
	
	# Check for pdflatex if PDF generation is needed
	pdflatex_exe = shutil.which('pdflatex')
	if not pdflatex_exe:
		print('[Delivery] Warning: pdflatex not found (install MacTeX to generate PDF)')

	source_dox = CMAKE_SOURCE_DIR + '/doc/doxygen.dox'
	temp_dox   = os.path.abspath('delivery_doxygen.dox')
	out_dir    = os.path.abspath('delivery/doc')

	if not os.path.exists(source_dox):
		print('[Delivery] Warning: ' + source_dox + ' not found, skipping documentation')
		return

	# Copy doxygen.dox into delivery/ (no spaces in path) so @INCLUDE works reliably
	os.makedirs(out_dir, exist_ok=True)
	delivery_dox = os.path.abspath('delivery/doxygen.dox')
	shutil.copy2(source_dox, delivery_dox)

	# Create delivery/doc/images/ and copy sample*.svg and sample*.cpp there
	doc_images_dir = os.path.join(out_dir, 'images')
	os.makedirs(doc_images_dir, exist_ok=True)
	
	# Copy sample*.svg from test/snapshot/ref
	snapshot_dir = CMAKE_SOURCE_DIR + '/test/snapshot/ref'
	if os.path.exists(snapshot_dir):
		for file in glob.glob(os.path.join(snapshot_dir, 'sample_*.svg')):
			shutil.copy2(file, doc_images_dir)
	
	# Copy sample*.cpp from delivery/samples
	if os.path.exists(input_dirs_samples):
		for file in glob.glob(os.path.join(input_dirs_samples, 'sample_*.cpp')):
			shutil.copy2(file, doc_images_dir)

	# Build INPUT list: include both delivery/include and delivery/samples
	input_list = input_dirs_include
	if os.path.exists(input_dirs_samples):
		input_list += ' ' + input_dirs_samples

	# Use delivery/doc/images/ for Doxygen examples and images
	doc_images_dir_abs = os.path.abspath(os.path.join(out_dir, 'images'))

	# Write a temporary override config that @INCLUDE the copied doxygen.dox
	# and overrides OUTPUT_DIRECTORY + INPUT with build-directory paths (no spaces).
	doxygen_include_key = '@' + 'INCLUDE'
	lines = [
		doxygen_include_key + '               = ' + delivery_dox,
		'OUTPUT_DIRECTORY       = ' + out_dir,
		'INPUT                  = ' + input_list,
		'EXAMPLE_PATH           = ' + doc_images_dir_abs,
		'IMAGE_PATH             = ' + doc_images_dir_abs,
		'RECURSIVE              = YES',
		'EXAMPLE_RECURSIVE      = YES',
	]
	with open(temp_dox, 'w') as f:
		f.write('\n'.join(lines) + '\n')

	try:
		result = subprocess.run([doxygen_exe, temp_dox],
					   cwd=CMAKE_CURRENT_BINARY_DIR,
					   check=False,
					   capture_output=True,
					   text=True)
		if result.returncode != 0:
			print('[Delivery] Warning: Doxygen returned exit code ' + str(result.returncode))
			if result.stderr:
				print('[Delivery] Doxygen stderr:')
				for line in result.stderr.split('\n'):
					if line.strip():
						print('  ' + line)
		print('[Delivery] Doxygen documentation generated -> ' + out_dir + '/html/')
		if pdflatex_exe:
			latex_dir = os.path.join(out_dir, 'latex')
			if os.path.exists(os.path.join(latex_dir, 'refman.pdf')):
				print('[Delivery]   PDF generated -> ' + latex_dir + '/refman.pdf')
			else:
				print('[Delivery]   LaTeX generated but PDF compilation may have failed')

		# Write a redirect index.html at delivery/doc/ so opening the folder
		# immediately lands on the documentation without browsing into html/
		redirect_html = os.path.join(out_dir, 'index.html')
		html_content = """<!DOCTYPE html>
<html>
<head>
<meta http-equiv="refresh" content="0;url=html/index.html"/>
<title>Berialdraw Documentation</title>
</head>
<body>
<p>
<a href="html/index.html">Berialdraw Documentation</a>
</p>
</body>
</html>"""
		with open(redirect_html, 'w') as f:
			f.write(html_content)
	except Exception as e:
		print('[Delivery] Warning: Doxygen generation failed: ' + str(e))


def main():
	"""Main entry point"""
	# Copy resources first (so delivery/include is populated before doc generation)
	for job in copy_jobs:
		src, dst, wildcard, filter_func = job
		if os.path.exists(src):
			copy_with_filter(src, dst, wildcard, filter_func)

	# Generate documentation after includes and samples are copied
	generate_doxygen_if_needed()

	# Check if pdflatex is available for PDF generation
	if not shutil.which('pdflatex'):
		print('[Delivery] Note: pdflatex not found. To generate PDF documentation:')
		print('[Delivery]   macOS: brew install mactex-small')
		print('[Delivery]   Or: brew install mactex (includes more fonts)')
		print('[Delivery]   Then rebuild with: cmake --build build')

	print('[Delivery] Build complete!')
	print('[Delivery] Output directory: delivery/')

if __name__ == '__main__':
	main()
