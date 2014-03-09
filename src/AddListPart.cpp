//#define TEST_AddListPart


#ifdef TEST_AddListPart
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
using namespace std;
// Add the Find module to the compilation:
#include "FindListPart.cpp"
#endif

#include "ValueList.h"

//#undef DBG_OFF
#include "dbg.h"

#ifdef TEST_FindListPart
#	define CATCH_CONFIG_MAIN
#endif
#include "catch.hpp"


// Add a substring to a ';'-sepadated list of substrings, if not yet there.
// Prepending or appending according to ADD_MODE.
// See FindListPart() for more details.
enum ENUM_CLASS ADD_MODE { APPEND, PREPEND };
string AddListPart(const string& sequence_in, const string& element_in, MATCH_TYPE flags = MATCH_TYPE::PATH, ADD_MODE add_mode = ADD_MODE::APPEND)
//!!FIXME: element itself can also be a sequence!
{
	if (element_in.empty()) {
		return sequence_in;
	}
DBGMARK

	if (FindListPart(sequence_in, element_in, flags) != string::npos)
	{
#ifdef TEST_AddListPart
DBG "['", element_in, "' IS ALREADY THERE, NOT ADDING!]";
#endif
		return sequence_in;
	}
DBGMARK
	
	string result;
	if (sequence_in.empty()) {
		result = element_in;
	} else if (add_mode == ADD_MODE::APPEND) {
		result = sequence_in;
		result += ELEMENT_SEPARATOR;
		result += element_in;
	} else if (add_mode == ADD_MODE::PREPEND) {
		result = element_in;
		result += ELEMENT_SEPARATOR;
		result += sequence_in;
	}	
	return result;
}
#ifdef TEST_AddListPart
int main()
{
	char* sequence1 = "c:\\tmp;;;X:\\In;;;;;d:\\tools;;dummy;";

	DBG "--------------- APPEND...";
	DBG_(sequence1);
	DBG_(AddListPart(sequence1, "c:\\tmp"));
	DBG_(AddListPart(sequence1, "x:\\in"));
	DBG_(AddListPart(sequence1, "Dummy"));
	DBG_(AddListPart(sequence1, "xxx"));
	PAUSE

	DBG "--------------- PREPEND...";
	DBG_(sequence1);
	DBG_(AddListPart(sequence1, "c:\\tmp", MATCH_TYPE::PATH, ADD_MODE::PREPEND));
	DBG_(AddListPart(sequence1, "x:\\in", MATCH_TYPE::PATH, ADD_MODE::PREPEND));
	DBG_(AddListPart(sequence1, "dummy", MATCH_TYPE::PATH, ADD_MODE::PREPEND));
	DBG_(AddListPart(sequence1, "xxx", MATCH_TYPE::PATH, ADD_MODE::PREPEND));
	PAUSE

	DBG "--------------- APPEND 2...";
	char* sequence2 = "e:\\";
	DBG_(sequence2);
	DBG_(AddListPart(sequence2, "c:\\tmp"));
	PAUSE

	DBG "--------------- APPEND TO EMPTY...";
	char* sequence3 = "";
	DBG_(sequence3);
	DBG_(AddListPart(sequence3, "c:\\tmp"));
	PAUSE

	return 0;
}
#endif