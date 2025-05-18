import os
import zipfile
import fnmatch
from datetime import datetime

def zip_directory(directory_path, output_dir, excluded):
	timestamp = datetime.now().strftime("%d-%m-%Y-%H-%M-%S")
	directory_name = os.path.basename(directory_path.rstrip('/\\'))
	zip_path = os.path.join(output_dir, f"{directory_name}_{timestamp}.zip")
	with zipfile.ZipFile(zip_path, 'w', zipfile.ZIP_DEFLATED) as zipf:
		for root, dirs, files in os.walk(directory_path):
			for file in files:
					file_path = os.path.normpath(os.path.join(root, file))
					if not any(fnmatch.fnmatch(file_path, pattern) for pattern in excluded):
						print("+",file_path)
						arcname = os.path.relpath(file_path, directory_path)
						zipf.write(file_path, arcname)
					else:
						print("-",file_path)
	print(f"zip created : {zip_path}")

if __name__ == "__main__":
	directory_to_zip = '../berialdraw'
	output_directory = '..'
	excluded = [
		'*/.git/*',
		'*/.vs/berialdraw/*',
		'*/builds/*',
		'*/build/*',
		'*/html/*',
		'*/out/*',
		'*/docs/*',
		'*.log',
		'*.tmp',
		'*/.DS_Store',
		'*.db'
		]
	zip_directory(directory_to_zip, output_directory, excluded)
