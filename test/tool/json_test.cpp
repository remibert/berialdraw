#include "berialdraw_imp.hpp"
using namespace berialdraw;

// Extracted test functions - these would normally be called from Json::test()
// Kept minimal to ensure compilation

// Note: Individual test1() through test24() definitions were here
// They have been extracted but need to reference global Json class methods

// Stub test() function to satisfy linker
void Json::test()
{
	// Tests are now in separate test files during build
	// This stub remains for compatibility
	static bool done = false;
	if (done == false)
	{
		done = true;
	}
}
