#include "berialdraw_imp.hpp"
//#define JSON_TRACE
using namespace berialdraw;

const int32_t Json::LAST  = 0x7FFFFFFF;
const int32_t Json::FIRST = 0x80000000;

/** Constructor */
Json::Json(const char * buffer) : 
	m_iterator(*this)
{
	if(buffer)
	{
		String content(buffer);
		unserialize(content);
	}
}

/** Destructor */
Json::~Json()
{
	delete m_json;
}

/** Provides read access to element at the given index in the vector. */
JsonIterator Json::operator[](int index)
{
	return m_iterator[index];
}

/** Provides read access to element at the given index in the vector. */
JsonIterator Json::operator[](const char * key)
{
	return m_iterator[key];
}

/** Clear the content of json */
void Json::clear()
{
	delete m_json;
	m_json = 0;
}

/** Skip blank character */
wchar_t Json::skip_blank(TextStream & content)
{
	wchar_t result = '\0';
 	wchar_t character = ' ';
	do
	{
		// Skip white space
		while(character)
		{
			character = content.read_char();
			if(character > ' ')
			{
				uint32_t of = content.offset();
				content.offset(content.offset()-1);
				break;
			}
		}

		// If comment
		if(character == '#' || character == ';')
		{
			while(character)
			{
				character = content.read_char();
				if(character == '\n')
				{
					break;
				}
			}
		}
		// If comment
		else if(character == '/')
		{
			content.read_char();
			character = content.read_char();

			// Single line comment
			if(character == '/')
			{
				while(character)
				{
					character = content.read_char();
					if(character == '\n')
					{
					//	content.offset(content.offset()-1);
						break;
					}
				}
			}
			// Multiline comment
			else if(character == '*')
			{
				while(character)
				{
					character = content.read_char();
					if(character == '*')
					{
						character = content.read_char();
						if(character == '/')
						{
							character = ' ';
							//content.offset(content.offset()-1);
							break;
						}
					}
				}
			}
			else
			{
				// Bad comment detected
				character = Utf8::not_a_char;
				break;
			}
		}
		else
		{
			// Not a blank character can stop parsing
			break;
		}
	}
	while (character <= ' ' && character);

	result = character;

	return result;
}

/** Choose item type */
Item * Json::choose_type(TextStream & content)
{
	Item * result = 0;
	wchar_t character = skip_blank(content);

	switch(character)
	{
	case 'n':case 'N':
		result = new ItemNull;
		break;

	case 'f':case 'F': case 't':case 'T':
		result = new ItemBool;
		break;

	case '\"':case '\'':
		result = new ItemString;
		break;

	case '[':
		result = new ItemArray;
		break;

	case '{':
		result = new ItemObject;
		break;

	case '\0':
		break;

	default:
		if (character == '-' || (character >= '0' && character <= '9'))
		{
			result = new ItemInt;
		}
		break;
	}

	if (result)
	{
		if(result->unserialize(content) == false)
		{
			delete result;
			result = 0;
		}
	}
	return result;
}

/** Unserialize from json */
void Json::unserialize(const char *content)
{
	if (content)
	{
		String data(content);
		unserialize(data);
	}
}

/** Unserialize from json */
void Json::unserialize(TextStream & content)
{
	Item * item = 0;
	bool result = true;

	// If json already existing
	if(m_json)
	{
		// remove existing
		delete m_json;
		m_json = 0;
	}
	content.offset(0);

	// Create type
	item = choose_type(content);
	if(item)
	{
		// Ignore all blank
		wchar_t character = Json::skip_blank(content);

		// If the end of content reached
		if(character == 0)
		{
			m_json = dynamic_cast<ItemCollection*>(item);
		}
		else
		{
			// Problem another data in file
			delete item;
			result = false;
		}
	}
	else
	{
		// The file is empty
		result = false;
	}

	if (result == false)
	{
		throw_error(content);
	}
}

/** Serialize to json */
void Json::serialize(TextStream & out, int32_t indent)
{
	out.clear();
	if(m_json)
	{
		m_json->serialize(out, indent);
	}
}

/** Update the line and column for where the error is located */
void Json::throw_error(TextStream & content)
{
	JsonUnserializeError err;
	uint32_t offset_parse = 0;
	wchar_t character;
	err.m_line = 1;
	err.m_column = 1;
	uint32_t offset = content.offset();
	content.offset(0);
	do
	{
		character = content.read_char();
		if (character == '\n')
		{
			err.m_line ++;
			err.m_column = 1;
		}
		else if (character)
		{
			err.m_column ++;
		}
	}
	while (character && content.offset() < offset);
	throw err;
}

/** Get the count of items in the current node */
int32_t Json::count()
{
	if (m_json)
	{
		return m_json->count();
	}
	return 0;
}

#ifdef _DEBUG

