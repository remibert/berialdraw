#include "berialdraw_imp.hpp"
using namespace berialdraw;

#include <assert.h>
static void check(const String & str,const char * ref)
{
	assert(strcmp(str.c_str(), ref) == 0);
	assert(strlen(str.c_str()) == str.size());
}

void String::test1()
{
	String str("toto");

	check(str, "toto");
	str.append("hello");
	check(str,"totohello");
	
	for(int i = 0; i < 64; i++)
	{
		str.append("_");
	}

	uint32_t sz = str.size();
	assert(str.size() == 73);

	String str2(str);
	assert(str2.size() == 73);
	
	str = "jjj";
	check(str,"jjj");
	str2 = str;
	check(str2, "jjj");

	str.clear();
	str.print("-%02d-",12434);
	check(str, "-12434-");

	str = "Rémi à la plage";

	assert(str == "Rémi à la plage");
	assert(str != "Rémi à la piscine");

	str2 = str;
	assert(str == str2);
	str2 = "Rémi à la piscine";
	assert(str != str2);

	assert(str.count() == 15);
	assert(str.size() == 17);
	wchar_t character = str.get(1);
	assert(character == 233);

	character = str.get(0);
	assert(character == 'R');
	character = str.get(14);
	assert(character == 'e');
	character = str.get(15);
	assert(character == '\0');
	character = str.get(5);
	assert(character == 224);

	str.write_format("bonjour");
	assert(str == "Rémi à la plagebonjour");
	str.clear();
	str2 = "toto";
	assert(!(str2 == str));
	assert((str2 != str));

	{
		String a;
		String b;
		assert((a == str));
		assert(!(a != str));

		assert(a == b);
		assert(!(a != b));
	}

	{
		str.clear();
		str += "1234";
		str2 = "5678";
		check(str, "1234");
		str = str + str2;
		check(str, "12345678");
		check(str2, "5678");
		str += str2;
		check(str, "123456785678");
		check(str2, "5678");

		str += "901";
		check(str, "123456785678901");

		str = "34";
		str = str +"56";
		check(str, "3456");
	}
}

void String::test2()
{
	{
		String str("123");
		int32_t val = 0;
		assert(str.to_int(val));
		assert(val == 123);
	}

	{
		String str("0x123");
		int32_t val = 0;
		assert(str.to_int(val));
		assert(val == 0x123);
	}
	{
		String str("0b11111111");
		int32_t val = 0;
		assert(str.to_int(val));
		assert(val == 0xFF);
	}
	{
		String str("toto");
		int32_t val = 0;
		assert(str.to_int(val) == false);
	}
	{
		String str("têàtotàoà");
		wchar_t character;

		uint32_t offset = 0;

		for(int i = 0; i<= (int)str.count(); i++)
		{
			character = str.read_char();
			assert(character == str.get(i));
		}
		
		character = str.read_char();
		character = str.read_char();
		character = str.read_char();
		character = str.read_char();
		character = str.read_char();
		character = str.read_char();
		character = str.read_char();
	}
	{
		String str("rémi");
		str.replace('e',1);
		check(str,"remi");
		str.replace(0xE9,0);
		check(str,"éemi");
		str.replace(0xE9,-1);
		check(str,"éemé");
		str.replace('0', 0);
		check(str,"0emé");
		str.replace('1', 1);
		check(str,"01mé");
		str.replace('2', 2);
		check(str,"012é");
		str.replace('3', 3);
		check(str,"0123");
		str.replace('4', 4);
		check(str,"0123");
		str.replace('5', 5);
		check(str,"0123");

		str.replace('a', -1);
		check(str,"012a");
		str.replace('b', -2);
		check(str,"01ba");
		str.replace('c', -3);
		check(str,"0cba");
		str.replace('d', -4);
		check(str,"dcba");
		str.replace('e', -5);
		check(str,"dcba");
		str.replace('f', -6);
		check(str,"dcba");
	}

	{
		String str;
		str.insert("rémi",0);
		check(str,"rémi");
		str.remove(-1);
		check(str,"rém");
		str.remove(-1);
		check(str,"ré");
		str.remove(-1);
		check(str,"r");
		str.remove(-1);
		check(str,"");
		str.remove(-1);
		check(str,"");
	}
	{
		String str;
		str.insert("rémi",0);
		check(str,"rémi");
		str.remove(0);
		check(str,"émi");
		str.remove(0);
		check(str,"mi");
		str.remove(0);
		check(str,"i");
		str.remove(0);
		check(str,"");
		str.remove(0);
		check(str,"");
	}
	{
		String str("êàtotàoà");
		str.remove((int32_t)str.count());
		check(str,"êàtotàoà");
		str.remove(-1);
		check(str,"êàtotào");
		str.remove(0);
		check(str,"àtotào");
		str.remove(2);
		check(str,"àttào");
		str.remove(2);
		check(str,"àtào");
		str.remove(2);
		check(str,"àto");
		str.remove(2);
		check(str,"àt");
		str.remove(2);
		check(str,"àt");
	}
	{
		String str("êàtotàoà");
		uint32_t len = str.count();
		for(int32_t i = (int32_t)len; i >= 0; i--)
		{
			str.insert(0xE9,i);
		}
		check(str,"éêéàétéoétéàéoéàé");
	}
	{
		String str("êàtoto");
		str.insert(0xE9,1);
		check(str,"êéàtoto");
	}
	{
		String str2;
		str2.insert(0xE9,1);
		check(str2,"");
		str2.insert(0xE9,0);
		check(str2,"é");
	}
}

