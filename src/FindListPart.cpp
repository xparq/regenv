#include <algorithm>
#include <iterator>
//#include <locale>
using namespace std;

#ifdef TEST_FindListPart
#include <iostream>
#include <string>
//using namespace std;
#endif

#include "ValueList.h"

//!!NOTE: As this file is currently just #included directly into regenv.cpp, 
//!!      setting this flag here would interfere with the one set there!
//#undef DBG_OFF
#include "dbg.h"


#ifdef TEST_FindListPart
#	define CATCH_CONFIG_MAIN
#endif
#include "catch.hpp"


char __still_no_lambda__normalize_case(char c) { return ::tolower(c); }	
char __still_no_lambda__normalize_case_and_slash(char c) { return (c == '\\' ? '/' : ::tolower(c)); }	

CHARPOS FindListPart(const string sequence_in, const string element_in, MATCH_TYPE flags = MATCH_TYPE::PATH, CHARPOS pos = 0)
// Find 'element' in a ';'-sepadated 'sequence' of elements (substrings), starting 
// from 'pos', and return its offset if found, else string::npos.
//
// If flags has MATCH_TYPE::PATH then '/' and '\' are treated the same.
//
// The search is not case-sensitive.
//
// NOTE:
//	An element "en" can't be just string-searched, because in a "seq;uence" 
//	it would be incorrectly matched. The comparison should be done on 
//	complete list elements (non-separator chars between separators). 
//	The beginning (the virtual "-1st" char) and the end (EOS) of 'sequence'
//	are also considered separators.
//
//!!TODO:
// !	If 'element' itself is a set of sub-elements (a ;-separated list), 
//	its own parts should be searched for one by one in 'sequence', too, 
//	otherwise it'd be an "all or nothing" case, where all the parts would 
//	only be found if 'sequence' happens to contain 'element' verbatim (which 
//	is probably a common case, in fact).
//	This practically means converting the 'sequence' and 'element' to
//	ordered sets, then merging them into 'sequence', while preserving 
//	the order of those elements existing in both, as found in 'sequence' 
//	(not in 'element').
//
// !	Add locale/UTF support (everywhere, not just here)!
{
	if (sequence_in.empty()) {
		return string::npos;
	}
	if (element_in.empty()) {
		return 0;
	}

	// Normalize both sequence and element to:
	// a) be case-insensitive
	// b) if PATH-type: contain only forward slashes
	auto f_xlat = (flags & MATCH_TYPE::PATH
				? __still_no_lambda__normalize_case_and_slash
				: __still_no_lambda__normalize_case);
	string sequence, element;
	transform(sequence_in.begin(), sequence_in.end(), back_inserter(sequence), f_xlat);
	transform(element_in.begin(), element_in.end(), back_inserter(element), f_xlat);	

DBG_(sequence);
DBG_(element);

	CHARPOS seqlen = sequence.length();
	CHARPOS elemlen = element.length();

	CHARPOS seq_partstart_pos = pos;
	CHARPOS seq_partend_pos; // the char AFTER a part (i.e. a separator or EOS)

	while (seq_partstart_pos < seqlen)
	{
		// Skip leading separators, in case we are sitting on any.
		while (sequence[seq_partstart_pos] == ELEMENT_SEPARATOR) {
			++seq_partstart_pos;
		}
		// Note: we may be standing at EOS now. If yes, let's make
		// everything less complicated (and prone to off-by-one access 
		// violations) by catapulting right now.
		if (!sequence[seq_partstart_pos])
			break;

		// 'element' is found if the substring of 'sequence' from 'seq_partstart_pos'
		// matches 'element', AND the next position is either ELEMENT_SEPARATOR, or EOS.
		CHARPOS seq_partend_pos = sequence.find(ELEMENT_SEPARATOR, seq_partstart_pos);
		if (seq_partend_pos == string::npos)
		    seq_partend_pos = seqlen;

		string part = sequence.substr(seq_partstart_pos, seq_partend_pos - seq_partstart_pos);
DBG_(part);
		if (part == element) // found!
		{
			return seq_partstart_pos;
		}

		seq_partstart_pos = seq_partend_pos; // move to the trailing separator (or EOS)
	}

	return string::npos; // not found
}

#if defined(TEST_FindListPart)

