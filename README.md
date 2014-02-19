regenv

Swiss-army-knife for Win32 Registry-based environment variables

Copyright (C) 2012-2014 Szabolcs Szasz. Some rights reserved. 
Parts copyright (C) 2005-2008 Jonathan Wilkes (http://www.xanya.net).
See LICENSE.txt for the details.

================================================================================

About

	RegEnv is a command-line tool for setting/updating/deleting System, 
	User and Session environment variables in the Windows Registry.

	The reason why yet another setenv tool was created it that I still
	(in 2012!) could not find a single command-line tool with all the
	critically important features (a) well documented, and (b) in a 
	script-friendly interface.
	
	Key features:
	
	- Sending (or not sending) the WM_SETTINGCHANGE notification to 
	  the system (so that newly launched commands will see the changed
	  env. variables).
	  
	- Support for "list" variables (like PATH, with ';'-separated
	  subvalues) for adding, checking and deleting subvalues.
	  
	- Idempotent list-variable updates: "regenv set -l var subval" 
	  will only add 'subval' to 'var' if it's not yet there.
	
	started processes will actually see the updated variables.

Installation

	Just unpack the downloaded .zip to a directory of your choice 
	(practically to one that's on the PATH), but if you are reading this, 
	I guess, you have already figured it out...)

Usage

	For all the up-to-date usage details, type:

		regenv -?

	Or, see the XANYA website for more information: 
	http://www.xanya.net/site/utils/setenv.php

Change History
	
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

License and Terms of Use

	See LICENSE.txt for the licensing details.

Contact

	If you have anything to say, please send an e-mail to lunakid@gmail.com.
