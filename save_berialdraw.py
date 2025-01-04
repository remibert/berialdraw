import os
import zipfile
from datetime import datetime

def zip_directory(directory_path, output_dir, excluded_extensions, excluded_directories):
	timestamp = datetime.now().strftime("%d-%m-%Y-%H-%M-%S")

	directory_name = os.path.basename(directory_path.rstrip('/\\'))
	zip_name = f"{directory_name}_{timestamp}.zip"
	zip_path = os.path.join(output_dir, zip_name)

	with zipfile.ZipFile(zip_path, 'w', zipfile.ZIP_DEFLATED) as zipf:
		for root, dirs, files in os.walk(directory_path):
			for d in excluded_directories:
				if d in dirs:
					dirs.remove(d)

			for file in files:
				if not any(file.endswith(ext) for ext in excluded_extensions):
					file_path = os.path.join(root, file)
					arcname = os.path.relpath(file_path, directory_path)
					print("Adding %s"%arcname)
					zipf.write(file_path, arcname)

	print(f"zip created : {zip_path}")

if __name__ == "__main__":
	directory_to_zip = '../berialdraw'
	output_directory = '..'
	excluded_dir = ['.git','.vs','builds','build','html','out','docs']
	excluded_ext = ['.git', '.log','.tmp','.DS_Store','.db']
	zip_directory(directory_to_zip, output_directory, excluded_ext, excluded_dir)
