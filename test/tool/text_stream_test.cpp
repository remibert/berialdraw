#include "berialdraw_imp.hpp"
using namespace berialdraw;


// Unitary test
void TextStream::test()
{
	MemoryLeakLog
	test5();
	test4();
	test3();
	test2();
	test1();
}

void TextStream::test1()
{
	String buffer("0x12345,");
	int64_t value;
	bool accurate;

	buffer.read_integer(value,accurate);
	assert(value == 0x12345 && accurate == false);

	buffer = "-123h ljkh lijh ";
	buffer.read_integer(value,accurate);
	assert(value == -123 && accurate == false);

	buffer = "0o777 klmj lmkjkj ";
	buffer.read_integer(value,accurate);
	assert(value == 0x1FF && accurate == false);

	buffer = "0xABCDEF0123456789 klmj lmkjkj ";
	buffer.read_integer(value,accurate);
	assert(value == 0xABCDEF0123456789 && accurate == false);

	buffer = "0b111111111111 klmj lmkjkj ";
	buffer.read_integer(value,accurate);
	assert(value == 0xfff && accurate == false);

	buffer = "0uabcdef123411111 klmj lmkjkj ";
	buffer.read_integer(value,accurate);
	assert(value == 0 && accurate == false);

	buffer = "0zabcdef1234111111111111 klmj lmkjkj ";
	buffer.read_integer(value, accurate);
	assert(value == 0 && accurate == false);

	buffer = "0ABCDEF0123456789 klmj lmkjkj ";
	buffer.read_integer(value, accurate);
	assert(value == 0 && accurate == false);

	buffer = "1ABCDEF0123456789 klmj lmkjkj ";
	buffer.read_integer(value, accurate);
	assert(value == 1 && accurate == false);

	buffer = "a1ABCDEF0123456789 klmj lmkjkj ";
	buffer.read_integer(value, accurate);
	assert(value == 0 && accurate == false);

	buffer = "12.34a";
	buffer.read_integer(value, accurate);
	assert(value == 789 && accurate == true);

	buffer = "-12.34a";
	buffer.read_integer(value, accurate);
	assert(value == -789 && accurate == true);

	buffer = "0.34a";
	buffer.read_integer(value, accurate);
	assert(value == 21 && accurate == true);

	buffer = "-0.34a";
	buffer.read_integer(value, accurate);
	assert(value == -21 && accurate == true);
}

void TextStream::test2()
{
	String buffer("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_ lkj lmkj ");
	String name;

	buffer.read_name(name);
	assert(name == "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_");

	buffer = "0x1234";
	buffer.read_name(name);
	assert(name == "");
}

void TextStream::test3()
{
	String buffer;
	buffer.pad('0',10);
	buffer.pad('1',5);
	assert(buffer == "000000000011111");
}

void TextStream::test4()
{
	String buffer;
	buffer.write_format("0x%04X",0xABCD);
	assert(buffer == "0xABCD");
	buffer.write_format("0x%04X",0x1234);
	assert(buffer == "0xABCD0x1234");
}

void TextStream::test5()
{
	String buffer("\t\n 0x12345,\n");
	int64_t integer;
	bool accurate;
	uint32_t res = buffer.read_integer(integer,accurate);
	assert(res == 10);
	assert(integer == 0x12345);
	assert(accurate == false);

	buffer = "    123.345  ";
	res = buffer.read_integer(integer,accurate);
	assert(res == 11);
	assert(integer == 7894);
	assert(accurate == true);


	buffer = "   -123.345  ";
	res = buffer.read_integer(integer,accurate);
	assert(res == 11);
	assert(integer == -7894);
	assert(accurate == true);


	buffer = "-123.345";
	res = buffer.read_integer(integer,accurate);
	assert(res == 8);
	assert(integer == -7894);
	assert(accurate == true);

	buffer = "123.345";
	res = buffer.read_integer(integer,accurate);
	assert(res == 7);
	assert(integer == 7894);
	assert(accurate == true);


	buffer = "123.";
	res = buffer.read_integer(integer,accurate);
	assert(res == 4);
	assert(integer == 123*64);
	assert(accurate == true);

	buffer = "-123.";
	res = buffer.read_integer(integer,accurate);
	assert(res == 5);
	assert(integer == -123*64);
	assert(accurate == true);

	buffer = "123";
	res = buffer.read_integer(integer,accurate);
	assert(res == 3);
	assert(integer == 123);
	assert(accurate == false);

	buffer = "-123";
	res = buffer.read_integer(integer,accurate);
	assert(res == 4);
	assert(integer == -123);
	assert(accurate == false);

}
