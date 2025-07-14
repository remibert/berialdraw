from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class BerialDrawConan(ConanFile):
	name = "berialdraw"
	version = "1.0"
	settings = "os", "compiler", "build_type", "arch"
	generators = "CMakeDeps", "CMakeToolchain"
	exports_sources = "*"
	requires = "sdl/3.2.14"

	def layout(self):
		cmake_layout(self)
		self.folders.source = "."
		self.folders.build = "/Users/remi/Downloads/berialdraw"

	def build(self):
		cmake = CMake(self)
		cmake.configure()
		cmake.build()

	def package(self):
		self.copy("*.h",     dst="include", src="inc")
		self.copy("*.lib",   dst="lib",     keep_path=False)
		self.copy("*.a",     dst="lib",     keep_path=False)
		self.copy("*.dylib", dst="lib",     keep_path=False)
		self.copy("*.so",    dst="lib",     keep_path=False)
		self.copy("*",       dst="bin",     src="bin")

	def package_info(self):
		self.cpp_info.libs = ["berialdraw"]