void Json::test1()
{
	String out;
	ItemArray arr;

	arr.append(new ItemString("Deux"));
	arr.insert(0, new ItemString("Zero"));
	arr.insert(1, new ItemString("Un"));
	arr.insert(-1, new ItemString("five"));
	arr.insert(-2, new ItemString("four"));
	arr.insert(3, new ItemString("Trois"));
	arr.append(new ItemString("Six"));
	arr.insert(0,new ItemString("==="));
	arr.insert(-1,new ItemString("---"));
	arr.remove(-1);
	arr.remove(0);
	arr.replace(4, new ItemString("Quatre"));
	arr.replace(-2, new ItemString("Cinq"));

	out.clear();arr.serialize(out);assert(out == "[\"Zero\",\"Un\",\"Deux\",\"Trois\",\"Quatre\",\"Cinq\",\"Six\"]");
	out.clear();
	arr.remove(0);
	out.clear();arr.serialize(out);assert(out == "[\"Un\",\"Deux\",\"Trois\",\"Quatre\",\"Cinq\",\"Six\"]");
	arr.remove(-1);
	out.clear();arr.serialize(out);assert(out == "[\"Un\",\"Deux\",\"Trois\",\"Quatre\",\"Cinq\"]");
	arr.remove(-2);
	out.clear();arr.serialize(out);assert(out == "[\"Un\",\"Deux\",\"Trois\",\"Cinq\"]");
	arr.remove(2);
	out.clear();arr.serialize(out);assert(out == "[\"Un\",\"Deux\",\"Cinq\"]");
	arr.clear();
	out.clear();arr.serialize(out);assert(out == "[]");

	arr.append(new ItemArray);
	dynamic_cast<ItemArray*>(arr[0])->append(new ItemString("One"));
	arr.insert(1,new ItemArray);
	dynamic_cast<ItemArray*>(arr[1])->append(new ItemString("Two"));
	out.clear();arr.serialize(out);assert(out == "[[\"One\"],[\"Two\"]]");
}

void Json::test2()
{
	String out;
	ItemArray arr;
	arr.append(new ItemInt(123));
	arr.append(new ItemInt(-456));
	arr.append(new ItemString("hello"));
	arr.append(new ItemBool(true));
	arr.append(new ItemBool(false));
	out.clear(); arr.serialize(out);
}

void Json::test3()
{
	ItemObject main;
	main.set("toto",new ItemString("titi"));
	{
		ItemArray * arr = new ItemArray();
			arr->append(new ItemInt(123));
			arr->append(new ItemInt(-456));
			arr->append(new ItemString("hello"));
			arr->append(new ItemBool(true));
			arr->append(new ItemBool(false));
		ItemObject * obj = new ItemObject;

		main.set("array", arr);
		obj->set("one",new ItemInt(1));
		obj->set("two",new ItemString("2"));
		main.set("DEUX", obj);
	}

	String out;
	out.clear(); main.serialize(out,0);
	assert(out == "{\n"
		"	\"toto\" : \"titi\",\n"
		"	\"array\" : \n"
		"	[\n"
		"		123,\n"
		"		-456,\n"
		"		\"hello\",\n"
		"		true,\n"
		"		false\n"
		"	],\n"
		"	\"DEUX\" : \n"
		"	{\n"
		"		\"one\" : 1,\n"
		"		\"two\" : \"2\"\n"
		"	}\n"
		"}");
	out.clear(); main.serialize(out);
	assert(out == "{\"toto\":\"titi\",\"array\":[123,-456,\"hello\",true,false],\"DEUX\":{\"one\":1,\"two\":\"2\"}}");

	main.set("array",new ItemInt(1234));
	out.clear(); main.serialize(out);
	assert(out == "{\"toto\":\"titi\",\"array\":1234,\"DEUX\":{\"one\":1,\"two\":\"2\"}}");

	Item * deux_node = main["DEUX"];
}

void Json::test4()
{
	Json json;

	String buff(
		"		   // toto\n"
		"	  # comment \n"
		"		; other comment\n"
		"  					/* \n"
		"  jkhjlkkjh  */ /* * */ \n"
		" // titi\n"
		"[ -345 , 12, 0xA12a , 0o777, 0b11111111, 	 \n"
		"\"toto\\u20Ac \\\" \\\'	\n"
		"\"	, [		]	,	\n"
		" [ \n"
		" \"titi\"	,	"
		"[12		\n"
		" , null	,  true \n"
		",false,3,5	\n"
		"	 , 0x1234 \n"
		"]	\n"
		"]		\n"
		"	]	 "
		);
	json.unserialize(buff);

	String out;
	json.serialize(out);
	assert(out == "[-345,12,41258,511,255,\"toto€ \\\" '\\t\\n\",[],[\"titi\",[12,null,true,false,3,5,4660]]]");
}

void Json::test5()
{
	Json json;

	String buff(
		"		   // toto\n"
		"  					/* \n"
		"  jkhjlkkjh  */ /* * */ \n"
		" // titi\n"
		"{ 'value' : \n { 'try'		\n:		\n 'hello' } , 'toto':{'hello':'world'} , \n"
		"   /* klkjmlk \n"
		"  */\n"
		"// ljimloi\n"
		"	}"
		"    /* klkjmlk \n"
		"  */\n"
		"// ljimloi"
		);
	json.unserialize(buff);

	String out;
	json.serialize(out);
	assert(out == "{\"value\":{\"try\":\"hello\"},\"toto\":{\"hello\":\"world\"}}");
}

