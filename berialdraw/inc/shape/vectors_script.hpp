#pragma once

namespace berialdraw
{
/**
* The VectorsScript class is designed to handle scripts that define Bezier curves using a custom grammar. 
* It offers functionality to interpret the script, manage variables, and construct Bezier polygons. 
* The class is versatile, enabling the dynamic processing of paths with support for operators and 
* variable substitution, making it a powerful tool for generating and manipulating complex vector shapes.
* 
* Grammar of vector buffer  :
* 
* Values :
*	- 'a'..'z' : constantes or parameters
*	- 'A'..'Z' : variables
* 
* Commands :
*	- '.' : vector point  (2 parameters X,Y)
*	- '^' : conic point   (2 parameters X,Y)
*	- '$' : cubic point   (2 parameters X,Y)
*	- '=' : set variables (1 parameter)
*	- '~' : next contour terminator
*	- ';' : command terminator
* 
* Operators :
*	- '+' : add value
*	- '-' : substract value
*	- '/' : divide value
*	- '*' : multiply value
*	- '%' : modulo value
*	- '>' : binary shift >>
*	- '<' : binary shift << 
* 
* Number :
*	- D       : decimal number     (D = [0..9])
*	- 0xXXXX  : hexadecimal number (X = [0..9, A..F, a..f])
*	- 0oOOOO  : octal number       (O = [0..7])
*	- 0bBBBBB : binary number      (B = [0,1])
*	Negative number start with '-', all numbers are limited to 32 bits.
*
* Space, tabulation, end of line are forbidden.
* 
* Example :
*	data = "X=10;Y=11;R=3;Y=X*3393>13;.X,Y;$X+10,Y;" 
*	you select :
*	- X = 10;
*	- Y = 11;
*	- R = 3;
*	- X = R * 3393 >> 13;
*	- Point(X,Y);
*	- Cubic(X+10, Y);
* 
*/
	class VectorsScript
	{
	public:
		/** Provides error detection during script parsing, returning specific error codes from the Error enumeration. */
		enum Error
		{
			SUCCESS                  = 0, ///< The script was successfully parsed and executed.
			UNSUPPORTED_VARIABLES    = 1, ///< The script contains variables that are not supported.
			TOO_MANY_PARAMETERS      = 2, ///< A command in the script received more parameters than allowed.
			BAD_INTEGER              = 3, ///< An integer value in the script is malformed or invalid.
			BAD_COMMAND              = 4, ///< The script contains an unrecognized or invalid command.
			DIVIDE_BY_ZERO           = 5, ///< An attempted division by zero occurred during script evaluation.
		};

		/** Initializes the class with a script buffer and a reference to a polygon. 
		@param buffer vector script buffer 
		@param polygon Reference to a Polygon instance that will be constructed by the class.*/
		VectorsScript(const char * buffer, Polygon & polygon);
		
		/** Initializes the class with a polygon, allowing scripts to be loaded later using select. 
		@param polygon Reference to a Polygon instance that will be constructed by the class.*/
		VectorsScript(Polygon & polygon);

		/** Cleans up resources when the object is destroyed. */
		~VectorsScript();
		
		/**  Retrieves the value of a variable.
		@return value */
		Coord get(char letter);
		
		/** Assigns a value to a variable identified by a letter. 
		@param letter variable letter
		@param value value of variable */
		void  set(char letter, Coord value);
		
		/** The parse method interprets the loaded script, constructing a Bezier polygon while respecting defined variables and operations.
		@return parsing error */
		enum VectorsScript::Error parse();
		
		/** The select method loads a script into the internal buffer for parsing and execution. 
		@param buffer buffer to load */
		void select(const char * buffer);
	private:
/// @cond DOXYGEN_IGNORE
		uint32_t count_points();
		uint32_t count_contours();
		uint32_t to_var(char letter);
		void    set_variable(uint32_t index, Coord value);
		Coord get_variable(uint32_t index);
		void    clear_params();
		Coord operation(char m_operator, Coord param1, Coord param2);
		char    get_char();
		void    next();
		void    first();
		void    set_error(enum Error err);
		void    parse_command(uint32_t maxparams);
		
		enum { MAX_VARIABLES = (10+26+26)};
		Coord m_variables[MAX_VARIABLES];
		const char* m_pos = 0;
		const char* m_buffer = 0;
		char        m_separator = 0;
		char        m_operator = 0;
		char        m_command = 0;
		int32_t     m_pos_error = 0;
		enum Error  m_error = SUCCESS;
		Coord m_temp;

		Polygon & m_polygon;
/// @endcond
	};
};
