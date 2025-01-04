#coding:latin-1
# https://fonts.google.com/icons
from svgpathtools import svg2paths, Path, Line, CubicBezier, QuadraticBezier
import xml.etree.ElementTree as ET
from math import cos, sin, sqrt, pi, atan2, acos
import os
import glob

class SvgToIcn:
	def __init__(self, svg_file, size=1024.):
		self.svg_file = svg_file
		self.size = size

	def get_size(self, svg_file):
		tree = ET.parse(svg_file)
		root = tree.getroot()
		self.width = root.get("width")
		self.height = root.get("height")
		
		try:
			self.width = eval(self.width)
		except:
			self.width = None

		try:
			self.height = eval(self.height)
		except:
			self.height = None
		
		
	def get_viewbox(self,svg_file):
		"""
		Extracts the viewBox attribute from an SVG file.

		Args:
			svg_file (str): Path to the SVG file.

		Returns:
			tuple: A tuple containing the viewBox values (x, y, width, height).
		"""
		# Parse the SVG file to get the root element
		tree = ET.parse(svg_file)
		root = tree.getroot()

		# Get the viewBox attribute from the <svg> element
		viewbox_str = root.get('viewBox')
		
		self.color = 0
		self.color = self.get_default_color(root.get("fill"))

		if viewbox_str is not None:
			# Split the viewBox string into its components and convert to float
			viewbox = tuple(map(float, viewbox_str.split()))
			return viewbox
		return None  # Return None if no viewBox is found

	def convert_arc_to_bezier(self, x1, y1, x2, y2, rx, ry, phi, large_arc_flag, sweep_flag):
		"""
		Converts an elliptical arc defined by its start and end points, radii, rotation, and flags 
		into a series of quadratic Bezier curves.

		Args:
			x1 (float): x-coordinate of the start point of the arc.
			y1 (float): y-coordinate of the start point of the arc.
			x2 (float): x-coordinate of the end point of the arc.
			y2 (float): y-coordinate of the end point of the arc.
			rx (float): x-radius of the ellipse.
			ry (float): y-radius of the ellipse.
			phi (float): rotation of the ellipse in degrees.
			large_arc_flag (int): Flag indicating if the arc is a large arc (1) or a small arc (0).
			sweep_flag (int): Flag indicating if the arc should be drawn in a positive-angle direction (1) or a negative-angle direction (0).

		Returns:
			List[QuadraticBezier]: A list of QuadraticBezier curves that approximate the arc.
		"""
		phi_rad = pi * phi / 180.0  # Convert degrees to radians

		# Step 1: Compute the transformed coordinates
		dx = (x1 - x2) / 2.0
		dy = (y1 - y2) / 2.0
		x1_prime = cos(phi_rad) * dx + sin(phi_rad) * dy
		y1_prime = -sin(phi_rad) * dx + cos(phi_rad) * dy

		# Correct the radius if necessary
		rx = abs(rx)
		ry = abs(ry)
		radius_check = (x1_prime**2) / (rx**2) + (y1_prime**2) / (ry**2)
		if radius_check > 1:
			rx *= sqrt(radius_check)
			ry *= sqrt(radius_check)

		# Step 2: Compute the center of the ellipse
		sign = -1 if large_arc_flag == sweep_flag else 1
		s = ((rx**2 * ry**2 - rx**2 * y1_prime**2 - ry**2 * x1_prime**2) /
			 (rx**2 * y1_prime**2 + ry**2 * x1_prime**2))
		if s > 0.:
			sqrt_factor = sqrt(
				(rx**2 * ry**2 - rx**2 * y1_prime**2 - ry**2 * x1_prime**2) /
				(rx**2 * y1_prime**2 + ry**2 * x1_prime**2)
			)
		else:
			sqrt_factor = 0.
		
		cx_prime = sign * sqrt_factor * (rx * y1_prime) / ry
		cy_prime = sign * sqrt_factor * -(ry * x1_prime) / rx

		cx = cos(phi_rad) * cx_prime - sin(phi_rad) * cy_prime + (x1 + x2) / 2.0
		cy = sin(phi_rad) * cx_prime + cos(phi_rad) * cy_prime + (y1 + y2) / 2.0

		# Step 3: Compute the angles
		def angle(u, v):
			"""
			Computes the angle between two vectors u and v.

			Args:
				u (tuple): A 2D vector (x, y).
				v (tuple): A 2D vector (x, y).

			Returns:
				float: The angle in radians between vectors u and v.
			"""
			dot = u[0] * v[0] + u[1] * v[1]
			mag_u = sqrt(u[0]**2 + u[1]**2)
			mag_v = sqrt(v[0]**2 + v[1]**2)
			cos_angle = dot / (mag_u * mag_v)
			return acos(min(max(cos_angle, -1.0), 1.0))

		start_angle = atan2((y1_prime - cy_prime) / ry, (x1_prime - cx_prime) / rx)
		end_angle = atan2((-y1_prime - cy_prime) / ry, (-x1_prime - cx_prime) / rx)
		delta_angle = end_angle - start_angle
		
		# Adjust delta_angle based on the sweep flag
		if not sweep_flag and delta_angle > 0:
			delta_angle -= 2 * pi
		elif sweep_flag and delta_angle < 0:
			delta_angle += 2 * pi

		# Step 4: Split the arc into Bezier curves
		num_segments = max(int(abs(delta_angle) / (pi / 2)) + 1, 1)  # Determine the number of segments for approximation
		beziers = []

		for i in range(num_segments):
			t1 = i / num_segments
			t2 = (i + 1) / num_segments
			start_t = start_angle + t1 * delta_angle
			end_t = start_angle + t2 * delta_angle
			
			# Append a QuadraticBezier segment for each portion of the arc
			beziers.append(QuadraticBezier((x1, y1), (cx, cy), (x2, y2)))
		return beziers

	def to_64(self, point):
		"""
		Converts a point (tuple or complex) to a string representation 
		with scaled integer values.

		Args:
			point (tuple or complex): The point to convert. 
									   Can be a tuple (x, y) or a complex number (x + yi).

		Returns:
			str: A string representation of the point in the format "x,y".
		"""
		mult = (1. / max(self.width,self.height)) * self.size  # Scale factor for converting points
		if type(point) == type((0, 2)):  # Check if the point is a tuple
			return "%d,%d" % ((point[0] * mult), (point[1] * mult))
		else:  # Assume the point is a complex number
			return "%d,%d" % ((point.real * mult), (point.imag * mult))

	def get_bounding_boxes(self, paths):
		"""
		Computes the bounding boxes for a list of paths.

		Args:
			paths (List[Path]): A list of paths for which to compute bounding boxes.

		Returns:
			List[Dict]: A list of dictionaries containing the bounding box 
						 information for each path.
		"""
		bounding_boxes = []

		for i, path in enumerate(paths):
			xmin, xmax, ymin, ymax = path.bbox()  # Get bounding box coordinates
			width = xmax - xmin  # Calculate width
			height = ymax - ymin  # Calculate height
			
			# Append bounding box data to the list
			bounding_boxes.append({
				'path': i + 1,
				'xmin': xmin,
				'ymin': ymin,
				'xmax': xmax,
				'ymax': ymax,
				'width': width,
				'height': height
			})
		return bounding_boxes  # Return the list of bounding boxes

	def get_default_color(self, color):
		if color is not None and "none" not in color:
			if "#" in color:
				return eval("0x"+color[1:])
			else:
				return eval(color)
		else:
			return self.color
	
	def get_color(self, style):
		result = self.color
		opacity = 255
		if style is not None:
			if len(style) > 0:
				spl = style.split(";")
				for s in spl:
					if "fill:" in s:
						ss = s.split(":")
						result = self.get_default_color(ss[1])
					elif "fill-opacity:" in s:
						ss = s.split(":")[1]
						opacity = (int)(eval(ss)*255)
		return result | (opacity << 24)

	def parse_converted_paths(self, converted_paths, attributes):
		"""
		Parses the converted paths and creates a string representation 
		of the path segments.

		Args:
			converted_paths (List[Path]): A list of converted paths.

		Returns:
			str: A string representation of the path segments.
		"""
		result = {"type":"icon", "resolution":{"width":int(self.size//64),"height":int(self.size//64)}, "paths":[]}

		for i, path in enumerate(converted_paths):
			all_paths = {}
			all_paths["color"] = self.get_color(attributes[i].get('style', ''))
			subpaths = path.continuous_subpaths()  # Get continuous subpaths
			subpath_icn = ""
			for subpath_index, subpath in enumerate(subpaths):
				for segment in subpath:
					# Identify the type of segment
					segment_type = segment.__class__.__name__
					if segment_type == 'Line':
						# For Line segments
						subpath_icn += ".%s;" % self.to_64(segment.start)
						subpath_icn += ".%s;" % self.to_64(segment.end)
					elif segment_type == 'CubicBezier':
						# For Cubic Bezier segments
						subpath_icn += ".%s;" % self.to_64(segment.start)
						subpath_icn += "$%s;" % self.to_64(segment.control1)
						subpath_icn += "$%s;" % self.to_64(segment.control2)
						subpath_icn += ".%s;" % self.to_64(segment.end)
					elif segment_type == 'QuadraticBezier':
						# For Quadratic Bezier segments
						subpath_icn += ".%s;" % self.to_64(segment.start)
						subpath_icn += "^%s;" % self.to_64(segment.control)
						subpath_icn += ".%s;" % self.to_64(segment.end)
				subpath_icn += "~;"  # End of subpath indicator
			all_paths["path"] = subpath_icn

			result["paths"].append(all_paths)

		return result  # Return the string representation

	def move_paths_to_origin(self, paths, x, y):
		return [
			path.translated(complex(-x, -y)) 
			for path in paths
		]

	def process_svg(self, svg_file):
		"""
		Processes an SVG file, converting arcs to Bezier curves and returning new paths.

		Args:
			svg_file (str): The path to the SVG file to be processed.

		Returns:
			List[Path]: A list of new paths with arcs converted to Bezier curves.
		"""
		# Parse the SVG file
		paths, attributes = svg2paths(svg_file)
		new_paths = []
		
		for i, path in enumerate(paths):
			new_path = Path()  # Create a new Path object
			commands = path.d()  # Get the path commands

			for segment in path:
				if segment.__class__.__name__ == 'Arc':
					# Extract parameters from Arc segment
					arc = segment
					x1, y1 = arc.start.real, arc.start.imag
					x2, y2 = arc.end.real, arc.end.imag
					rx, ry = arc.radius.real, arc.radius.imag
					phi = arc.rotation
					large_arc_flag = arc.large_arc
					sweep_flag = arc.sweep

					# Convert Arc to Quadratic Bezier curves
					beziers = self.convert_arc_to_bezier(x1, y1, x2, y2, rx, ry, phi, large_arc_flag, sweep_flag)

					# Append the converted Bezier segments to the new path
					for bezier in beziers:
						new_path.append(bezier)
				else:
					# For non-Arc segments, just add them to the new path
					new_path.append(segment)
			new_paths.append(new_path)  # Add the new path to the list of new paths
		return new_paths, attributes

	def parse(self):
		converted_paths, attributes = self.process_svg(svg_file)  # Process the SVG file
		view_box = self.get_viewbox(svg_file)
		self.get_size(svg_file)
		if view_box is not None:
			self.x,self.y,self.width,self.height = view_box
			converted_paths = self.move_paths_to_origin(converted_paths, self.x, self.y)

		return self.parse_converted_paths(converted_paths, attributes) # Parse the converted paths


# Example usage
if __name__ == "__main__":
	
	# Process SVG files matching the pattern
	for svg_file in glob.glob("../icons/*.svg"):
		name = os.path.splitext(svg_file)[0]
		name = name.split("_24dp")[0] + ".icn"
		print("%s" % name)
		icn = SvgToIcn(svg_file)
		icon = icn.parse()
		with open(name,"w") as file:
			file.write("%s"%icon)

