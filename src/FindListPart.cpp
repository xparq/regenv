#include <algorithm>
#include <iterator>
//#include <locale>
using namespace std;

#ifdef TEST_FindListPart
#include <iostream>
#include <string>
//using namespace std;
#endif

#include "ListPart.h"

//#undef DBG_OFF
#include "dbg.h"


char __still_no_lambda__normalize_case(char c) { return ::tolower(c); }	
char __still_no_lambda__normalize_case_and_slash(char c) { return (c == '\\' ? '/' : ::tolower(c)); }	

string::size_type FindListPart(const string& sequence_in, const string& element_in, LIST_TYPE list_type = LIST_TYPE::PATH, string::size_type pos = 0)
// Find a substring in a ';'-sepadated sequence of substring elements, 
// starting from 'pos', and return its offset if found, else string::npos.
//
// The search is not case-sensitive, and if is_path then treats '/' and '\' 
// as being the same.
//
// NOTE: An element "en" can't be just string-searched, because in a "seq;ence;" 
//       it would be incorrectly found. The comparison should be done on complete
//       list elements, which are non-separator chars between separators, where
//       the beginning (the virtual "-1st" char) and the end (EOS) of 'sequence'
//       are also considered separators.
//
//       Also, if 'element' itself is a list, its component sub-elements should be
//       searched one by one in 'sequence', otherwise it'd be an "all or nothing" 
//       business, where (all) the sub-elements would only be found if 'sequence'
//       happens to contain 'element' (the whole sub-element list) verbatim (which 
//       is probably quite a common case, in fact).
//       This version only supports this "all or nothing" case.
//
//!!FIXME: Can we be more useful if element itself is also a sequence?
//	At least we could trim leading/trailing path separators and path-separtors
//	from element before comparison, but that would be too much hassle for low gain.
//!!FIXME: Add locale/UTF support (everywhere, not just here)!
{
	if (sequence_in.empty()) {
		return string::npos;
	}
	if (element_in.empty()) {
		return 0;
	}

	// Normalize both sequence and element to (a) be case-insensitive, (b) containing only forward slashes.
	auto f_xlat = (list_type == LIST_TYPE::PATH
				? __still_no_lambda__normalize_case_and_slash
				: __still_no_lambda__normalize_case);
	string sequence, element;
	transform(sequence_in.begin(), sequence_in.end(), back_inserter(sequence), f_xlat);
	transform(element_in.begin(), element_in.end(), back_inserter(element), f_xlat);	
		
DBG_(sequence);
DBG_(element);

	string::size_type seqlen = sequence.length();
	string::size_type elemlen = element.length();
	string::size_type seq_partstart_pos = pos;
	string::size_type seq_partend_pos; // the char AFTER a part (i.e. a separator or EOS)
	while (seq_partstart_pos < seqlen)
	{
		// Skip leading separators, in case we are sitting on any.
		while (sequence[seq_partstart_pos] == ELEMENT_SEPARATOR) {
			++seq_partstart_pos;
		}
		// Note: we may be standing at EOS now. If yes, let's make
		// everything less complicated (and prone to off-by-one access 
		// violations) by just escaping now...
		if (!sequence[seq_partstart_pos])
			break;

		// 'element' is found if the substring of 'sequence' from 'seq_partstart_pos'
		// matches 'element', AND the next position is either ELEMENT_SEPARATOR, or EOS.
		string::size_type seq_partend_pos = sequence.find(ELEMENT_SEPARATOR, seq_partstart_pos);
		if (seq_partend_pos == string::npos)
		    seq_partend_pos = seqlen;

		string part = sequence.substr(seq_partstart_pos, elemlen/*or as many left, is shorter*/);
DBG_(part);
		if (part == element) // found!
		{
			return seq_partstart_pos;
		}

		seq_partstart_pos = seq_partend_pos; // move to the trailing separator (or EOS)
	}

	return string::npos; // not found
}
#ifdef TEST_FindListPart
int main()
{
	char* sequence1 = "c:\\tmp;X:\\In;;;d:\\tools;;fakedummy;dummyfake;dummy";

	DBG "--------------- Basic non-PATH...";
	DBG_(sequence1);
	DBG_(FindListPart(sequence1, "c:\\tmp", LIST_TYPE::NORMAL));
	DBG_(FindListPart(sequence1, "x:\\in", LIST_TYPE::NORMAL));
	DBG_(FindListPart(sequence1, "Dummy", LIST_TYPE::NORMAL));
	DBG_(FindListPart(sequence1, "xxx", LIST_TYPE::NORMAL));
	PAUSE

	DBG "--------------- Positioned non-PATH..";
	DBG_(FindListPart(sequence1, "c:\\tmp", LIST_TYPE::NORMAL, 1));
	DBG_(FindListPart(sequence1, "x:\\in", LIST_TYPE::NORMAL, 1));
	DBG_(FindListPart(sequence1, "dummy", LIST_TYPE::NORMAL, 34));
	DBG_(FindListPart(sequence1, "xxx", LIST_TYPE::NORMAL, 1));
	PAUSE

	DBG "--------------- PATH: Case and slash differences...\n";
	DBG_(FindListPart(sequence1, "C:\\TMP", LIST_TYPE::PATH));
	DBG_(FindListPart(sequence1, "C:/Tmp", LIST_TYPE::PATH));
	DBG_(FindListPart(sequence1, "x:/IN", LIST_TYPE::PATH));
	DBG_(FindListPart(sequence1, "DUMmy", LIST_TYPE::PATH));
	DBG_(FindListPart(sequence1, "xXx", LIST_TYPE::PATH));
	PAUSE

	DBG "--------------- Compound element...\n";
	char* sequence_compo1 = "c:\\tmp;X:\\In;;;d:\\tools;;fakedummy;dummyfake;dummy";
	DBG_(sequence_compo1);
	DBG_(FindListPart(sequence1, "fakedummy;dummyfake", LIST_TYPE::PATH));
	DBG_(FindListPart(sequence1, "faked", LIST_TYPE::NORMAL));
	PAUSE
	
	return 0;
}
#endif