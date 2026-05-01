#include "pybind/pyberialdraw.hpp"

namespace py = pybind11;

// Callback using py::print
static void pybind_printf_callback(const char* format, va_list args)
{
	// Create a copy to calculate the size
	va_list args_copy;
	va_copy(args_copy, args);
	
	// Calculate the required size
	int size = vsnprintf(nullptr, 0, format, args_copy);
	va_end(args_copy);
	
	if (size > 0)
	{
		std::string buffer(size + 1, '\0');
		vsnprintf(&buffer[0], size + 1, format, args);
		
		// Use py::print without extra newline (already in format)
		py::print(buffer, py::arg("end") = "");
		fflush(stdout);
	}
}

// Initialize the callback on module load
void redirect_print()
{
	bd_redirect_printf(pybind_printf_callback);
}
