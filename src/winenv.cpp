// License - Free for any purpose, just do not remove the copyright
// Copyright (C) 1997-2007 - Jonathan Wilkes
// Copyright (C) 2012-2013 - Szabolcs Szasz
// See README for more details, e.g. change history and licensing.

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#include <tchar.h>	// _T()

#include <iostream>
#include <string>
using namespace std;

// This is only effective before the first include of dbg.h
#define DBG_OFF

// Merge in some code
// (...which is fairly generic and should be modularized further out)
#include "FindListPart.cpp"
#include "AddListPart.cpp"
#include "DelListPart.cpp"

// Some more includes...
#include <set>
#include <map>
#include "winreg.h"

#include "dbg.h"

/*!!TODO: Can I easily create a transparent UNICODE/ANSI ostream?...
template <typename CHARTYPE> struct autostream : public ostream
{
//...
};
autostream<char> out;
!!*/

// Just a convenience helper (could be a derived class of set<char>, but not now...):
#define OPTION(optchar)     (options.find(optchar) != options.end())
#define NO_OPTION(optchar) (options.find(optchar) == options.end())


//---------------------------------------------------------------------------
// Exit codes...
//
const int RETURN_SUCCESS = 0;
const int RETURN_NOT_FOUND = 1;
const int RETURN_ERROR = 2;
const int RETURN_ACCESS_DENIED = 5;