void String::test3()
{
	String s("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	
	String a;
	s.slice(0,0xFFFFFF,a);
	assert(a == "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	s.slice(0,-1,a);
	assert(a == "0123456789ABCDEFGHIJKLMNOPQRSTUVWXY");
	s.slice(5,12,a);
	assert(a == "56789AB");
	s.slice(5,-12,a);
	assert(a == "56789ABCDEFGHIJKLMN");
	s.slice(-5,-12,a);
	assert(a == "");
	s.slice(-12,-5,a);
	assert(a == "OPQRSTU");
	s.slice(100000,1000000,a);
	assert(a == "");
	s.slice(-100000,-100,a);
	assert(a == "");
	s.slice(-5,0,a);
	assert(a == "VWXYZ");
	s.slice(0,5,a);
	assert(a == "01234");
}

void String::test4()
{
	String text("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	text.remove(1,3);
	assert(text == "ADEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	text.remove(-5,-3);
	assert(text == "ADEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuxyz");
	text.remove(30,0);
	assert(text == "ADEFGHIJKLMNOPQRSTUVWXYZabcdef");
	text.remove(0,10);
	assert(text == "MNOPQRSTUVWXYZabcdef");
	text.remove(2,0);
	assert(text == "MN");
	text.remove(0,2);
	assert(text == "");
	text.remove(0,2);
	assert(text == "");
}

void String::test5()
{
	String str("Salut rémi, rémi Comment tu vas. rémi hello");

	int32_t pos = str.find("rémi");
	assert(pos == 6);
	pos = str.find("rémi",++pos);
	assert(pos == 12);
	pos = str.find("rémi",++pos);
	assert(pos == 33);
	pos = str.find("rémi",++pos);
	assert(pos == INT32_MAX);

	pos = -43;
	pos = str.find("rémi",pos);
	assert(pos == -37);
	pos = str.find("rémi",++pos);
	assert(pos == -31);
	pos = str.find("rémi",++pos);
	assert(pos == -10);
	pos = str.find("rémi",++pos);
	assert(pos == INT32_MAX);

	pos = str.find("prout");
	assert(pos == INT32_MAX);

	pos = str.find("");
	assert(pos == INT32_MAX);

	pos = str.find(0);
	assert(pos == INT32_MAX);
}

void String::test6()
{
	String str("Salut rémi, rémi Comment tu vas. rémi hello");

	int32_t pos = str.rfind("rémi");
	assert(pos == -10);
	pos = str.rfind("rémi",--pos);
	assert(pos == -31);
	pos = str.rfind("rémi",--pos);
	assert(pos == -37);

	pos = str.rfind("rémi",--pos);
	assert(pos == INT32_MAX);

	pos = 44;
	pos = str.rfind("rémi",--pos);
	assert(pos == 33);
	pos = str.rfind("rémi",--pos);
	assert(pos == 12);
	pos = str.rfind("rémi",--pos);
	assert(pos == 6);
	pos = str.rfind("rémi",--pos);
	assert(pos == INT32_MAX);

	pos = str.rfind("prout");
	assert(pos == INT32_MAX);

	pos = str.rfind("");
	assert(pos == INT32_MAX);

	pos = str.rfind(0);
	assert(pos == INT32_MAX);
}


void String::test7()
{
	// Test basic replace
	String str("hello world");
	str.replace("world", "universe");
	check(str, "hello universe");

	// Test replace multiple occurrences
	str = "cat bat cat dog cat";
	str.replace("cat", "mouse");
	check(str, "mouse bat mouse dog mouse");

	// Test replace with longer string
	str = "a b c";
	str.replace("b", "hello");
	check(str, "a hello c");

	// Test replace with shorter string
	str = "hello world hello";
	str.replace("hello", "hi");
	check(str, "hi world hi");

	// Test replace with empty string (remove)
	str = "a,b,c,d";
	str.replace(",", "");
	check(str, "abcd");

	// Test replace with UTF-8 characters
	str = "café café café";
	str.replace("café", "thé");
	check(str, "thé thé thé");

	// Test replace when substring not found
	str = "hello world";
	str.replace("xyz", "abc");
	check(str, "hello world");

	// Test replace empty search (should do nothing)
	str = "hello";
	str.replace("", "x");
	check(str, "hello");

	// Test replace with null replaced (should treat as empty)
	str = "a-b-c";
	str.replace("-", "");
	check(str, "abc");

	// Test single character replace
	str = "aaa";
	str.replace("a", "b");
	check(str, "bbb");

	// Test overlapping patterns don't cause infinite loops
	str = "aaaa";
	str.replace("aa", "bb");
	check(str, "bbbb");
}

void String::test8()
{
	// Test basic variable search
	String str("Hello $(name), welcome!");
	String var;
	int32_t pos = str.search_var(var);
	assert(pos == 6);
	assert(var == "name");

	// Test search with starting position
	str = "Hello $(first), this is $(second) test";
	pos = str.search_var(var, 0);
	assert(pos == 6);
	assert(var == "first");
	
	pos = str.search_var(var, pos + 1);
	assert(pos == 24);
	assert(var == "second");

	// Test no variable found
	str = "Hello world";
	pos = str.search_var(var);
	assert(pos == INT32_MAX);

	// Test unclosed variable
	str = "Hello $(name world";
	pos = str.search_var(var);
	assert(pos == INT32_MAX);

	// Test variable without opening
	str = "Hello name}";
	pos = str.search_var(var);
	assert(pos == INT32_MAX);

	// Test variable with UTF-8 characters
	str = "Bonjour $(prénom), bienvenue!";
	pos = str.search_var(var);
	assert(pos == 8);
	assert(var == "prénom");

	// Test multiple variables with UTF-8
	str = "Hello $(éléphant) and $(ours)";
	pos = str.search_var(var, 0);
	assert(pos == 6);
	assert(var == "éléphant");
	
	pos = str.search_var(var, pos + 1);
	assert(pos == 22);
	assert(var == "ours");

	// Test variable at the beginning
	str = "$(var) is at start";
	pos = str.search_var(var);
	assert(pos == 0);
	assert(var == "var");

	// Test variable at the end
	str = "This is $(var)";
	pos = str.search_var(var);
	assert(pos == 8);
	assert(var == "var");

	// Test consecutive variables
	str = "$(first)$(second)";
	pos = str.search_var(var, 0);
	assert(pos == 0);
	assert(var == "first");
	
	pos = str.search_var(var, pos + 1);
	assert(pos == 8);
	assert(var == "second");

	// Test variable with numbers and underscore
	str = "Config: $(var_name_123)";
	pos = str.search_var(var);
	assert(pos == 8);
	assert(var == "var_name_123");

	// Test empty variable name
	str = "Empty: $()";
	pos = str.search_var(var);
	assert(pos == 7);
	assert(var == "");
}

void String::test9()
{
	// Test to_upper with ASCII
	String str("hello world");
	str.to_upper();
	assert(str == "HELLO WORLD");

	// Test to_lower with ASCII
	str = "HELLO WORLD";
	str.to_lower();
	assert(str == "hello world");

	// Test to_upper with mixed case
	str = "HeLLo WoRLD";
	str.to_upper();
	assert(str == "HELLO WORLD");

	// Test to_lower with mixed case
	str = "HeLLo WoRLD";
	str.to_lower();
	assert(str == "hello world");

	// Test to_upper with UTF-8 accented characters
	str = "café français";
	str.to_upper();
	assert(str == "CAFé FRANçAIS");

	// Test to_lower with UTF-8 accented characters
	str = "CAFÉ FRANÇAIS";
	str.to_lower();
	assert(str == "cafÉ franÇais");

	// Test to_upper preserves numbers and special characters
	str = "test123!@# ABC";
	str.to_upper();
	assert(str == "TEST123!@# ABC");

	// Test to_lower preserves numbers and special characters
	str = "TEST123!@# abc";
	str.to_lower();
	assert(str == "test123!@# abc");

	// Test to_upper with empty string
	str = "";
	str.to_upper();
	assert(str == "");

	// Test to_lower with empty string
	str = "";
	str.to_lower();
	assert(str == "");

	// Test to_upper with single character
	str = "a";
	str.to_upper();
	assert(str == "A");

	// Test to_lower with single character
	str = "Z";
	str.to_lower();
	assert(str == "z");

	// Test to_upper with accented single character
	str = "é";
	str.to_upper();
	assert(str == "é");

	// Test to_lower with accented single character
	str = "É";
	str.to_lower();
	assert(str == "É");

	// Test chain operations
	str = "HeLLo WoRLD";
	str.to_upper().to_lower().to_upper();
	assert(str == "HELLO WORLD");

	// Test with mixed ASCII and UTF-8
	str = "Hello Café World";
	str.to_upper();
	assert(str == "HELLO CAFé WORLD");

	str = "HELLO CAFÉ WORLD";
	str.to_lower();
	assert(str == "hello cafÉ world");
}

void String::test10()
{
	// Test move constructor - transfer ownership
	String str1("hello world");
	String str2(std::move(str1));
	assert(str2 == "hello world");
	assert(str1.size() == 0);  // Source should be empty after move

	// Test move constructor with large string
	String str3("This is a much longer string with more data to transfer");
	uint32_t size = str3.size();
	String str4(std::move(str3));
	assert(str4.size() == size);
	assert(str3.size() == 0);
	assert(str4 == "This is a much longer string with more data to transfer");

	// Test move assignment operator
	String str5("source");
	String str6("destination");
	str6 = std::move(str5);
	assert(str6 == "source");
	assert(str5.size() == 0);  // Source should be empty after move

	// Test move assignment with self-assignment check
	String str7("test");
	str7 = std::move(str7);  // Should handle gracefully with self-move
	assert(str7.size() == 0 || str7 == "test");  // Self-move may leave it empty

	// Test move with UTF-8 characters
	String str8("café français");
	uint32_t count = str8.count();
	String str9(std::move(str8));
	assert(str9.count() == count);
	assert(str9 == "café français");
	assert(str8.size() == 0);

	// Test chained moves
	String str10("chained");
	String str11(std::move(str10));
	String str12(std::move(str11));
	assert(str12 == "chained");
	assert(str10.size() == 0);
	assert(str11.size() == 0);

	// Test move in operations
	String str13("hello");
	String str14 = std::move(str13) + " world";
	assert(str14 == "hello world");
}

void String::test()
{
	MemoryLeakLog
	test10();
	test9();
	test8();
	test7();
	test6();
	test5();
	test4();
	test3();
	test2();
	test1();
}