void Json::test6()
{
	String out;

	Json json("{'toto':{'titi':{'tutu':123,'tata':[456,222,333]},'tootoo':'789'},'tartufe':344,'bool':'1','nobool':'0','bb':true,'number':'0x1234','boolstr0':'false','boolstr1':'true'}");

	JsonIterator it(json);
	assert(it["toto"][0]["tata"][2] == 333);
	assert(it[0][0][1][1] == 222);

	assert(it["boolstr0"] == 0);
	assert(it["boolstr1"] == 1);
	

	assert(it["number"] == 0x1234);

	assert(it["bb"] == "true");
	assert(it["bb"] == 1);
	assert(it["bb"] == true);

	int bb = it["bb"];
	assert(bb);

	String bbs(it["bb"]);
	assert(bbs == "true");
	long long bbl = it["bb"];
	assert(bbl == 1);
	assert(it["bb"].is_null() == false);

	it["bb"] = false;
	bbs  = (String)it["bb"];
	json.serialize(out, 0);
	assert(bbs == "false");
	
	bool tartufe = it["tartufe"];
	assert(tartufe);
	bbs = (String)it["tartufe"];
	assert(bbs == "344");
	assert(it["tartufe"].is_null() == false);


	assert(it["toto"]["tootoo"] == "789");
	assert(it["toto"]["tootoo"] == 789);	
	assert(it["toto"]["tootoo"] == true);
	long long val = it["toto"]["tootoo"];
	assert(val == 789);
	assert(it["toto"]["tootoo"] == val);
	assert(it["toto"]["tootoo"] != (val-1));
	bool val2 = it["toto"]["tootoo"];
	assert(val2);

	assert(it["nobool"] == false);

	assert(it["bool"] == true);
	assert(it["bool"] == 1);
	assert(it["bool"] == "1");

	assert(it["toto"]["titi"]["tutu"] == "123");
	assert(it["toto"]["titi"]["tutu"] == true);
	assert((it["toto.titi.tutu_"] | (int)0xCAFEFADE) == 0xCAFEFADE);
}

void Json::test7()
{
	Json json;
	String s("{'toto':");
	s="['\\ \"\b\f\n\r\t\x02\u0003']";
	json.unserialize(s);
	json.serialize(s);
	assert(s == "[\" \\\"\\b\\f\\n\\r\\t\\x02\\x03\"]");

	s="['\\ \\\"\\b\\f\\n\\r\\t\\x02\\u0003']";
	json.unserialize(s);

	s="['toto";
	try{json.unserialize(s);assert(false);}catch(...){}
	s="		"; 
	try{json.unserialize(s);assert(false);}catch(...){}
	s="/*		";
	try{json.unserialize(s);assert(false);}catch(...){}
	s="/#		";
	try{json.unserialize(s);assert(false);}catch(...){}
}

void Json::test8()
{
	Json json;
	String s;
	json.unserialize("{'object':['a','b','c','d','e']}");
	JsonIterator it(json);

	assert(it["object"][0] == "a");
	assert(it["object"][1] == "b");
	assert(it["object"][2] == "c");
	assert(it["object"][3] == "d");
	assert(it["object"][4] == "e");
	assert((it["object"][5] | "Prout") == "Prout");

	assert((it["object"][-6] | "Prout") == "Prout");
	assert(it["object"][-5] == "a");
	assert(it["object"][-4] == "b");
	assert(it["object"][-3] == "c");
	assert(it["object"][-2] == "d");
	assert(it["object"][-1] == "e");

	json.unserialize("[{'a':'A','b':'B','c':'C','d':'D','e':'E'}]");
	assert(it[0][0] == "A");
	assert(it[0][1] == "B");
	assert(it[0][2] == "C");
	assert(it[0][3] == "D");
	assert(it[0][4] == "E");
	assert((it[0][5] | "Prout") == "Prout");

	assert((json[0][-6] | "Prout") == "Prout");
	assert(it[0][-5] == "A");
	assert(it[0][-4] == "B");
	assert(it[0][-3] == "C");
	assert(it[0][-2] == "D");
	assert(it[0][-1] == "E");

	json.unserialize("['a','b','c','d','e']");

	assert(it[0] == "a");
	assert(it[1] == "b");
	assert(it[2] == "c");
	assert(it[3] == "d");
	assert(it[4] == "e");
	assert((it[5] | "Prout") == "Prout");

	assert((it[-6]| "Prout") == "Prout");
	assert(it[-5] == "a");
	assert(it[-4] == "b");
	assert(it[-3] == "c");
	assert(it[-2] == "d");
	assert(it[-1] == "e");

	json.unserialize("{'a':'A','b':'B','c':'C','d':'D','e':'E'}");
	assert(it[0] == "A");
	assert(it[1] == "B");
	assert(it[2] == "C");
	assert(it[3] == "D");
	assert(it[4] == "E");
	assert((it[5] | "Prout") == "Prout");

	assert((it[-6] | "Prout") == "Prout");
	assert(it[-5] == "A");
	assert(it[-4] == "B");
	assert(it[-3] == "C");
	assert(it[-1] == "E");
	assert(it[-2] == "D");

	json.unserialize("{'object':{},'array':[],'integer':123,'string':'string','boolean':true, 'null':null}");
	assert(json["object"].type()  == JsonType::OBJECT);
	assert(json["integer"].type() == JsonType::INT);
	assert(json["string"].type()  == JsonType::STRING);
	assert(json["array"].type()   == JsonType::ARRAY);
	assert(json["boolean"].type() == JsonType::BOOL);
	assert(json["null"].type() == JsonType::NONE);

	try{json.unserialize("[0xAcDr]");assert(false);}catch(...){}
	json.unserialize("['\\\\']");

	json.unserialize("['\\\\']");
	json.serialize(s);
	assert(s == "[\"\\\"]");
}

