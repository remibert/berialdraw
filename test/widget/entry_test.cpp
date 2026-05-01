#include "berialdraw_imp.hpp"
using namespace berialdraw;

#include <assert.h>
#define check(str,ref) assert(strcmp(str.input().c_str(), ref) == 0);

void Entry::test1()
{
	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_RIGHT);
		entry.left_arrow_key();
		entry.down_arrow_key();
		entry.append_key('0');
		check(entry,"Hello  World\n0HW\n");
	}

	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_LEFT);
		entry.left_arrow_key();
		entry.move_cursor(1,2,true);
		entry.move_cursor(1,1,true);
		entry.erase_text_selected();
		check(entry,"W\n");
	}

	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_LEFT);
		entry.right_arrow_key();
		entry.move_cursor(0,2,true);
		entry.erase_text_selected();
		check(entry,"He");
	}
	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_LEFT);
		entry.left_arrow_key();
		entry.move_cursor(1,5,true);
		entry.erase_text_selected();
		check(entry,"\n");
	}
	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_LEFT);
		entry.left_arrow_key();
		entry.down_arrow_key(true);
		entry.erase_text_selected();
		check(entry,"HW\n");
	}
	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_LEFT);
		entry.delete_key();
		check(entry,"");
	}
	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_LEFT);
		entry.backspace_key();
		check(entry,"");
	}
	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_LEFT);
		entry.left_arrow_key();
		entry.right_arrow_key(true);
		entry.right_arrow_key(true);
		entry.right_arrow_key(true);
		entry.right_arrow_key(true);
		entry.erase_text_selected();
		check(entry,"o  World\nHW\n");
		entry.up_arrow_key();
	}
	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_LEFT);
		entry.left_arrow_key();

		entry.down_arrow_key();
		entry.append_key('1');
		check(entry,"Hello  World\n1HW\n");
		entry.up_arrow_key();

		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();

		entry.down_arrow_key();
		entry.append_key('0');
		check(entry,"Hello  World\n1HW0\n");
	}
	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::ALIGN_RIGHT);
		entry.left_arrow_key();

		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();

		entry.down_arrow_key();
		entry.append_key('0');
		check(entry,"Hello  World\nH0W\n");
	}

	{
		String s("Hello  World\nHW\n");
		Entry entry(&s);
		entry.entry_align(Align::CENTER);
		entry.left_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.down_arrow_key();
		entry.append_key('0');
		check(entry,"Hello  World\nH0W\n");
		
		entry.right_arrow_key();
		entry.up_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.down_arrow_key();
		entry.append_key('1');
		check(entry,"Hello  World\nH0W1\n");


		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.up_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.down_arrow_key();
		entry.append_key('2');
		check(entry,"Hello  World\n2H0W1\n");

		entry.home_key();
		entry.down_arrow_key();
		entry.append_key('3');
		check(entry,"Hello  World\n32H0W1\n");
		
	}
	{
		String s("Hello World\nsalut\n");
		Entry entry(&s);
		entry.select_none();
		entry.append_key('1');
		entry.append_key('2');
		entry.append_key('3');
		check(entry,"123Hello World\nsalut\n");
		entry.select_all();
		entry.append_key('1');
		check(entry,"1");
	}
	{
		String s("Hello World\nsalut\n");
		Entry entry(&s);
		uint32_t count = entry.line_length(0);
		assert(count == 11);
		count = entry.line_length(1);
		assert(count == 5);
		count = entry.line_length(2);
		assert(count == 0);
		count = entry.line_length(3);
		assert(count == 0);
	}

	{
		String s("Hello World");
		Entry entry(&s);
		entry.home_key(); assert(entry.cursor_position() == 0);
		entry.right_arrow_key(); assert(entry.cursor_position() == 1);
		entry.right_arrow_key(); assert(entry.cursor_position() == 2);
		entry.right_arrow_key(); assert(entry.cursor_position() == 3);
		entry.right_arrow_key(); assert(entry.cursor_position() == 4);

		entry.right_arrow_key(true); assert(entry.cursor_position() ==  5); assert(entry.selection_start() == 4); assert(entry.selection_end() == 5);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  6); assert(entry.selection_start() == 4); assert(entry.selection_end() == 6);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  7); assert(entry.selection_start() == 4); assert(entry.selection_end() == 7);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  8); assert(entry.selection_start() == 4); assert(entry.selection_end() == 8);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  9); assert(entry.selection_start() == 4); assert(entry.selection_end() == 9);
		entry.right_arrow_key(true); assert(entry.cursor_position() == 10); assert(entry.selection_start() == 4); assert(entry.selection_end() == 10);
		entry.right_arrow_key(true); assert(entry.cursor_position() == 11); assert(entry.selection_start() == 4); assert(entry.selection_end() == 11);
		entry.right_arrow_key(true); assert(entry.cursor_position() == 11); assert(entry.selection_start() == 4); assert(entry.selection_end() == 11);
		entry.right_arrow_key(true); assert(entry.cursor_position() == 11); assert(entry.selection_start() == 4); assert(entry.selection_end() == 11);

		entry.left_arrow_key(true); assert(entry.cursor_position() == 10); assert(entry.selection_start() == 4); assert(entry.selection_end() == 10);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  9); assert(entry.selection_start() == 4); assert(entry.selection_end() ==  9);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  8); assert(entry.selection_start() == 4); assert(entry.selection_end() ==  8);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  7); assert(entry.selection_start() == 4); assert(entry.selection_end() ==  7);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  6); assert(entry.selection_start() == 4); assert(entry.selection_end() ==  6);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  5); assert(entry.selection_start() == 4); assert(entry.selection_end() ==  5);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  4); assert(entry.selection_start() == 4); assert(entry.selection_end() ==  4);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  3); assert(entry.selection_start() == 3); assert(entry.selection_end() ==  4);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  2); assert(entry.selection_start() == 2); assert(entry.selection_end() ==  4);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  1); assert(entry.selection_start() == 1); assert(entry.selection_end() ==  4);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  0); assert(entry.selection_start() == 0); assert(entry.selection_end() ==  4);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  0); assert(entry.selection_start() == 0); assert(entry.selection_end() ==  4);

		entry.right_arrow_key(true); assert(entry.cursor_position() ==  1); assert(entry.selection_start() == 1); assert(entry.selection_end() == 4);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  2); assert(entry.selection_start() == 2); assert(entry.selection_end() == 4);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  3); assert(entry.selection_start() == 3); assert(entry.selection_end() == 4);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  4); assert(entry.selection_start() == 4); assert(entry.selection_end() == 4);

		entry.home_key(true); assert(entry.cursor_position() ==  0); assert(entry.selection_start() == 0); assert(entry.selection_end() == 4);
		entry.end_key(true);  assert(entry.cursor_position() == 11); assert(entry.selection_start() == 4); assert(entry.selection_end() == 11);

		entry.left_arrow_key(true); assert(entry.cursor_position() == 10); assert(entry.selection_start() == 4); assert(entry.selection_end() == 10);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  9); assert(entry.selection_start() == 4); assert(entry.selection_end() ==  9);
		entry.left_arrow_key(true); assert(entry.cursor_position() ==  8); assert(entry.selection_start() == 4); assert(entry.selection_end() ==  8);

		entry.home_key(true); assert(entry.cursor_position() ==  0); assert(entry.selection_start() == 0); assert(entry.selection_end() == 4);

		entry.right_arrow_key(true); assert(entry.cursor_position() ==  1); assert(entry.selection_start() == 1); assert(entry.selection_end() == 4);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  2); assert(entry.selection_start() == 2); assert(entry.selection_end() == 4);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  3); assert(entry.selection_start() == 3); assert(entry.selection_end() == 4);
		entry.right_arrow_key(true); assert(entry.cursor_position() ==  4); assert(entry.selection_start() == 4); assert(entry.selection_end() == 4);

		entry.end_key(true);  assert(entry.cursor_position() == 11); assert(entry.selection_start() == 4); assert(entry.selection_end() == 11);

		entry.select_text(0, 11); assert(entry.cursor_position() == 11); assert(entry.selection_start() == 0); assert(entry.selection_end() == 11);

		entry.select_text(0, 20); assert(entry.cursor_position() == 11); assert(entry.selection_start() == 0); assert(entry.selection_end() == 11);

		entry.select_text(5, 5); assert(entry.cursor_position() == 5); assert(entry.selection_start() == 5); assert(entry.selection_end() == 5);
		entry.select_text(20, 20); assert(entry.cursor_position() == 11); assert(entry.selection_start() == 11); assert(entry.selection_end() == 11);

		entry.select_text(19, 20); assert(entry.cursor_position() == 11); assert(entry.selection_start() == 11); assert(entry.selection_end() == 11);
	}

	{
		String s("Default value");
		Entry entry(&s);
		//entry.input(s);
		assert(entry.cursor_position() == 0);
		entry.append_key('1');
		assert(entry.cursor_position() == 1);
		entry.append_key('2');
		assert(entry.cursor_position() == 2);
		entry.append_key('3');
		assert(entry.cursor_position() == 3);
		check(entry,"123");
		entry.clear_input();
		assert(entry.cursor_position() == 0);
	}

	{
		String s("Default value");
		Entry entry(&s);
		entry.append_key('1');
		entry.append_key('2');
		entry.append_key('3');
		check(entry,"123");
	}

	{
		String s("Default value");
		Entry entry(&s);
		entry.end_key();
		entry.append_key('1');
		entry.append_key('2');
		entry.append_key('3');
		check(entry,"Default value123");
	}
	{
		String s("Default value");
		Entry entry(&s);
		entry.home_key();
		entry.append_key('1'); 
		entry.append_key('2');
		entry.append_key('3');
		check(entry,"123Default value");
	}
	{
		String s("Default value");
		Entry entry(&s);
		entry.right_arrow_key();
		entry.append_key('1');
		entry.append_key('2');
		entry.append_key('3');
		check(entry,"Default value123");
	}
	{
		String s("Default value");
		Entry entry(&s);
		entry.left_arrow_key();
		entry.append_key('1');
		entry.append_key('2');
		entry.append_key('3');
		check(entry,"123Default value");
	}

	{
		String s("");
		Entry entry(&s);
		entry.typing_mode(TypingMode::REPLACEMENT);
		entry.append_key('1');
		entry.append_key('2');
		entry.append_key('3');
		check(entry,"123");
		entry.home_key();
		entry.append_key('4');
		check(entry,"423");
		entry.append_key('5');
		check(entry,"453");
		entry.append_key('6');
		check(entry,"456");
		entry.append_key('7');
		check(entry,"4567");
	}

	{
		String s("burp");
		Entry entry(&s);
		assert(entry.typing_mode() == TypingMode::INSERTION);

		entry.typing_mode(TypingMode::REPLACEMENT);
		assert(entry.typing_mode() == TypingMode::REPLACEMENT);
		entry.append_key('a');
		entry.append_key('b');
		entry.append_key('c');
		entry.append_key('d');
		entry.append_key('e');
		entry.append_key('f');
		check(entry,"abcdef");
		entry.home_key();
		entry.append_key('g');
		entry.append_key('h');
		entry.append_key('i');
		entry.append_key('j');
		entry.append_key('k');
		entry.append_key('l');
		entry.append_key('m');
		check(entry,"ghijklm");
	}

	{
		String s("burp");
		Entry entry(&s);
		assert(entry.typing_mode() == TypingMode::INSERTION);
		entry.typing_mode(TypingMode::REPLACEMENT);
		assert(entry.typing_mode() == TypingMode::REPLACEMENT);
	}
	{
		String s("rémi à la plage");
		Entry entry(&s);
		entry.select_text(0,0);
		check(entry,"rémi à la plage");
		entry.end_key();
		entry.backspace_key();
		entry.backspace_key();
		entry.backspace_key();
		entry.backspace_key();
		entry.backspace_key();
		entry.backspace_key();
		entry.backspace_key();
		entry.append_key('\'');
		entry.append_key('e');
		entry.append_key('a');
		entry.append_key('u');
		check(entry,"rémi à l'eau");
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.backspace_key();
		entry.append_key('d');
		entry.append_key('a');
		entry.append_key('n');
		entry.append_key('s');
		check(entry,"rémi dans l'eau");

		entry.home_key();
		entry.backspace_key();
		entry.backspace_key();
		entry.backspace_key();
		entry.backspace_key();
		check(entry,"rémi dans l'eau");

		entry.delete_key();
		entry.delete_key();
		entry.delete_key();
		entry.delete_key();
		check(entry," dans l'eau");

		entry.end_key();
		entry.delete_key();
		entry.delete_key();
		entry.delete_key();
		entry.delete_key();
		check(entry," dans l'eau");
		entry.left_arrow_key();
		entry.delete_key();
		check(entry," dans l'ea");
	}
	{
		String s;
		Entry entry(&s);
		entry.append_key('a');
		entry.append_key('b');
		entry.append_key('c');
		check(entry,"abc");
		entry.left_arrow_key();
		entry.append_key('d');
		entry.left_arrow_key();
		entry.append_key('e');
		entry.left_arrow_key();
		entry.append_key('f');
		entry.left_arrow_key();
		entry.append_key('g');
		check(entry,"abgfedc");
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.left_arrow_key();
		entry.append_key(0xE9);
		check(entry,"éabgfedc");

		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.right_arrow_key();
		entry.append_key(0xE0);
		check(entry,"éabgfedcà");
	}

	{

		String s("Hello");
		Entry hello(&s);
		hello.left_arrow_key();
		hello.left_arrow_key();
		hello.right_arrow_key();
		hello.right_arrow_key();
		hello.right_arrow_key();
		hello.right_arrow_key();
		hello.right_arrow_key();
	}
}

// Unitary test
void Entry::test()
{
	static bool done = false;
	if (done == false)
	{
		MemoryLeakLog
		done = true;
		test1();
	}
}
