regenv

Swiss-army-knife (a smaller one ;) ) for Win32 Registry-based environment variables.
(This is a partly stupidified, parly smartified descendant of SetEnv by Jonathan Wilkes.)
I've been using it everyday and everywhere in my Windows command-line environments.

Copyright (C) 2012-2014 Szabolcs Szasz. Some rights reserved. 
Parts copyright (C) 2005-2008 Jonathan Wilkes (ex: http://www.xanya.net).
See LICENSE.txt for the details.

================================================================================

About

	RegEnv is a command-line tool for setting, changing & deleting 
	System, User and Session ("Volatile") environment variables stored
	in the Windows Registry.
	
	Whenever you launch a console with a shell (and then any program
	from there), its set of environment variables are ultimately (and 
	in slightly cumbersome ways) affected (and sometimes, curiously, 
	_not_ affected) by the ones set permanently (or for the time of a 
	login session) in the Registry.
	
	These variables are beyond the reach of the "SET" shell command,
	which can only see and set variables temporarily, for the current 
	shell and its invoked child processes.

	This tool provides full control to scripts over those variables 
	stored in the registry, that you could otherwise only manipulate
	via Control Panel (and other 3rd-party utilities, which are less
	versatile than RegEnv).

	Why yet another setenv utility? Because I (even in 2012!) still 
	couldn't find a single command-line tool with all the critically 
	important features I needed, and with a (i) well-documented and 
	(ii) script-friendly interface, plus (iii) source code. 
	
	Jonathan Wilkes' SetEnv came closest, so I forked that one.

Features
	
	- Sending (or not sending) WM_SETTINGCHANGE notification to the
	  system (so that newly launched consoles will see the changed env.
	  variables). (This is very useful e.g. for various session init 
	  scripts, but is amazingly missing from many such tools.)
	  
	- Support for "list" ("multi-value") variables (like PATH, with 
	  ';'-separated parts) for adding, checking and deleting parts.
	  (SetEnv could already do this nicely.)
	  
	- Idempotent list-variable updates: "regenv set -l var part" will 
	  only add 'part' to 'var' if it's not yet there.

	- No install ceremony, just a zip. (This tool is for developers 
	  and sysadmins anyway.)

Installation

	Huh?

Usage

	regenv -?
	(or regenv with no args)

Changes
	
	See CHANGES.txt.

License

	See LICENSE.txt.

More

	If you have anything to say, please send an e-mail to "lunakid" at the...
	you know which search provider that runs a nice email service too, and
	is not Yahoo. ;)

	For the original SetEnv tool, Jonathan published an excellent article on 
	CodeProject: http://www.codeproject.com/Articles/12153/SetEnv

	See also: Microsoft's SETX command (http://ss64.com/nt/setx.html), which, 
	while quite versatile, still doesn't support sending the WM_SETTINGCHANGE
	notification.