void Json::test9()
{
	String s("{\n"
	"	\"toto\" : \"titi\",\n"
	"	\"array\" : \n"
	"	[\n"
	"		123a,\n"
	"	],\n"
	"}");
	Json json;

	try
	{
		json.unserialize(s);
		assert(false);
	}
	catch(JsonUnserializeError err)
	{
		assert(err.m_line == 5 && err.m_column == 7);
	}

	s = "{\n"
	"	\"toto\" : \"titi\",\n"
	"	array : \n"
	"	[\n"
	"		123,\n"
	"	],\n"
	"}";
	try
	{
		json.unserialize(s);
		assert(false);
	}
	catch(JsonUnserializeError err)
	{
		assert(err.m_line == 3 && err.m_column == 2);
	}
	
	s = "{noll}";
	try{json.unserialize(s);assert(false);}catch(...){}
	s = "{falsa}";
	try{json.unserialize(s);assert(false);}catch(...){}
	s = "{trie}";
	try{json.unserialize(s);assert(false);}catch(...){}

}

void Json::test10()
{
	Json json;
	String out;

	//  BOOL
	json["toto"]["tata"]["titi"] = true;
	
	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":true}}}");
	assert(json["toto"].count() == 1);
	assert(json["toto"]["toto"].count() == ~0);
	assert(json["toto"]["tata"].count() == 1);
	assert(json["toto"]["tata"]["titi"].count() == ~0);

	json["tutu"]["tuti"] = false;
	
	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":true}},\"tutu\":{\"tuti\":false}}");

	json["tutu"]["tuti"] = 4567;
	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":true}},\"tutu\":{\"tuti\":4567}}");

	// STRING
	json.clear();
	json["toto"]["tata"]["titi"] = "1234";

	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":\"1234\"}}}");
	assert(json["toto"].count() == 1);
	assert(json["toto"]["toto"].count() == ~0);
	assert(json["toto"]["tata"].count() == 1);
	assert(json["toto"]["tata"]["titi"].count() == ~0);

	json["tutu"]["tuti"] = "789";

	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":\"1234\"}},\"tutu\":{\"tuti\":\"789\"}}");

	json["tutu"]["tuti"] = 4567;

	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":\"1234\"}},\"tutu\":{\"tuti\":4567}}");

	// INTEGER
	json.clear();
	json["toto"] = 1234;
	json.serialize(out);
	assert(out == "{\"toto\":1234}");

	json["titi"] = 4567;
	json.serialize(out);
	assert(out == "{\"toto\":1234,\"titi\":4567}");


	json.clear();
	json["toto"]["tata"]["titi"] = 1234;
	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":1234}}}");
	assert(json["toto"].count() == 1);
	assert(json["toto"]["toto"].count() == ~0);
	assert(json["toto"]["tata"].count() == 1);
	assert(json["toto"]["tata"]["titi"].count() == ~0);

	json["tutu"]["tuti"] = 789;
	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":1234}},\"tutu\":{\"tuti\":789}}");

	json["tutu"]["tuti"] = 4567;
	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":1234}},\"tutu\":{\"tuti\":4567}}");

	json.clear();
	json["toto"] = 1234;
	json.serialize(out);
	assert(out == "{\"toto\":1234}");

	json["titi"] = 4567;
	json.serialize(out);
	assert(out == "{\"toto\":1234,\"titi\":4567}");

	json.clear();
}

void Json::test11()
{
	Json json;
	String out;

	// STRING
	json.clear();
	json["toto"]["tata"]["titi"][0] = "1234";
	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":[\"1234\"]}}}");

	json["toto"]["tata"]["titi"][0] = "4567";
	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":[\"4567\"]}}}");

	json["toto"]["tata"]["titi"][1] = "789";
	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":[\"4567\",\"789\"]}}}");

	json["toto"]["tata"]["titi"][55] = "555";
	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":[\"4567\",\"789\",\"555\"]}}}");

	json["toto"]["tata"]["titi"][-55] = "-555";
	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":[\"-555\",\"4567\",\"789\",\"555\"]}}}");

	json["toto"]["tata"]["titi"][-1] = "-111";
	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":[\"-555\",\"4567\",\"789\",\"-111\"]}}}");

	json["toto"]["tata"]["titi"][-2] = "-222";
	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":[\"-555\",\"4567\",\"-222\",\"-111\"]}}}");

	json["toto"]["tata"]["titi"][0] = "000";
	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":[\"000\",\"4567\",\"-222\",\"-111\"]}}}");

	json["toto"]["tata"]["titi"][1] = "111";
	json.serialize(out);
	assert(out == "{\"toto\":{\"tata\":{\"titi\":[\"000\",\"111\",\"-222\",\"-111\"]}}}");

}

