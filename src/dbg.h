// DBG.HPP 0.154 -- C++ Debug Helper Macros
//
// CONTROL SYMBOLS:
//
//	DBG_OFF (or NDEBUG)   // turn off all the debug scaffolding
//	DBG_TRACE_OFF         // turn off stack tracing completely
//	DBG_TRACE_SILENT      // turn off tracing regular scope transitions
//	                         (only print stack dump on terminate() )
//	DBG_CFG_DEVICE        // DbgOut::DEBUG_CONSOLE, DbgOut::POPUP_MESSAGE,
//	DBG_CFG_TRACEDEVICE      and DbgOut::STD_STREAM (<-- default)
//	DBG_CFG_STREAMPREFIX  // line prefix used when printing to a
//	                         standard stream (default: "DBG> ")
//	DBG_CFG_DBGCONPREFIX  // line prefix used when printing to dbg. console
//	                         (default: "DBG> ")
//	DBG_CFG_MSGBOXPREFIX  // line prefix used when printing to MessageBox
//	                         (default: <none>)
// OPERATIONS:
//
//      DBG x, y, ...;        // free-form debug dump
//      DBGDUMP x, y, ...;    // free-form decorated dump (commas inserted)
//      DBGBOX x, y, ...;     // free-form dump in a message box
//
//      DBG_(anyexpr)         // dump in the form: "DBG> (anyexpr) == (value)
//
//      DBG_throw(x)
//      DBGASSERT(x)          // assert that *may* show the failed expr.
//                               (it can show that if can access the source!)
//      DBGASSERTEX(x)        // this variant can always show the failed expr.
//      DBG_CHECK_CLASS_INVARIANT // calls 'this->class_invariant();'
//      DBGABORT              // "commented" abort
//      DBGMARK               // Outputs an "I was here" trace mark.
//      DBGBEEP               // Just a beep
//
//      DBGTRACE              // Push a scope trace marker
//      DBGTRACE_QUIET        // Turn to silent scope tracing
//      DBGTRACE_LOUD         // Turn to verbose scope tracing
//
//  Obsolete:
//	DBG_P(ptr)            // named pointer dump, see: DBG_()
//	DBG_I(int)            // named int (and char) dump, see: DBG_()
//	DBG_F(float)          // named float or double dump, see: DBG_()
//  Currently broken:
//      DBGLOG DBGLOG_OPEN(const char* logname)
//             DBGLOG(DBGLOG log, const char* fmt, ...)
//             DBGLOG_VARG(DBGLOG log, const char* fmt, va_list args)
//             DBGLOG_CLOSE(DBGLOG log)
//      DBGFILE(const char* filename, const char* fmt, ...)
//
// PLATFORM-SPECIFIC:
//   Win32:
//	DBG_CFG_[TRACE]DEVICE == DbgOut::DEBUG_CONSOLE --> OutputDebugString
//	DBG_CFG_[TRACE]DEVICE == DbgOut::POPUP_MESSAGE --> MessageBox
//	DBGBEEP --> MessageBeep(-1); // (system default beep)
//
/////////////////////////////////////////////////////////////////////////////

/** TODO:
@todo Tell the Doxygen guys that whole todo lists could/should be supported with e.g. a "@todo:" syntax.
@todo Doxygen's CPP_MULTILINE...-autobrief does not seem to do its job.


	Add control option to DBGASSERT: break(debug)/abort/ignore
	(Optinally a GUI question?)
	http://www.cuj.com/documents/s=8248/cujcexp2104alexandr/alexandr.htm
	http://www.cuj.com/documents/s=8464/cujcexp0308alexandr/

	Separate the trace and debug stuff? (Trace depends on plain dbg dumping,
	but debug printing does not depend on Tracing...)

	More stack stuff (and "John Panzer has published in the C/C++ Users
	Journal, January 1999, his essay "Automatic code Instrumentation"):
	http://www.codeproject.com/tips/stackdumper.asp?target=debug%7Cc%2B%2B

	Using the static singleton pattern (for the output stream)
	is not thread-safe!

	Take the excellent ideas (and implementation) from here:
	http://www.codeproject.com/debug/qafdebug.asp?target=debug%7Cc%2B%2B#Intro

	Add proper char/wchar support. (str("") for clearing the output buffer
	is not OK!) A good resource is:
	http://www.codeproject.com/debug/debugout.asp?target=debug%7Cc%2B%2B

	CLEAN UP the remaining old stuff. As part of that, consider:
	Extra code remains for non-debug if DBGLOG_OPEN is used!...
	Should I cease support for keeping a log open? Performance
	would be horrible for mass logging then (open/close pairs
	for every item)! Or should I cease support log file creation
	by these macros? (Existing streams could be passed to the
	debug output handler anyway.)
	Besides, debug outputting may be a completely different job
	from managing log files, after all, so I might drop log support
	altogether.


HISTORY:

0.154 (2003-11-07):

	Added debug dump and stack dump facilities and an advanced
	asserter class.

0.15 (2003-11-04):

	First fairly usable C++ version for multiple compilers and
	configurations. Basic test suite added (see dbg.h.test/...).

0.13pre (2003-10-14):

0.10 (2003-10-04):

	Forked from DBG.H 3.05/7 with lots of small changes.
*/


