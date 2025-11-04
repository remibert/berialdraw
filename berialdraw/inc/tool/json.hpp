#pragma once
namespace berialdraw
{
	class Item;
	class ItemVariant;

	/** Parsing file error exception */
	class JsonUnserializeError : public std::exception
	{
	public:
#ifndef SWIG
/// @cond DOXYGEN_IGNORE
		int32_t m_line = 0;
		int32_t m_column = 0;
/// @endcond
#endif
	};

	/** Itermediate iterator destroyed */
	class JsonIteratorError : public std::exception
	{
	};



	/** Json data */
	class Json
	{
	friend JsonIterator;
	public:
		/** Constructor */
		Json(const char * buffer=0);

		/** Destructor */
		virtual ~Json();

		/** Provides read access to element at the given index in the vector. */
		JsonIterator operator[](int index);

		/** Provides read access to element at the given index in the vector. */
		JsonIterator operator[](const char * key);

		/** Get the count of items in the current node */
		int32_t count();

		/** Serialize to json */
		void serialize(TextStream & out, int32_t indent=-1);

		/** Unserialize from json (can throw JsonUnserializeError) */
		void unserialize(const char *content);

		/** Unserialize from json (can throw JsonUnserializeError) */
		void unserialize(TextStream & content);

		/** Clear the content of json */
		void clear();

		/** Skip blank character */
		static wchar_t skip_blank(TextStream & content);

		/** Choose item type */
		static Item * choose_type(TextStream & content);

		/** Last element of a json array */
		static const int32_t LAST;
		/** First element of a json array */
		static const int32_t FIRST;
#ifndef SWIG
	#ifdef _DEBUG
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
		static void test6();
		static void test7();
		static void test8();
		static void test9();
		static void test10();
		static void test11();
		static void test12();
		static void test13();
		static void test14();
		static void test15();
		static void test16();
		static void test17();
		static void test18();
		static void test19();
		static void test20();
		static void test21();
		static void test22();
		static void test23();
	#endif

	protected:
/// @cond DOXYGEN_IGNORE
		Json & operator= (const Json & other){return *this;}
		Json(Json & other):m_iterator(*this){}
	
		/** Update the line and column for where the error is located */
		void throw_error(TextStream & content);

		ItemCollection * m_json = 0;
		JsonIterator m_iterator;
/// @endcond
#endif
	};
}