//---------------------------------------------------------------------------
// Registry locations for the env. variables...
//
struct regkey { HKEY rootkey; TCHAR* path; };
// System variables
const regkey REG_ENV_SYSTEM = { HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment") };
// User variables
const regkey REG_ENV_USER   = { HKEY_CURRENT_USER, _T("Environment") };
// Session variables
const regkey REG_ENV_SESSION = { HKEY_CURRENT_USER, _T("Volatile Environment") };


//---------------------------------------------------------------------------
#include "helptext.h"

void ShowBanner()
{
	cout << _T("WINENV 0.94 - Copyright (C) 2012-2013 Szabolcs Szasz, 2005-2008 Jonathan Wilkes\n\n");
}

// Do not call this automatically in case of user errors, because
// it writes to stdout (assuming explicit user request), not stderr!
void Usage()
{
	ShowBanner();
	cout << HELP_TEXT;
}

void ShowExamples()
{
	ShowBanner();
	cout << EXAMPLES_TEXT;
}


//---------------------------------------------------------------------------
int DoNotify();	// stupid forward declare :-(
int DoSet(regkey& envspace, string varname, string value, set<char> options)
{
	StupidRegKey regkey(envspace.rootkey, envspace.path);
	
	if (regkey.status != ERROR_SUCCESS) {
		cerr << "- ERROR: cannot access '"<<envspace.path<<"'! (Err. code: "<<regkey.status<<")"<<endl;
		return RETURN_ERROR;
	}
	
	regkey.CheckValue_STR(varname);
DBG "regkey.CheckValue_STR() --> status == ", regkey.status;
	if (regkey.IsNewValue()) {
		// OK, just no 'varname' yet; set it to 'value', regardless of -a, -p, -s!
DBG "GO TO DIRECT SET, BECAUSE THIS IS A NEW VALUE...";
		goto SKIP_TO_SET;
	} else if (regkey.status != ERROR_SUCCESS) {
		cerr << "- ERROR: reading '"<<varname<<"' failed! (Err. code: "<<regkey.status<<")"<<endl;
		return RETURN_ERROR;
	}

	// So, 'varname' already exists - update it...

	// 1. No fancy options, just set it!
	if (NO_OPTION('s') && NO_OPTION('a') && NO_OPTION('p')) {
DBG "GO TO DIRECT SET, AS REQUESTED...";
		goto SKIP_TO_SET;
	}

	// 2. We'll need the original value, so read it:
	regkey.GetValue_STR(varname);
	if (regkey.status != ERROR_SUCCESS) {
		cerr << "- ERROR: getting value of '"<<varname<<"' failed! (Err. code: "<<regkey.status<<")"<<endl;
		return RETURN_ERROR;
	}

	// 3. Handle substring mode differently:
	if (OPTION('s')) {
DBG "UPDATING EXISTING LIST VAR...";
	value = AddListPart(regkey.current_value, value, LIST_TYPE::PATH, 
			OPTION('p') ? ADD_MODE::PREPEND : ADD_MODE::APPEND);

	} else {
DBG "UPDATING EXISTING NORMAL VAR...";
		// "Plain" -p and -a processing here:
		//	a) NOT trying to find <value> in the var first, and
		//	b) not inserting a separator either!
		if (OPTION('p')) {
			value += regkey.current_value;
		} else {
			value = regkey.current_value + value;
		}
	}

SKIP_TO_SET:

DBG "SETTING '", varname, "' TO '", value, "' in '", envspace.path, "' AS ",
    (OPTION('x') ? REG_EXPAND_SZ : 0/*use default*/);
	regkey.SetValue_STR(varname, value, 
		(OPTION('x') ? REG_EXPAND_SZ : 0/*use default*/));
	if (regkey.status == ERROR_SUCCESS) {
		// Forced notification?
		if (OPTION('!')) DoNotify(); //!! result ignored...
		return RETURN_SUCCESS;
	} else {
		cerr << "- ERROR: Setting '"<<varname<<"' to '"<<value<<"' FAILED! (Err. code: "<<regkey.status<<")"<<endl;
		return RETURN_ERROR;
	}
}


//---------------------------------------------------------------------------
int DoDel(regkey& envspace, string varname, string value, set<char> options)
{
	StupidRegKey regkey(envspace.rootkey, envspace.path);

	if (regkey.status != ERROR_SUCCESS) {
		cerr << "- ERROR: cannot access '"<<varname<<"'! (Err. code: "<<regkey.status<<")"<<endl;
		return RETURN_ERROR;
	}

	regkey.CheckValue_STR(varname);
DBG "regkey.CheckValue_STR() --> status == ", regkey.status;
	if (regkey.IsNewValue()) {
DBG "NOT DELETING NON-EXISTENT '", varname, "'.";
		return RETURN_SUCCESS;
	}

	if (OPTION('s')) {
DBG "DELETING '", varname, "' from '", envspace.path, "'...";	
		regkey.DelValue(varname);
		return regkey.status == ERROR_SUCCESS ? RETURN_SUCCESS : RETURN_ERROR;
	} else {
DBG "DELETING FROM EXISTING LIST VAR...";
		// Read the original value:
		regkey.GetValue_STR(varname);
		if (regkey.status != ERROR_SUCCESS) {
			cerr << "- ERROR: getting value of '"<<varname<<"' failed! (Err. code: "<<regkey.status<<")"<<endl;
			return RETURN_ERROR;
		}

		value = DelListPart(regkey.current_value, value, LIST_TYPE::PATH);
		
DBG "UPDATING '", varname, "' TO '", value, "' in '", envspace.path, "' AS ",
    (OPTION('x') ? REG_EXPAND_SZ : 0/*use default*/);
		regkey.SetValue_STR(varname, value, 0/*use default*/);
		if (regkey.status == ERROR_SUCCESS) {
			// Forced notification?
			if (OPTION('!')) DoNotify(); //!! result ignored...
			return RETURN_SUCCESS;
		} else {
			cerr << "- ERROR: Updating '"<<varname<<"' to '"<<value<<"' FAILED! (Err. code: "<<regkey.status<<")"<<endl;
			return RETURN_ERROR;
		}
	}	
	
	return RETURN_ERROR;
}


//---------------------------------------------------------------------------
int DoNotify()
{
	// This might take long, so let's wait 5 seconds...
	DWORD result = 0;
	SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM) "Environment", SMTO_ABORTIFHUNG, 5000, &result);
	if (result == ERROR_SUCCESS) {
		return RETURN_SUCCESS;
	} else {
		cerr << "- ERROR: Broadcasting the WM_SETTINGCHANGE message failed! (Err. code: "<<result<<")"<<endl;
		return RETURN_ERROR;
	}
}