void Json::test12()
{
	Json json;
	String out;

	json[0] = "789";
	json.serialize(out);
	assert(out == "[\"789\"]");

	json[0] = "888";
	json.serialize(out);
	assert(out == "[\"888\"]");

	json[-1] = "000";
	json.serialize(out);
	assert(out == "[\"000\"]");


	json[-1000] = "111";
	json.serialize(out);
	assert(out == "[\"111\",\"000\"]");

	json[1000] = "222";
	json.serialize(out);
	assert(out == "[\"111\",\"000\",\"222\"]");

	json[2] = "333";
	json.serialize(out);
	assert(out == "[\"111\",\"000\",\"333\"]");

	json[3] = "444";
	json.serialize(out);
	assert(out == "[\"111\",\"000\",\"333\",\"444\"]");

	json.clear();
	json["alphonse"][1]["toto"] = "789";
	json.serialize(out);
	assert(out == "{\"alphonse\":[{\"toto\":\"789\"}]}");

	json.clear();
	json.unserialize("{\"alphonse\":[\"123\"]}");
	json["alphonse"][1]["toto"] = "789";

	json.serialize(out);
	assert(out == "{\"alphonse\":[\"123\",{\"toto\":\"789\"}]}");

	json["alphonse"][1]["toto"] = "999";
	json.serialize(out);
	assert(out == "{\"alphonse\":[\"123\",{\"toto\":\"999\"}]}");

	json[2] = "999";
	json.serialize(out);
	assert(out == "{\"alphonse\":[\"123\",{\"toto\":\"999\"}]}");

	json["titi"] = "888";
	json.serialize(out);
	assert(out == "{\"alphonse\":[\"123\",{\"toto\":\"999\"}],\"titi\":\"888\"}");

	json.unserialize("{\"alphonse\":[\"123\",{'toto':'4567'}]}");
	json.serialize(out);
	assert(out == "{\"alphonse\":[\"123\",{\"toto\":\"4567\"}]}");

	json["alphonse"][1]["toto"] = "789";
	json.serialize(out);
	assert(out == "{\"alphonse\":[\"123\",{\"toto\":\"789\"}]}");


	json.clear();
	json["alphonse"][0x456] = "123";
	json.serialize(out);
	assert(out == "{\"alphonse\":[\"123\"]}");

	json["alphonse"][0] = "456";
	json.serialize(out);
	assert(out == "{\"alphonse\":[\"456\"]}");

	json["alphonse"][1]["toto"] = "789";
	json.serialize(out);
	assert(out == "{\"alphonse\":[\"456\",{\"toto\":\"789\"}]}");

}

void scan_child_next(JsonIterator i, String & out)
{
	for(i.first(); i.exist(); i.next())
	{
		scan_child_next(i.child(),out);
		if(i.has_child() == false)
		{
			out.write_format("%d,'%s',",(int)i.value(), i.key().c_str());
		}
	}
}

void scan_child_previous(JsonIterator i, String & out)
{
	for(i.last(); i.exist(); i.previous())
	{
		scan_child_previous(i.child(),out);
		if(i.has_child() == false)
		{
			out.write_format("%d,'%s',",(int)i.value(), i.key().c_str());
		}
	}
}

void Json::test13()
{
	Json json("{'alone':123,'toto':{'titi':{'tutu':456,'tata':[789,1011,1213]},'tootoo':'1415'},'tartufe':1617, 'list':[1819,2021,[2223,2324]]}");

	JsonIterator i(json);
	String out;
	scan_child_next(i,out);
	assert(out == "123,'alone',456,'tutu',789,'',1011,'',1213,'',1415,'tootoo',1617,'tartufe',1819,'',2021,'',2223,'',2324,'',");
	out.clear();
	scan_child_previous(i,out);
	assert(out == "2324,'',2223,'',2021,'',1819,'',1617,'tartufe',1415,'tootoo',1213,'',1011,'',789,'',456,'tutu',123,'alone',");
}

