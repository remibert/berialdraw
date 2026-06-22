def convert_file(input_path, output_path):
	replacements = {
		'stroke-width:40': 'stroke-width:52',
		'stroke-width:60': 'stroke-width:78',
		'stroke-width:75': 'stroke-width:97',
		'horiz-adv-x="0,00"': '',
		'horiz-adv-x="0.00"': ''
	}

	with open(input_path, 'r', encoding='utf-8') as infile:
		content = infile.read()

	for old, new in replacements.items():
		content = content.replace(old, new)

	with open(output_path, 'w', encoding='utf-8') as outfile:
		outfile.write(content)


if __name__ == "__main__":
	input_file = "Cerial.svg"
	output_file = "Cerial-bold-new.svg"
	convert_file(input_file, output_file)