//---------------------------------------------------------------------------
int main(int argc, char** argv)
{
DBG "DEBUG MODE ON";

	const char SESSION = 'V';
	const char USER = 'U';
	const char SYSTEM = 'S';

	map<char, regkey> namespaces;
	namespaces[SESSION] = REG_ENV_SESSION;
	namespaces[USER] = REG_ENV_USER;
	namespaces[SYSTEM] = REG_ENV_SYSTEM;

	// Actions & parameters
	string action;	// will be set to "help" as default
	string varname;	// name of the env. variable(s)
	string value;	// value for the variable(s)

	// Options
	set<char> options;
	set<char> selectors;	// will be set to 'V' if none specified

	// Process the cmdline arguments...
	bool got_action = false, got_varname = false, got_value = false; // helper flags
	for (int i = 1; i < argc; i++)
	{
//DBG "Processing argv[", i, "] == '", argv[i], "'...";
		// NOTE: -options are only accepted BEFORE a variable name,
		// in order to allow values with a leading '-' or '/'.
		if (!got_varname
			&& (argv[i][0] == '-' || argv[i][0] == '/')) // option?
		{
			if (!argv[i][1]) {
				cerr << _T("- ERROR: missing option letter.\n");
				continue;
			}
				
			// Process any combined option letters...
			for (int c, n = 1; c = argv[i][n]; n++) {
				// Treat the /? "option" as an action:
				if (c == '?') {
					action = "help";
					got_action = true;
				} else {
					options.insert(argv[i][n]);
//DBG "OPTION '", argv[i][n], "' ADDED!";
				}
				// Go on further for a possible '-e'...

				if (argv[i][n] == 'n') { // name selector?
					n++;
					// Eat up each selector letter:
					while (1) {
						c = argv[i][n];
						if (c == 'V' || c == 'U' || c == 'S') {
							selectors.insert(c);
//DBG "NAMESPACE CHAR '", (char)c, "' ADDED!";
							n++;
						} else {
							n--; // need to re-read c in the outer loop!
							break;
						}
					}
				}
			}
		} else if (!got_action) { // perhaps an action?
			action = argv[i];
			got_action = true;
		} else if (!got_varname) { // perhaps a varname?
			varname = argv[i];
			got_varname = true;
		} else if (!got_value) { // perhaps a value?
			value = argv[i];
			got_value = true;
		}
	}
	
	// Set defaults...
	if (action.empty()) action = "help";
	if (selectors.empty()) selectors.insert('V'); // Session variable

DBG_(action);
//DBG_(options);
//DBG_(selectors);
DBG_(varname);
DBG_(value);

	// OK, do what's been requested...
	if (action == "set") {
		if (varname.empty()) {
			cerr << _T("- ERROR: missing variable name and value for SET.\n");
			return RETURN_ERROR;
		} else if (value.empty()) {
			cerr << _T("- ERROR: missing value for SET.\n");
			return RETURN_ERROR;
		}

		DWORD result = ERROR;
		for (set<char>::iterator n = selectors.begin(); n != selectors.end(); n++) {
			regkey& selected = namespaces[*n];
DBG "Doing SET for '", *n, "'...";
			result = DoSet(selected, varname, value, options);
			if (result != ERROR_SUCCESS) {
				cerr << "- ERROR: failed to change '["<<(unsigned long)selected.rootkey<<"]\\"
					<<selected.path<<"\\"<<varname<<"'"
					<<" (Err. code: "<<result<<")\n";
			}
		}
		return result == ERROR_SUCCESS ? RETURN_SUCCESS : RETURN_ERROR;
		
	} else if (action == "check") {
	
			cerr << _T("- ERROR: 'check' is not implemented yet.\n");
			return RETURN_ERROR;

		if (varname.empty()) {
			cerr << _T("- ERROR: missing variable name for CHECK.\n");
			return RETURN_ERROR;
		} else if (OPTION('x') && value.empty()) {
			cerr << _T("- ERROR: missing value for CHECK.\n");
			return RETURN_ERROR;
		}
	} else if (action == "del") {	
	
		if (varname.empty()) {
			cerr << _T("- ERROR: missing variable name for DEL.\n");
			return RETURN_ERROR;
		} else if (OPTION('x') && value.empty()) {
			cerr << _T("- ERROR: missing value for DEL.\n");
			return RETURN_ERROR;
		}

		DWORD result = ERROR;
		for (set<char>::iterator n = selectors.begin(); n != selectors.end(); n++) {
			regkey& selected = namespaces[*n];
DBG "Doing DEL for '", *n, "'...";
			result = DoDel(selected, varname, value, options);
			if (result != ERROR_SUCCESS) {
				cerr << "- ERROR: failed to change '["<<(unsigned long)selected.rootkey<<"]\\"
					<<selected.path<<"\\"<<varname<<"'"
					<<" (Err. code: "<<result<<")\n";
			}
		}
		return result == ERROR_SUCCESS ? RETURN_SUCCESS : RETURN_ERROR;
		
	} else if (action == "show") {	
	
			cerr << _T("- ERROR: 'show' is not implemented yet.\n");
			return RETURN_ERROR;

	} else if (action == "notify") {	

		return DoNotify();
		
	} else if (action == "help") {	
		if (OPTION('e')) {
			ShowExamples();
		} else {
			Usage();
		}
		return RETURN_SUCCESS;
	}
	else {
		cerr << _T("- ERROR: unknown action. Use /? for help.\n");
		return RETURN_ERROR;
	}

	// Should not arrive here, but the evil never sleeps.
	cerr << _T("- UNKNOWN ERROR. (Use /? for help.)\n");
	return RETURN_ERROR;
}