void Json::test14()
{
	Json json;

	JsonIterator win(json);
	String out;
		win["type"] = "window";
		json.serialize(out);
		assert(out == "{\"type\":\"window\"}");
		assert(win["type"] == "window");

		JsonIterator position = win["position"];
			position["x"] = 12;
			position["y"] = 34;
			json.serialize(out);
			assert(out == "{\"type\":\"window\",\"position\":{\"x\":12,\"y\":34}}");
			assert(position["x"] == 12);
			assert(position["y"] == 34);
			assert(win["position"]["x"] == 12);
			assert(win["position"]["y"] == 34);
			assert(win["type"] == "window");


		JsonIterator size = win["size"];
			size["width"] = 320;
			size["height"] = 240;
			json.serialize(out);
			assert(out == "{\"type\":\"window\",\"position\":{\"x\":12,\"y\":34},\"size\":{\"width\":320,\"height\":240}}");
			assert(size["width"] == 320);
			assert(size["height"] == 240);
			assert(win["size"]["width"] == 320);
			assert(win["size"]["height"] == 240);

		win["size"]["width"] = 325;
			json.serialize(out);
			assert(out == "{\"type\":\"window\",\"position\":{\"x\":12,\"y\":34},\"size\":{\"width\":325,\"height\":240}}");

		win["toto"]["titi"]["tata"]["toutou"] = "tutu";
			json.serialize(out);
			assert(out == "{\"type\":\"window\",\"position\":{\"x\":12,\"y\":34},\"size\":{\"width\":325,\"height\":240},\"toto\":{\"titi\":{\"tata\":{\"toutou\":\"tutu\"}}}}");
			assert(win["toto"]["titi"]["tata"]["toutou"] == "tutu");

		win["type3"]["type4"]["type5"] = "window5";
			json.serialize(out);
			assert(out == "{\"type\":\"window\",\"position\":{\"x\":12,\"y\":34},\"size\":{\"width\":325,\"height\":240},\"toto\":{\"titi\":{\"tata\":{\"toutou\":\"tutu\"}}},\"type3\":{\"type4\":{\"type5\":\"window5\"}}}");
			assert(win["type3"]["type4"]["type5"] == "window5");

		win["toto"]["toti"]["tata"]["toutou"] = "tuti";
			json.serialize(out);
			assert(out == "{\"type\":\"window\",\"position\":{\"x\":12,\"y\":34},\"size\":{\"width\":325,\"height\":240},\"toto\":{\"titi\":{\"tata\":{\"toutou\":\"tutu\"}},\"toti\":{\"tata\":{\"toutou\":\"tuti\"}}},\"type3\":{\"type4\":{\"type5\":\"window5\"}}}");
			assert(win["toto"]["toti"]["tata"]["toutou"] == "tuti");
}

// Test default value if missing
void Json::test15()
{
	String out;
	Json json;

	JsonIterator it(json);
	int integer = it["integer"] | 123;
	assert(integer == 123);

	it["integer"] = 789;
	integer = it["integer"] | 123;
	assert(integer == 789);
	assert(integer != 123);
	assert(it["integer"] != 123);


	long long long_long = it["long_long"] | 1234LL;
	assert(long_long == 1234LL);

	it["long_long"] = 78910LL;
	long_long = it["long_long"] | 1234LL;
	assert(long_long == 78910LL);
	assert(long_long != 1234LL);
	assert(it["long_long"] != 1234LL);
	assert(it["long_long"] == 78910LL);
	


	String string = it["string"] | "123";
	assert(string == "123");

	it["string"] = "789";
	string = it["string"] | "123";
	assert(string == "789");
	assert(string != "123");
	assert(it["string"] != "123");


	bool boolean = it["boolean"] | false;
	assert(boolean == false);

	it["boolean"] = true;
	boolean = it["boolean"] | false;
	assert(boolean == true);
	assert(boolean != false);
	assert(it["boolean"] != false);


	String other("Prout");
	String other_set("Burp");
	string = it["string2"] | other;
	assert(string == other);

	it["string2"] = other_set;
	string = it["string2"] | other;
	assert(string == other_set);
	assert(it["string2"] == other_set);
	assert(it["string2"] != other);

	json.serialize(out);
	assert(out  == "{\"integer\":789,\"long_long\":78910,\"string\":\"789\",\"boolean\":true,\"string2\":\"Burp\"}");

}

void Json::test16()
{
	Json json;
	String out;

	int integer = json["integer"] | 123;
	assert(integer == 123);

	json["integer"] = 789;
	integer = json["integer"] | 123;
	assert(integer == 789);


	long long long_long = json["long_long"] | 1234LL;
	assert(long_long == 1234LL);

	json["long_long"] = 78910LL;
	long_long = json["long_long"] | 1234LL;
	assert(long_long == 78910LL);


	String string = json["string"] | "123";
	assert(string == "123");

	json["string"] = "789";
	string = json["string"] | "123";
	assert(string == "789");


	bool boolean = json["boolean"] | false;
	assert(boolean == false);

	json["boolean"] = true;
	boolean = json["boolean"] | false;
	assert(boolean == true);

	json.serialize(out);
	assert(out  == "{\"integer\":789,\"long_long\":78910,\"string\":\"789\",\"boolean\":true}");
}

void Json::test17()
{
	Json json("{\"type\":\"window\",\"position\":{\"x\":12,\"y\":34},\"size\":{\"width\":325,\"height\":240},'boolean':true}");

	JsonIterator win(json);

		String type = win["type"];
		assert(type == "window");

		bool boolean = win["boolean"];
		assert(boolean);

		JsonIterator position = win["position"];
			assert(position["x"] == 12);
			assert(position["y"] == 34);

		int integer = position["x"];
		assert(integer == 12);

		long long long_long = position["y"];
		assert(long_long == 34);
		
		JsonIterator size = win["size"];
			assert(size["width" ] == 325);
			assert(size["height"] == 240);

}