TEST_CASE("--------------- Sporadic crashes on MSVC", "[path-crash]") {
	REQUIRE(FindListPart("123456", "123456_123456XXX", MATCH_TYPE::NORMAL) == string::npos);
	REQUIRE(FindListPart("123456", "123456_123456XXX", MATCH_TYPE::PATH) == string::npos);
}


const char* sequence1 = "c:\\tmp;X:\\In;;;d:\\tools;;fakedummy;dummyfake;dummy";

TEST_CASE("--------------- Basic non-PATH-type matching") {

 SECTION("Only match whole list parts, not substrings") {
	REQUIRE(FindListPart(sequence1, "dummy", MATCH_TYPE::NORMAL) == 45);
 }
 SECTION("Caseless matching") {
	REQUIRE(FindListPart(sequence1, "Dummy", MATCH_TYPE::NORMAL) == 45);
 }
 SECTION("Item not to be found") {
	REQUIRE(FindListPart(sequence1, "xxx", MATCH_TYPE::NORMAL) == string::npos);
 }
 SECTION("Paths with backslashes, to be found (concidentally)") {
	REQUIRE(FindListPart(sequence1, "c:\\tmp", MATCH_TYPE::NORMAL) == 0);
	REQUIRE(FindListPart(sequence1, "x:\\in",  MATCH_TYPE::NORMAL) == 7);
 }
 SECTION("Paths with forward slashes, NOT to be found") {
	REQUIRE(FindListPart(sequence1, "c:/tmp", MATCH_TYPE::NORMAL) == string::npos);
	REQUIRE(FindListPart(sequence1, "x:/in",  MATCH_TYPE::NORMAL) == string::npos);
 }
}

TEST_CASE("--------------- Basic PATH-type matching") {

 SECTION("Only match whole list parts, not substrings") {
	REQUIRE(FindListPart(sequence1, "dummy", MATCH_TYPE::PATH) == 45);
 }
 SECTION("Caseless matching") {
	REQUIRE(FindListPart(sequence1, "Dummy", MATCH_TYPE::PATH) == 45);
 }
 SECTION("Item not to be found") {
	REQUIRE(FindListPart(sequence1, "xxx", MATCH_TYPE::PATH) == string::npos);
 }
 SECTION("Paths with backslashes, to be found") {
	REQUIRE(FindListPart(sequence1, "c:\\tmp", MATCH_TYPE::PATH) == 0);
	REQUIRE(FindListPart(sequence1, "x:\\in",  MATCH_TYPE::PATH) == 7);
 }
 SECTION("Paths with forward slashes, to be found") {
	REQUIRE(FindListPart(sequence1, "c:/tmp", MATCH_TYPE::PATH) == 0);
	REQUIRE(FindListPart(sequence1, "x:/in",  MATCH_TYPE::PATH) == 7);
 }
}

TEST_CASE("--------------- Positioned non-PATH", "[hide]") {

 SECTION(sequence1) {
	DBG_(FindListPart(sequence1, "c:\\tmp", MATCH_TYPE::NORMAL, 1));
	DBG_(FindListPart(sequence1, "x:\\in", MATCH_TYPE::NORMAL, 1));
	DBG_(FindListPart(sequence1, "dummy", MATCH_TYPE::NORMAL, 34));
	DBG_(FindListPart(sequence1, "xxx", MATCH_TYPE::NORMAL, 1));
 }
}

TEST_CASE("--------------- PATH: Case and slash differences", "[hide]") {

 SECTION(sequence1) {
	DBG_(FindListPart(sequence1, "C:\\TMP", MATCH_TYPE::PATH));
	DBG_(FindListPart(sequence1, "C:/Tmp", MATCH_TYPE::PATH));
	DBG_(FindListPart(sequence1, "x:/IN", MATCH_TYPE::PATH));
	DBG_(FindListPart(sequence1, "DUMmy", MATCH_TYPE::PATH));
	DBG_(FindListPart(sequence1, "xXx", MATCH_TYPE::PATH));
 }
}

TEST_CASE("--------------- Compound element", "[hide]") {
 SECTION(sequence1) {
	DBG_(FindListPart(sequence1, "fakedummy;dummyfake", MATCH_TYPE::PATH));
	DBG_(FindListPart(sequence1, "faked", MATCH_TYPE::NORMAL));
 }
}

#endif
