#ifdef TEST_DelListPart
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


// Delete a substring from a ';'-sepadated list of substrings.
// See FindListPart() for details.
string DelListPart(const string& sequence_in, const string& element_in, MATCH_TYPE flags = MATCH_TYPE::PATH, string::size_type start_pos = 0)
//!!FIXME: element itself can also be a sequence!
{
	if (sequence_in.empty()) {
		return sequence_in;
	}
	if (element_in.empty()) {
		return sequence_in;
	}

	string::size_type pattern_pos = FindListPart(sequence_in, element_in, flags, start_pos);
	if (pattern_pos == string::npos) {
#ifdef TEST_DelListPart
DBG "[", element_in, " NOT FOUND!]";
#endif
		return sequence_in;
	}

#ifdef TEST_DelListPart
DBG "[", element_in, " FOUND, DELETING...]";
#endif
	string::size_type len = element_in.length();
	string::size_type endpart_pos = pattern_pos + element_in.length();
	// Also delete any following ELEMENT_SEPARATOR(s).
	// Note: if the substring is the first in the list, its trailing
	// ELEMENT_SEPARATORS will become redundant. If it's in the middle,
	// then it's also preceded with a separator, which will be kept.
	// If it's the last substring, trailing separators are also redundant.
	while (sequence_in[endpart_pos] == ELEMENT_SEPARATOR)
		endpart_pos++;

	// Delete any preceeding ELEMENT_SEPARATORS, too, in case pattern is
	// the last part in sample!
	if (endpart_pos >= sequence_in.length()) {
#ifdef TEST_DelListPart
DBG "Deleting preceeding PATH_SEP back from ", pattern_pos,
    " (endpart_pos == ", endpart_pos, ", sample.len == ", sequence_in.length(), ")?";
#endif
		while (pattern_pos > start_pos && sequence_in[pattern_pos-1] == ELEMENT_SEPARATOR) {
			pattern_pos--;
#ifdef TEST_DelListPart
DBG "Stepped back 1 char.";
#endif
		}
	}
	
	return sequence_in.substr(0, pattern_pos) + sequence_in.substr(endpart_pos, string::npos);
}
#ifdef TEST_DelListPart
int main()
{
	char* sequence1 = "c:\\tmp;;;X:\\In;;;;;d:\\tools;;dummy;";

	DBG "--------------- Basic non-PATH...";
	DBG_(sequence1);
	DBG_(DelListPart(sequence1, "c:\\tmp", MATCH_TYPE::NORMAL));
	DBG_(DelListPart(sequence1, "x:\\in", MATCH_TYPE::NORMAL));
	DBG_(DelListPart(sequence1, "Dummy", MATCH_TYPE::NORMAL));
	DBG_(DelListPart(sequence1, "xxx", MATCH_TYPE::NORMAL));
	PAUSE

	DBG "--------------- Positioned non-PATH...";
	DBG_(sequence1);
	DBG_(DelListPart(sequence1, "c:\\tmp", MATCH_TYPE::NORMAL, 1));
	DBG_(DelListPart(sequence1, "x:\\in", MATCH_TYPE::NORMAL, 1));
	DBG_(DelListPart(sequence1, "dummy", MATCH_TYPE::NORMAL, 1));
	DBG_(DelListPart(sequence1, "xxx", MATCH_TYPE::NORMAL, 1));
	PAUSE

	DBG "--------------- PATH type: case and slash differences...";
	DBG_(sequence1);
	DBG_(DelListPart(sequence1, "C:\\TMP"));
	DBG_(DelListPart(sequence1, "C:/Tmp"));
	DBG_(DelListPart(sequence1, "x:/IN"));
	DBG_(DelListPart(sequence1, "DUMmy"));
	DBG_(DelListPart(sequence1, "xXx"));
	PAUSE

	DBG "--------------- Element separators...";
	char* sequence2 = ";;;CRAP;;";
	DBG_(sequence2);
	DBG_(DelListPart(sequence2, "crap"));
	char* sequence3 = "CRAP;;";
	DBG_(sequence3);
	DBG_(DelListPart(sequence3, "crap"));
	char* sequence4 = "CRAP";
	DBG_(sequence4);
	DBG_(DelListPart(sequence4, "crap"));
	PAUSE
	
	DBG "--------------- Empty sample...";
	char* sequence_empty = "";
	DBG_(DelListPart(sequence_empty, "crap"));	
	DBG "--------------- Almost empty sample...";
	char* sequence_almostempty = ";";
	DBG_(DelListPart(sequence_almostempty, "crap"));
	PAUSE

	return 0;
}
#endif