void Json::test18()
{
	Json json;

	String buff(
		"# Python dictionnary\n"
		"{\n"
			"'''manylines'''        : \"\"\" Many \"\" \n lines 1 '''\"\"\",\n"
			"\"\"\"manylines2\"\"\" :    ''' Many '' \n \"\"\"lines 2 ''',\n"
			"'value' : None,\n"
			"'boolean' : True,\n"
			"'boolean2' : False,\n"
		"}"
		);
	json.unserialize(buff);

	assert(json["manylines"].is_null() == false);
	assert(json["value"].is_null() == true);
	assert(json["boolean"].is_null() == false);
	String out;
	json.serialize(out);
	assert(out == "{\"manylines\":\" Many \\\"\\\" \\n lines 1 '''\",\"manylines2\":\" Many '' \\n \\\"\\\"\\\"lines 2 \",\"value\":null,\"boolean\":true,\"boolean2\":false}");

	json.unserialize("{'long long':0x7ffffffffffffff}");
	assert(json["long long"] == 0x7ffffffffffffffLL);

	json.clear();
	json["toto"][Json::LAST] = 1;
	json["toto"][Json::LAST] = 2;
	json["toto"][Json::LAST] = 3;
	json["toto"][Json::LAST] = 4;
	json["toto"][Json::LAST] = 5;
	json["toto"][Json::LAST] = 6;

	json.serialize(out);
	assert(out == "{\"toto\":[1,2,3,4,5,6]}");

	json["toto"][0][0] = 123;
	json.serialize(out);
	assert(out == "{\"toto\":[[123],2,3,4,5,6]}");
	
	assert(json["toto"][0][0].remove());
	json.serialize(out);
	assert(out == "{\"toto\":[[],2,3,4,5,6]}");

	assert(json["toto"][0].remove());
	json.serialize(out);
	assert(out == "{\"toto\":[2,3,4,5,6]}");

	assert(json["toto"][-1].remove());
	json.serialize(out);
	assert(out == "{\"toto\":[2,3,4,5]}");

	assert(json["toto"][-1].remove());
	json.serialize(out);
	assert(out == "{\"toto\":[2,3,4]}");

	assert(json["toto"][-1].remove());
	json.serialize(out);
	assert(out == "{\"toto\":[2,3]}");

	assert(json["toto"][-1].remove());
	json.serialize(out);
	assert(out == "{\"toto\":[2]}");

	assert(json["toto"][-1].remove());
	json.serialize(out);
	assert(out == "{\"toto\":[]}");

	assert(json["toto"].remove());
	json.serialize(out);
	assert(out == "{}");

	json.unserialize("{'alone':123,'toto':{'titi':{'tutu':456,'tata':[789,1011,1213]},'tootoo':'1415'},'tartufe':1617, 'list':[1819,2021,[2223,2324]]}");
	assert(json["toto"]["titi"]["tutu"].remove());
	json.serialize(out);
	assert(out == "{\"alone\":123,\"toto\":{\"titi\":{\"tata\":[789,1011,1213]},\"tootoo\":\"1415\"},\"tartufe\":1617,\"list\":[1819,2021,[2223,2324]]}");

	assert(json["toto"]["titi"]["tata"].remove());
	json.serialize(out);
	assert(out == "{\"alone\":123,\"toto\":{\"titi\":{},\"tootoo\":\"1415\"},\"tartufe\":1617,\"list\":[1819,2021,[2223,2324]]}");

	assert(json["alone"].remove());
	json.serialize(out);
	assert(out == "{\"toto\":{\"titi\":{},\"tootoo\":\"1415\"},\"tartufe\":1617,\"list\":[1819,2021,[2223,2324]]}");

	assert(json["list"][2][1].remove());
	json.serialize(out);
	assert(out == "{\"toto\":{\"titi\":{},\"tootoo\":\"1415\"},\"tartufe\":1617,\"list\":[1819,2021,[2223]]}");

	assert(json["list"][2][0].remove());
	json.serialize(out);
	assert(out == "{\"toto\":{\"titi\":{},\"tootoo\":\"1415\"},\"tartufe\":1617,\"list\":[1819,2021,[]]}");

	assert(json["list"][2].remove());
	json.serialize(out);
	assert(out == "{\"toto\":{\"titi\":{},\"tootoo\":\"1415\"},\"tartufe\":1617,\"list\":[1819,2021]}");

	assert(json["list"].remove());
	json.serialize(out);
	assert(out == "{\"toto\":{\"titi\":{},\"tootoo\":\"1415\"},\"tartufe\":1617}");

	assert(json["toto"]["titi"].remove());
	json.serialize(out);
	assert(out == "{\"toto\":{\"tootoo\":\"1415\"},\"tartufe\":1617}");

	assert(json["toto"].remove());
	json.serialize(out);
	assert(out == "{\"tartufe\":1617}");

	assert(json[0].remove());
	json.serialize(out);
	assert(out == "{}");

	json.clear();
	json[Json::FIRST] = 4;
	json[Json::FIRST] = 3;
	json[Json::LAST]  = 5;
	json[Json::LAST]  = 6;
	json[Json::FIRST] = 2;
	json[Json::FIRST] = 1;
	json[Json::LAST]  = 7;
	json[Json::LAST]  = 8;

	json.serialize(out);
	assert(out == "[1,2,3,4,5,6,7,8]");

	json["toto"] = "titi";
	json.serialize(out);
	assert(out == "[1,2,3,4,5,6,7,8]");

	json[0]["toto"] = "titi";
	json.serialize(out);
	assert(out == "[{\"toto\":\"titi\"},2,3,4,5,6,7,8]");
}


