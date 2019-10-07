REGENV

Tool for manipulating Win32 Registry-based environment variables.

Copyright (C) 2012-2014 Szabolcs Szasz. Some rights reserved. 
Parts copyright (C) 2005-2008 Jonathan Wilkes.
See LICENSE.txt for copyright details.
See CHANGES.txt for version/status.
Master repo: http://github.com/lunakid/regenv

================================================================================

About

	RegEnv is a command-line tool for setting, changing & deleting 
	System, User and Session ("Volatile") environment variables stored
	in the Windows Registry.
	
	Whenever you launch a console (and then any program from there), 
	the environment variables set there are ultimately (and in slightly 
	cumbersome ways) affected (and sometimes, curiously, not affected) 
	by those set permanently (or for the time of a login session) in 
	the Windows Registry.
	
	These variables are beyond the reach of the plain old "SET" shell 
	command, which can only see and set variables temporarily, for the 
	current command-like shell instance (and its child processes).

	This tool provides full control to scripts over those variables
	stored in the registry, which could otherwise only be manipulated
	either by other 3rd-party command-line utilities (which I found 
	less handy than RegEnv, see below), or via Control Panel, manually.

	(Why yet another setenv utility? Because I (even in 2012!) still 
	couldn't find a single command-line tool with (1) all the features 
	that have been important to a CLI-style developer like me, with
	a (2) well-documented and (3) script-friendly interface, plus (4)
	open sources.	
	Jonathan Wilkes' nice SetEnv came closest, so I forked (and gutted)
	that one.)

Features
	
	- Sending (or not sending) WM_SETTINGCHANGE notification to the
	  system (so that newly launched consoles will see the changed env.
	  variables). (This is very useful e.g. for various session init 
	  scripts, but is amazingly missing from many such tools.)
	  
	- Support for Session ("Volatile") variables in the Registry.

	- Handle "list" ("multi-value") variables (like PATH, with parts
	  separated by ';') for adding, checking and deleting parts. IOW:
	  idempotent partial variable updates: "regenv set -s var part" 
	  will only add 'part' to 'var' if it's not yet there.
	  (SetEnv could already do this nicely.)

	- No install, just a single .EXE to deal with. (This is for 
	  developers and sysadmins anyway.)

Installation

	Copy & paste regenv.exe werever you want.

Usage

	See `regenv -?` (or just `regenv`) for help.

Changes
	
	See CHANGES.txt.

License

	See LICENSE.txt.

More

	Please file issues on GitHub, or you can reach me by "lunakid" at the...
	you know which search provider that runs a gigantic email service and
	is not Yahoo or Microsoft. ;)

	RegEnv is a partly stupidified, parly smartified remake of Jonathan Wilkes's 
	SetEnv.	For his original tool, Jonathan published an excellent article on 
	CodeProject: http://www.codeproject.com/Articles/12153/SetEnv.

	See also: Microsoft's SETX command (http://ss64.com/nt/setx.html), which, 
	while quite versatile, still doesn't support sending the WM_SETTINGCHANGE
	notification.