#ifndef _DBG_H_
#define _DBG_H_

#pragma warning( disable : 4100 ) // VC++: unreferenced formal parameter
#pragma warning( disable : 4127 ) // VC++: conditional expression is constant

//!!namespace dbg {

// ==========================================================================
//
//                            * Things common for DEBUG ON/OFF *
//
// ==========================================================================
//struct X_Invariant_Violation { X_Invariant_Violation(const char* expr); };
//struct DBG_X_ASSERT {};


#if defined(NDEBUG) && !defined(DBG_OFF)
#define DBG_OFF
#endif

#if defined(DBG_OFF)
// ==========================================================================
//
//                             * DEBUG OFF *
//
// ==========================================================================

#define DBGASSERT         if (false) Asserter(__FILE__, __LINE__, __FUNCTION__).verify
#define DBGASSERTEX(x)    {0;}
#define DBG_CHECK_CLASS_INVARIANT {0;}
#define DBG_throw(x)      throw x
#define DBGABORT          {0;}

#define DBG               (void)0,
#define DBGDUMP           (void)0,
#define DBGBOX            (void)0,
#define DBG_(x)           {0;}
#define DBG_I(x)          {0;}
#define DBG_F(x)          {0;}

#define DBGTRACE          {0;}
#define DBGTRACE_QUIET    {0;}
#define DBGTRACE_LOUD     {0;}

#define DBGMARK           {0;}
#define DBGBEEP           {0;}

#define DBGOUTPUT void*
#define DBGLOG void*
/*
static void DBGSETOUTPUT(DBGOUTPUT) {}
static void DBGTO(DBGOUTPUT, const char*, ...) {}
static void DBGLOG_WRITE(DBGLOG, const char*, ...) {}
static DBGLOG DBGLOG_OPEN(const char*) {return 0;}
static void DBGLOG_CLOSE(DBGLOG) {}
static void DBGFILE(const char*, const char*, ...) {}
*/

// ==========================================================================
//
#else //                        * DEBUG ON *
//
// ==========================================================================


#if !defined(DBG_CFG_NO_WIN32) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__))
#define DBG_HAS_WIN32__
#endif

// Include windows.h if
//	- we are on Win32 (and Win32 support not disabled)
//	- windows.h not yet included
#if defined(DBG_HAS_WIN32__) && !defined(WINAPI)
// Note: setting the following two are pretty rude, but so is not including
// windows.h from the application!... ;-)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#pragma comment(lib, "user32")  // help with using the Win32-specific features
#endif

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <stdexcept> // uncaught_exception
#include <vector>    // for stack trace & supporting 'DBGDUMP vector'
#include <string>    // for stack trace & supporting 'DBGDUMP string'
#define DBG_STD__ std
using DBG_STD__::ostream;
using DBG_STD__::stringstream;
using DBG_STD__::ifstream;
using DBG_STD__::string;
using DBG_STD__::vector;

#ifndef DBG_CFG_STREAMPREFIX
#define DBG_CFG_STREAMPREFIX  "DBG> "
#endif

#ifndef DBG_CFG_DBGCONPREFIX
#define DBG_CFG_DBGCONPREFIX  "DBG> "
#endif

#ifndef DBG_CFG_MSGBOXPREFIX
//#define DBG_CFG_MSGBOXPREFIX  ""
#endif