void Json::test19()
{
	Json json(
			"{\n"
			"'keys':\n"
			"{\n"
				"'radius':7,\n"
				"'thickness':1,\n"
				"'margins':[3,3,3,3],\n"
				"'font':{'size':28},\n"
				"'colors':\n"
				"{\n"
					"'border'    :0xFF7ABBE8,\n"
					"'background':0xFFE7FCFF,\n"
					"'text'      :0xFF0060AF,\n"
				"}\n"
			"},\n"
			"'specialtkeys':\n"
			"{\n"
				"'radius':7,\n"
				"'thickness':1,\n"
				"'margins':[3,3,3,3],\n"
				"'font':{'size':40},\n"
				"'keys':'\x01\x02',\n"
				"'colors':\n"
				"{\n"
					"'border'    :0xFF7ABBE8,\n"
					"'background':0xFFE7FCFF,\n"
					"'text'      :0xFF0060AF,\n"
				"}\n"
			"},\n"
			"'mappings':\n"
			"[\n"
				"{'shift':{'text':null, 'goto':1}, 'caps':{'text':'123', 'goto':3}, 'mapping':'qwertyuiop\\nasdfghjkl\\'\\n\\u2B06zxcvbnm\\u232B\\n\x02 \\u21A9'},\n"
				"{'shift':{'text':null, 'goto':0}, 'caps':{'text':'123', 'goto':3}, 'mapping':'QWERTYUIOP\\n_a_s_d_f_g_h_j_k_l\\'\\n\\u2B06ZXCVBNM\\u232B\\n\x02 \\u21A9'},\n"
				"{'shift':{'text':null, 'goto':3}, 'caps':{'text':'ABC', 'goto':0}, 'mapping':'1234567890\\n-/:;()\\u20AC&@\"\\n\\u2B06.,?!\\'\\u232B\\n\x01 \\u21A9'},\n"
				"{'shift':{'text':null, 'goto':2}, 'caps':{'text':'ABC', 'goto':0}, 'mapping':'[]{}#\x25^*+=\\n_\\|/~<>$\\u00A3\\u00A5\"\\n\\u2B06.,?!\\'\\u232B\\n\x01 \\u21A9'}\n"
			"]\n"
		"}");

	int count = json.count();
	for (int i = 0; i < json.count(); i++)
	{
		int    shift   = json["mappings"][i]["shift"]["goto"]   | 0;
		int    caps    = json["mappings"][i]["caps"]["goto"]    | 0;
		String mapping = json["mappings"][i]["mapping"] | "";
	}
}

void size_serialize(JsonIterator & parent, uint32_t width, uint32_t height)
{
	parent["size"]["width" ] = (int)width;
	parent["size"]["height"] = (int)height;
}

void position_serialize(JsonIterator & parent, int32_t x, int32_t y)
{
	parent["position"]["x"] = x;
	parent["position"]["y"] = y;
}

void size_unserialize(JsonIterator & parent, uint32_t & width, uint32_t & height)
{
	width  = (int)parent["size"]["width"];
	height = (int)parent["size"]["height"];
}

void position_unserialize(JsonIterator & parent, int32_t & x, int32_t & y)
{
	y = parent["position"]["y"];
	x = parent["position"]["x"];
}

void Json::test20()
{
	Json json;
	String out;
	JsonIterator win(json);
		win["type"] = "window";

		position_serialize(win, 12, 34);
		size_serialize(win, 320,240);

	json.serialize(out);
	Json json2(out);
		JsonIterator win2(json);
		int32_t x;
		int32_t y;
		uint32_t width;
		uint32_t height;
		assert(json2["type"] == "window");

		position_unserialize(win2, x, y);
		assert(x == 12 && y == 34);
		size_unserialize(win2, width, height);
		assert(width == 320 && height == 240);
}

void Json::test21()
{
	{
		Json json;
		String out;
		JsonIterator win(json);
		win["position"]["x_"] = 12345;
		win["position"]["y_"] = -34456;
		win["position"]["z"] = 789;
		win["position"]["w"] = -555;
		json.serialize(out);
		assert(out == "{\"position\":{\"x\":192.89,\"y\":-538.38,\"z\":789,\"w\":-555}}");
	}

	{
		Json json;
		String out;
		json.unserialize("{\"position\":{\"x\":192.89,\"y\":-538.38,\"z\":789,\"w\":-555}}");
		JsonIterator win(json);

		int x = win["position"]["x_"];
		int y = win["position"]["y_"];
		int z = win["position"]["z_"];
		int w = win["position"]["w_"];
		assert(x == 12344 && y ==-34456 && z == 50496 && w == -35520);
		
		x = win["position"]["x"];
		y = win["position"]["y"];
		z = win["position"]["z"];
		w = win["position"]["w"];
		
		assert(x == 192 && y ==-539 && z == 789 && w == -555);
		x = y;
	}
}

void Json::test22()
{
}

void Json::test23()
{
}

void Json::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
	#ifdef _WIN32
		DeviceWin32::show_console();
	#endif
		test23();
		test22();
		test21();
		test20();
		test19();
		test18();
		test17();
		test16();
		test15();
		test14();
		test13();
		test12();
		test11();
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
}

#endif