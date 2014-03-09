#include <windows.h>
#include "dbg.h"

// This check is probably unsafe (incomplete). E.g. MSVC links fine, 
// but GCC does not, even though it used to do so... Now I get 
// "The procedure entry point RegGetValueA could not be locaed  
// in the dynamic link library ADVAPI32.dll."
// Here RegGetValue is said to be only available on Windows XP 64-bit:
// http://social.msdn.microsoft.com/Forums/vstudio/en-US/910a0e22-42bf-437d-8f5b-0162cc691f89/the-procedure-entry-point-reggetvaluea-could-not-be-located-in-advapi32dll?forum=vcgeneral
// But it's obviously not true in my case, where I seem to have 
// it on my 32-bit XP, when compiling with MSVC. (And with a MinGW
// version, too.)
//#if 1
#if (_WIN32_IE < 0x0602)
#	define NO_REGGETVALUE 1
#else // Win2003 & up (?)
//for Vista: #include <Shlwapi.h>	// SHRegGetValue + SRRF_... consts
#error Not sure I know what platform this is; delete this messsage to proceed!
#endif


//
// For single-threaded use only!
// It's API has STL strings, while internally it uses LP[C]TSTR.
// Methods are neither orthogonal nor idempotent!
//
class StupidRegKey
{
public:
	LONG status; // Windows error code
	
	HKEY  hkey;
	DWORD type;
	DWORD size;
	LPTSTR current_value;

	//-------------------------------------------------------------------
	StupidRegKey(HKEY rootkey = NULL, string path = NULL) : 
		status		(ERROR),
		hkey		(NULL),
		type		(REG_SZ), 
		size		(0),
		current_value	(NULL)
	{
		status = ERROR_SUCCESS;
		if (rootkey) Open(rootkey, path);
	}

	~StupidRegKey() {
		if (hkey) RegCloseKey(hkey);
		if (current_value) delete current_value;
	}

	
	//-------------------------------------------------------------------
	void Open(HKEY rootkey, string path) {
		if (status != ERROR_SUCCESS) return;		
		status = RegOpenKeyEx(rootkey, path.c_str(), 0, KEY_READ|KEY_SET_VALUE, &hkey);
	}

	//-------------------------------------------------------------------
	void CheckValue_STR(string valuename) {
		if (status != ERROR_SUCCESS) return;		

		DWORD saved_type = type;

#ifdef NO_REGGETVALUE
DBG "[NO_REGGETVALUE] TYPE before RegQueryValueEx: ", type;
		status = RegQueryValueEx(hkey, valuename.c_str(), NULL, &type, NULL, &size);
		if (status != ERROR_FILE_NOT_FOUND &&
		    type != REG_SZ && type != REG_EXPAND_SZ) {
			status = ERROR_INVALID_DATATYPE;
		}
DBG "[NO_REGGETVALUE] TYPE after RegQueryValueEx: ", type;
#else
		//!! Only from XP64 and above... :-/
		status = RegGetValue( // http://msdn.microsoft.com/en-us/library/windows/desktop/ms724868(v=vs.85).aspx
			hkey, "", valuename.c_str(), 
			RRF_RT_REG_SZ | RRF_RT_REG_EXPAND_SZ | RRF_NOEXPAND,
			&type, NULL, &size);
		//!!TODO: Reconcile the return value with ERROR_INVALID_DATATYPE above!
#endif
		if (status != ERROR_SUCCESS) {
			type = saved_type; //! Need to reset this, as my WinXP sets it to garbage in case of ERROR_FILE_NOT_FOUND
		}
	}

	//-------------------------------------------------------------------
	bool IsNewValue() { return status == ERROR_FILE_NOT_FOUND; }

	//-------------------------------------------------------------------
	void GetValue_STR(string valuename) {
		if (status != ERROR_SUCCESS) return;		

		current_value = new TCHAR[size + 1]; // http://msdn.microsoft.com/en-us/library/windows/desktop/ms724911(v=vs.85).aspx
#ifdef NO_REGGETVALUE
		status = RegQueryValueEx(hkey, valuename.c_str(), NULL, NULL, (LPBYTE)current_value, &size);
		current_value[size] = '\0'; // http://msdn.microsoft.com/en-us/library/windows/desktop/ms724911(v=vs.85).aspx
				// "even if the function returns ERROR_SUCCESS, the application should 
				// ensure that the string is properly terminated before using it"
#else
		//!! Only from XP64 and above... :-/
		status = RegGetValue( // http://msdn.microsoft.com/en-us/library/windows/desktop/ms724868(v=vs.85).aspx
			hkey, "", valuename.c_str(),
			RRF_RT_REG_SZ | RRF_RT_REG_EXPAND_SZ | RRF_NOEXPAND,
			NULL, (LPBYTE)current_value, &size);
#endif
	}

	//-------------------------------------------------------------------
	void SetValue_STR(string valuename, string value, DWORD new_type = 0) {
		if (status != ERROR_SUCCESS && 
		    status != ERROR_FILE_NOT_FOUND) return;		

DBG "StupidRegKey::SetValue_STR(", valuename, ", ", value, ", ", new_type, ") --> eff. type: ",
    (new_type == 0/*use default*/? type : new_type);
		status = RegSetValueEx(hkey, valuename.c_str(), 0, 
			(new_type == 0/*use default*/? type : new_type),
			(const BYTE *)value.c_str(), (DWORD)value.length() + 1);
	}

	//-------------------------------------------------------------------
	void DelValue(string valuename) {
		if (status != ERROR_SUCCESS) return;
		status = RegDeleteValue(hkey, valuename.c_str());
	}
};