#ifndef DBG_CFG_DEVICE
#define DBG_CFG_DEVICE           DbgOut::STD_STREAM
//#define DBG_CFG_DEVICE           DbgOut::DEBUG_CONSOLE
#endif

#ifndef DBG_CFG_TRACEDEVICE
#define DBG_CFG_TRACEDEVICE      DbgOut::STD_STREAM
//#define DBG_CFG_TRACEDEVICE      DbgOut::DEBUG_CONSOLE
#endif

#ifndef DBG_CFG_OUTSTREAM
#define DBG_CFG_OUTSTREAM        DBG_STD__::cout
#endif

#ifndef DBG_CFG_TRACESTREAM
#define DBG_CFG_TRACESTREAM      DBG_STD__::cout
#endif

//===========================================================================
#ifndef DBG_TRACE_OFF
#  ifdef DBG_TRACE_SILENT
#    define DBGTRACE DbgScope scopetracer__(__FUNCTION__, __LINE__, DbgScope::TRACE_ONLY);
#  else
#    define DBGTRACE DbgScope scopetracer__(__FUNCTION__, __LINE__, DbgScope::PRINT);
#  endif
#  define DBGTRACE_QUIET { DbgTracer::instance().off(); }
#  define DBGTRACE_LOUD  { DbgTracer::instance().on(); }
#else
#  define DBGTRACE       {0;}
#  define DBGTRACE_QUIET {0;}
#  define DBGTRACE_LOUD  {0;}
#endif

#define DBG_throw(x)	{ \
		DbgOut::throwtrace(#x, __FUNCTION__, __FILE__, __LINE__);	\
		DbgTracer::trigger_stack_dump();	\
		throw x; }

#define DBGASSERT Asserter(__FILE__, __LINE__, __FUNCTION__).verify

#define DBGASSERTEX(x) { if (!(x)) {	\
		DbgOut::raw_line() << "*** ASSERTION FAILED: " << #x;	\
		DbgOut::raw_line() << "  at: " << __FUNCTION__	\
			<< " (" << __FILE__ << " line: " << __LINE__ << ")";	\
		DbgTracer::trigger_stack_dump();	\
		throw "DBG_X_ASSERT(!!...)"; } }

#define DBG_CHECK_CLASS_INVARIANT DBGASSERT(class_invariant())

#define DBGABORT	{ DBG "*** ABORTING... (at ", __FUNCTION__, ": ", __LINE__, ", ", __FILE__, ")"; \
			DbgTracer::instance().dump_call_stack(); /* Note: trigger_stack_dump is not enough for abort()! */	\
			abort(); }

#define DBG	DbgOut::raw_line() <<
#define DBGBOX	DbgOut::raw_line(DbgOut::FlushDbgLine(DbgOut::out(), DbgOut::POPUP_MESSAGE, DbgOut::dbgstream())) <<

#define DBGDUMP DbgOut::dumper_line(__FUNCTION__, __LINE__) <<

#define DBG_(x)		DbgOut::dumpval(#x, x);

#define DBG_I(x)	if (sizeof(x)==1)	\
				DBG "(",#x,")", "\t== ", "'",x,"'", " [",DBG_STD__::hex,DBG_STD__::showbase,(int)x,DBG_STD__::dec,"]";	\
			else	\
				DBG "(",#x,")", "\t== ", "(",x,")", " [",DBG_STD__::hex,DBG_STD__::showbase,x,DBG_STD__::dec,"]";
