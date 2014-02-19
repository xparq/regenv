regenv

Swiss-army-knife (a smaller one ;) ) for Win32 Registry-based environment variables.
(This is a partly stupidified, parly smartified descendant of SetEnv by Jonathan Wilkes.)
I use it everywhere in my Windows shell environments.

Copyright (C) 2012-2014 Szabolcs Szasz. Some rights reserved. 
Parts copyright (C) 2005-2008 Jonathan Wilkes (ex: http://www.xanya.net).
See LICENSE.txt for the details.

================================================================================

About

	RegEnv is a command-line tool for setting/updating/deleting System,
	User and Session environment variables in the Windows Registry.

	Why yet another setenv tool? Because I in 2012(!) I still couldn't 
	find a single command-line tool with all the critically important 
	features I needed, and with a well-documented and script-friendly 
	interface. (Jonathan Wilkes' SetEnv came closest, so I chose to 
	rewrite that one.)
	
	Key features:
	
	- Sending (or not sending) WM_SETTINGCHANGE notification to the
	  system (so that newly launched commands will see the changed
	  env. variables). (This one was amazingly missing from many such
	  tools.)
	  
	- Support for "list" ("multi-value") variables (like PATH, with 
	  ';'-separated parts) for adding, checking and deleting parts.
	  (SetEnv could already do this nicely.)
	  
	- Idempotent list-variable updates: "regenv set -l var part" 
	  will only add 'part' to 'var' if it's not yet there.

	- No install ceremony: just a zip. (This tool is, by its nature, 
	  for developers and sysadmins only.)

Installation

	Huh?

Usage

	regenv -? (or regenv with no args)

Changes
	
	See CHANGES.txt.

	The old SetEnv change history:

	1.09 [Fix] - (Feb  9, 2008) - Fixed a problem on Windows 98 where it sometimes failed to open the Autoexec.bat file.
	1.08 [New] - (May 31, 2007) - Added how to delete a USER environment variable to the usage information.
	1.07 [Fix] - (Jan 25, 2007) - Fixed a bug found by depaolim.
	1.06 [New] - (Jan 14, 2007) - Added dynamic expansion support (same as using ~ with setx)
		- Originally requested by Andre Amaral, further Request by Synetech
	1.05 [New] - (Sep 06, 2006) - Added support to prepend (rather than append) a value to an expanded string 
		- Requested by Masuia
	1.04 [New] - (May 30, 2006) - Added support for User environment variables.
	1.03 [Fix] - (Apr 20, 2006) - Bug fix in ProcessWinXP() discovered by attiasr 
	1.01 [Fix] - (Nov 15, 2005) - Bug fix in IsWinME() discovered by frankd 
	1.00 [New] - (Oct 29, 2005) - Initial Public Release.

License

	See LICENSE.txt for the licensing details.

More

	If you have anything to say, please send an e-mail to lunakid@gmail.com.

	For the original SetEnv tool, Jonathan published an excellent article on 
	CodeProject: http://www.codeproject.com/Articles/12153/SetEnv