/// @todo Uses the old C technique, clean it up!
#define DBG_F(x)	{ char buf[60]; __FORMAT_FLOAT_(buf,x);	\
			  DBGPRINT("%s: %s  (%s, %d)",	\
					#x, buf, __FILE__, __LINE__); }

#define DBGMARK { DBG "--> " << __FUNCTION__ << " (line: " << __LINE__ << ")"; }

#ifdef DBG_HAS_WIN32__
#define DBGBEEP {::MessageBeep((UINT)-1);}
#else
#define DBGBEEP {DBG_STD__::cerr << '\a';}
#endif

//===========================================================================
//	UTILITIES
//===========================================================================
inline string trim(const string& s)
{
	if (s.empty()) return s;
	size_t beg = s.find_first_not_of(" \t");
	if (beg == -1) return string("");
	size_t end = s.find_last_not_of(" \t");
	return string(s, beg, end - beg + 1);
}

// Try reading the failed ASSERT statement from the source file...
// Well, this is, of course, meaningful only when testing locally.
// But that DOES tend to happen, doesn't it?
inline string fetch_assert_line(const char* filename, unsigned line)
{
	string assertline("");
	ifstream source(filename);

	for (unsigned l = 0; source && l < line; ++l)
		getline(source, assertline);

	return trim(assertline);
}

//===========================================================================
/// The core output manager.
/// All debug output goes through a static singleton instance of this class.
//===========================================================================
struct DbgOut
{
	enum DeviceType { STD_STREAM, POPUP_MESSAGE, DEBUG_CONSOLE };

	DeviceType   device_;
	DeviceType   tracedevice_;
	// Concrete stream devices for the device_ == STD_STREAM case:
	ostream*     out_;      // normal diagnostic output
	ostream*     traceout_; // trace-info output
	stringstream outbuf_;   // buffer for normal output
	stringstream tracebuf_; // buffer for trace output

	// These must be "forward-defined" to workaround a DMC bug:
	static DeviceType    devicetype()  { return  instance().device_; }
	static DeviceType    tracedevice() { return  instance().tracedevice_; }
	static stringstream& out()         { return  instance().outbuf_; }
	static stringstream& traceout()    { return  instance().tracebuf_; }
	static ostream&      dbgstream()   { return  *(instance().out_); }
	static ostream&      tracestream() { return  *(instance().traceout_); }

	// Line-buffering provides a single point (the dtors) for output
	// control, so it can be easily turned on/off or redirected to
	// various output devices (e.g. MessageBox, OutputDebugString, ...).
	struct FlushLine
	{
		stringstream& linebuf_;
		DeviceType    device_;
		ostream&      outstream_;

		FlushLine(stringstream& linebuf = DbgOut::out(),
			DeviceType device = DbgOut::devicetype(),
			ostream& outstream = DbgOut::dbgstream())
		: linebuf_(linebuf), device_(device), outstream_(outstream)
		{
		}
		void flush();
	private:
		FlushLine& operator=(FlushLine&); // silence warning: "cannot generate op=..."
	};
	struct FlushDbgLine : FlushLine
	{
		FlushDbgLine(stringstream& linebuf = DbgOut::out(),
			DeviceType device = DbgOut::devicetype(),
			ostream& outstream = DbgOut::dbgstream())
		: FlushLine(linebuf, device, outstream)
		{
		}
		~FlushDbgLine();
	};
	struct FlushTraceLine : FlushLine
	{
		FlushTraceLine(stringstream& linebuf = DbgOut::out(),
			DeviceType device = DbgOut::devicetype(),
			ostream& outstream = DbgOut::dbgstream())
		: FlushLine(linebuf, device, outstream)
		{
		}
		~FlushTraceLine();
	};

	DbgOut(ostream& out, ostream& trace_out, DeviceType device, DeviceType tracedevice) :
		out_(&out),
		traceout_(&trace_out),
		device_(device),
		tracedevice_(tracedevice)
	{
	}
	DbgOut()
	{
	}
	static DbgOut& instance()
	{
		// Static singleton instance
		static DbgOut s_instance(DBG_CFG_OUTSTREAM, DBG_CFG_TRACESTREAM, DBG_CFG_DEVICE, DBG_CFG_TRACEDEVICE);
		return s_instance;
	}

	static ostream& raw_line( const FlushDbgLine& cl = FlushDbgLine(out(), devicetype(), dbgstream()) )
	{
		return out();
	}
	static DbgOut& dumper_line(const char* func, int line, const char* file = "",
	                                  const FlushDbgLine& cl = FlushDbgLine(out(), devicetype(), dbgstream()) )
	{
		out() << '[' << func << ":" << line << "]: ";
		return instance();
	}
	static ostream& trace_line( const char* func = "", int line = 0, const char* file = "",
	                            const FlushTraceLine& cl = FlushTraceLine(traceout(), tracedevice(), tracestream()) )
	{
		traceout() << "--> ";
		return traceout();
	}

	static void throwtrace(const char* expr, const char* func, const char* file, int line)
	{
		raw_line() << "*** EXCEPTION: " << expr;
		raw_line() << "  at: " << func << " (" << file << ", line: " << line << ")";
	}

	/// Type-specific formatted output of expressions
	/// @todo consolidate typed dumping with DbgOut& operator<<()
	template<typename T> static void dumpval(const char* expr, T x)
	{
		raw_line() << "",expr,"", "\t== ", "(",x,")", " [",DBG_STD__::hex,DBG_STD__::showbase,x,DBG_STD__::dec,"]";
	}

private:
	DbgOut(const DbgOut&);
	DbgOut& operator=(const DbgOut&);
};

// DbgOut template specializations:
// (DMC 8.3x is slightly template-challenged...)
#ifndef __DMC__
template<> inline void DbgOut::dumpval(const char* expr, bool x)
{
	raw_line() << "" << expr << "" << "\t== " << "(" << (x?"true":"false") << ")"
		   << " [" << x << "]";
}
template<> inline void DbgOut::dumpval(const char* expr, char x)
{
	raw_line() << "" << expr << "" << "\t== " << "'" << x << "'"
		   << " [" << DBG_STD__::hex<<DBG_STD__::showbase<< (int)x <<DBG_STD__::dec<<"]";
}
#endif


//===========================================================================
/// op<< and op, pairs for printing via the Output Manager (see above)
///
///	- op<< is basically the usual << output with some extra
///	  display-decoration (depending on the type)
///
///	- op, prints a comma before printing the object
///
//===========================================================================
/// Comman op. doing raw (undecorated) printing like the original << of the type.
template <class T> inline ostream& operator,(ostream& o, T x)  { return o << x; }

/// Generic comma op / << op for decorated printing.
template <class T> inline DbgOut& operator<<(DbgOut& d, T x) { DbgOut::out() << x; return d; }
template <class T> inline DbgOut& operator,(DbgOut& d, T x)  { DbgOut::out() << ", "; return d << x; }

#if defined(_MSC_VER)
// VC++ does not prepend 0x to pointer values by default, so let's do it
// ourselves. ("<< ios_base::showbase" would be nicer, but would require
// saving/restoring the ios state...)
template <class T> inline DbgOut& operator<<(DbgOut& d, T* x)
	{ if (x) DbgOut::out() /*<<ios_base::showbase*/ << "0x" << x; else DbgOut::out() << "NULL";
	  return d; }
#else
template <class T> inline DbgOut& operator<<(DbgOut& d, T* x)
	{ if (x) DbgOut::out() << x; else DbgOut::out() << "NULL";
	  return d; }
#endif
// (Note: Digital Mars C++ seems to be unable to find this one.)
template<> inline DbgOut& operator<<(DbgOut& d, const char* x)
	{ if (x) DbgOut::out() << '\"' << x << '\"'; else DbgOut::out() << "NULL";
	  return d; }

#if defined(_MSC_VER) && _MSC_VER > 1300
template<> inline DbgOut& operator<<(DbgOut& d, char x) { DbgOut::out() << '\'' << x << '\'' <<"(#"<<int(x)<<")"; return d; }
#endif

/// Dumping std::string...
template<> inline DbgOut& operator<<(DbgOut& d, string x) { DbgOut::out() << '\"' << x << '\"'; return d; }

/// Dumping std::vector...
template<> inline DbgOut& operator<<(DbgOut& d, vector<int> x)	{
	DbgOut::out() << "<";
	for (size_t i=0; i<x.size(); ++i) {
		DbgOut::out() << x[i];
		if (i<x.size()-1) DbgOut::out() << ", ";
	}
	DbgOut::out() << ">";
	return d;
}


struct DbgScope;
struct DbgScopeInfo { const char* func; int line; };

//===========================================================================
/// The scope stack tracer
struct DbgTracer
{
	DbgTracer()
	{
		trace_quietly_ = false;
		dumping_ = false;
		nesting_level_ = 0;
		indent[0] = '\0';
		call_stack.reserve(MAXNESTLEVEL);

		DBG_STD__::set_terminate(&dbg_terminate_handler);
	}
	~DbgTracer()
	{
		if (dumping_) dump_call_stack();
	}
	static DbgTracer& instance()
	{
		static DbgTracer s_instance; // static singleton instance
		return s_instance;
	}

	void enter_scope(const DbgScope& scope);
	void leave_scope(const DbgScope& scope);
	static void trigger_stack_dump()
	{
		instance().dumping_ = true;
//!!		instance().dump_call_stack();
	}

	void dump_call_stack();

	// Run-time output control...
	void off() { trace_quietly_ = true; }
	void on()  { trace_quietly_ = false; }
	bool quiet() { return trace_quietly_; }

	// State...
	static const int MAXNESTLEVEL = 50;
	bool trace_quietly_;
	bool dumping_;
	int  nesting_level_;
	char indent[MAXNESTLEVEL];
	vector<DbgScopeInfo> call_stack;

private:
	/// The "global" terminate-handler to catch unhandled exceptions etc.
	/// (Must be a member function to allow cross-module linking. :-/ )
	static void dbg_terminate_handler();
};

struct DbgScope
{
	enum TraceMode {TRACE_ONLY, PRINT};
	TraceMode mode;
	DbgScopeInfo info;

	DbgScope(const char* f, int l, TraceMode m = TRACE_ONLY) {
		mode = m;
		info.func = f; info.line = l;
		DbgTracer::instance().enter_scope(*this);
	}
	~DbgScope() {
		DbgTracer::instance().leave_scope(*this);
	}
};

//------------------------
inline DbgOut::FlushDbgLine::~FlushDbgLine()
{
//!!	if (DbgOut::instance().quiet())
//!!		linebuf_.str("");
//!!	else
		flush();
}

// This one needs to be moved after DbgTracer in order to compile...
inline DbgOut::FlushTraceLine::~FlushTraceLine()
{
	if (DbgTracer::instance().quiet())
		linebuf_.str("");
	else
		flush();
}

inline void DbgOut::FlushLine::flush()
{
	linebuf_ << DBG_STD__::endl;

	string cooked_line;
	switch (device_)
	{
	case POPUP_MESSAGE:
		#ifdef DBG_HAS_WIN32__
		#ifdef DBG_CFG_MSGBOXPREFIX
		cooked_line += DBG_CFG_MSGBOXPREFIX;
		#endif
		cooked_line += linebuf_.str();
		::MessageBox(0, cooked_line.c_str(), "DEBUG MESSAGE",
				MB_ICONASTERISK & MB_ICONINFORMATION);
		break;
		#endif
		; // Fall back to a debug console if no popup support...

	case DEBUG_CONSOLE:
		#ifdef DBG_HAS_WIN32__
		#ifdef DBG_CFG_DBGCONPREFIX
		cooked_line += DBG_CFG_DBGCONPREFIX;
		#endif
		cooked_line += linebuf_.str();
		::OutputDebugString(cooked_line.c_str());
		break;
		#endif
		; // Fall back to std. stream output if not supported...

	default:
	case STD_STREAM:
		outstream_ << DBG_CFG_STREAMPREFIX << linebuf_.str();
		break;
	}

	linebuf_.str(""); // clear the line buffer
}
//---------------------------------------------------------------------------
// Note: relying solely on the destructors doing the dump-related work
// simply will not work, because the program may be abort()-ing, when
// no dtors would get called at all.

inline void DbgTracer::enter_scope(const DbgScope& scope)
{
	call_stack.push_back(scope.info);
	if (scope.mode == DbgScope::PRINT) DbgOut::trace_line() << indent << "{ " << scope.info.func << " (line: " << scope.info.line << ")";
	if (nesting_level_ < MAXNESTLEVEL) ++nesting_level_;
	indent[nesting_level_-1] = '.'; indent[nesting_level_] = '\0';
}
inline void DbgTracer::leave_scope(const DbgScope& scope)
{
	if (nesting_level_ > 0) --nesting_level_;
	indent[nesting_level_] = '\0';
	if (scope.mode == DbgScope::PRINT) DbgOut::trace_line() << indent << "} " << scope.info.func;
	call_stack.pop_back();
}
inline void DbgTracer::dump_call_stack()
{
#ifndef DBG_TRACE_OFF
	DbgTracer& tr = DbgTracer::instance();
	DbgOut::raw_line() << "  call stack:";
	size_t i = tr.call_stack.size();
	if (i == 0) {
		DbgOut::raw_line() << "  -- (empty)";
		return;
	}
	do {
		--i;
		DbgOut::raw_line()
			<< "  -- "
			<< tr.call_stack[i].func
			<< " (line "
			<< tr.call_stack[i].line
			<< ")";
	} while (i != 0);

	dumping_ = false; // done (prevent the destructor dump again)
#endif
}

inline void DbgTracer::dbg_terminate_handler()
{
	//
	// NOTE: The DbgOut instance IS REQUIRED to be alive here!...
	// (See the namespace {} hack below that ensures(?) that.)
	//
	// @todo	Would be nice to check it somehow, but that's not
	// 	easy (impossible?) from a header-only implementation.
	//

	DbgTracer::instance().dump_call_stack();
	DBG "*** TERMINATE...";
	exit(-1);
}

//===========================================================================
struct Asserter
{
	const char* file_;
	unsigned    line_;
	const char* func_;

	Asserter(const char* file, unsigned line, const char* func) :
		file_(file),
		line_(line),
		func_(func)
	{}

	void verify(bool valid, const char* comment = 0)
	{
		if (valid) return;

		string assertline = fetch_assert_line(file_, line_);
		DbgOut::raw_line();
		DbgOut::raw_line() << "*** ASSERTION FAILED ***";
		DbgOut::raw_line() << "  --> " << func_ << " ("<< file_ << ", line: " << line_ << ")";
		if (comment) {
			DbgOut::raw_line() << "  --> " << (assertline.empty() ? comment : assertline);
		} else if (!assertline.empty()) {
			DbgOut::raw_line() << "  --> " << assertline;
		}

		DbgTracer::trigger_stack_dump();
		throw "DBG_X_ASSERT(!!...)";
	}
};
//===========================================================================


//===========================================================================
// Ensure proper construction/destruction order of the singleton objects...
namespace {
	DbgOut& dbgprinter = DbgOut::instance();
	DbgTracer& tracer  = DbgTracer::instance();
}
//===========================================================================

//!! OBSOLETE stuff; should be reworked/removed!

// ---------------------------------- Internal utilities...

	#define __FORMAT_FLOAT_(_buf, x) (( sizeof(x)==sizeof(float)  ? sprintf(_buf, "%f", (x))	\
					  : sizeof(x)==sizeof(double) ? sprintf(_buf, "%g", (x))	\
					  :                             sprintf(_buf, "(DBG: unknown float type)")), buf)
// ----------------------------------

#if 0
  #include <cstdio>
  #include <stdarg.h>
  //
  // These log functions use an explicitly created/selected output.
  //
  typedef FILE* DBGLOG;	// If changing this, the functions need reworking!

  static void DBGLOG_WRITE(DBGLOG log, const char* fmt, ...)
  {
	va_list args; va_start(args, fmt);
	__DBG_WRITE_VARG(log, fmt, args);
  }

  static DBGLOG DBGLOG_OPEN(const char* logname)
  {
        FILE* log = fopen(logname, "a+");
	if (!log) DBGOUT("DBG: Failed to open log output, using OutputDebugString!");
        DBGLOG_WRITE(log, "============================= --- BEGIN LOG --- =============================");
        return log;
  }

  static void DBGLOG_CLOSE(DBGLOG log)
  {
        DBGLOG_WRITE(log, "============================== --- END LOG --- ==============================");
        fclose(log);
	// Let's be nice...:
	if (log == __dbg_output) {
		DBGOUT("DBG: Closing default output, switching to OutputDebugString!");
		DBGSETOUTPUT(0);	// 0 means OutputDebugString
	}
  }

  static void DBGFILE(const char* filename, const char* fmt, ...)
  {
        DBGLOG log;
        va_list args; va_start(args, fmt);

        log = DBGLOG_OPEN(filename);
        __DBG_WRITE_VARG(log, fmt, args);
        DBGLOG_CLOSE(log);
  }
#endif


// ==========================================================================
//
//                                 * END *
//
// ==========================================================================
#endif

// Need to define it here, after DBG has been defined...
//inline X_Invariant_Violation::X_Invariant_Violation(const char* expr)
//	{ DBG "*** Invariant violation (Internal Error: ", (expr ? expr : "UNKNOWN"), ")! ***"; }

/* @todo Win32 specific:
 // Dump ITEMIDIST...
    printf("PIDL: %p\n", cfg.Root);
    printf("cb  : %04X\n", cfg.Root->mkid.cb);
    printf("bin0: %04X\n", (BYTE)cfg.Root->mkid.abID[0]);
    printf("bin1: %04X\n", (BYTE)cfg.Root->mkid.abID[1]);
*/

//!!} // namespace


#endif //_DBG_H_
