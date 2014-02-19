Project Tools - User's Guide
============================
(Last updated for v0.1.)


INTRODUCTION
------------ 

This is a small, simple, unobtrusive, self-containing, flexible and 
extensible project "scaffolding" framework for filesystem-based development 
work. And it does not have a proper name yet...

It provides basic built-in support for baselining and release (or archive)
packaging, and a simple, formalized way to describe mostly any filesystem-
based project, via environment variables, to interface any tools that can 
work with them.

- No install (or upgrade), just copy the thing into your project dir.

- No mandatory config, just use the defaults (saved to PROJECT.ini).
  If you want more, change the default settings to you needs.

- Existing project layouts can be adapted to it in seconds.

- You can host and move your project across Windows (XP or one with a 
  compatible CMD shell and WSH/JScript (cscript.exe))[!!TODO:, any Linux 
  (with sh), or switch to (vanilla) PHP5 (perhaps even 4?)], and it will 
  work basically the same.

- You can add your favorite tools without modifying the PATH variable or 
  you can set your own env. vars in PROJECT.ini in a platform-independent 
  way, instead of maintaining of all kinds multiple init scripts depending 
  on host systems etc.


TERMINOLOGY
-----------

"project command" (or "prj command"):

	The task name given to the prj command, like in:
		
		prj env

	where "env" is the project command (which prints the project 
	environment).

"shortcut command" (or "shortcut" or "proxy command"):

	A short "redirect" command script that launches a project command, 
	which usually have the same name and takes the same arguments as 
	the shortcut, but is located at a different location, which is not 
	on the PATH (or is not directly accesible for some other reason).

"command session" (or "command frame"):

	The process created for running a project command.

	Note: this may be most important for the "shell" command, but
	it's worth noting that any other commands (including custom ones) 
	are executed within identically created command frames.

"project dir" (or "project root"):

	The full path of the filesystem folder where the core work files of 
	a project are located.

	Note: this does not necessarily mean every "project asset": e.g. 
	a project database can be somewhere else (even on a different 
	server).

"project metadata dir":

	The folder where supplementary housekeeping data is stored 
	(recorded) about the project. (Notably change/version tracking etc.)

	(If this dir is missing, the core project material itself will remain 
	intact, but the services provided by this tool will most likely 
	suffer.)

"project environment":

	Parameters that are set for a command session either by the user 
	and/or by the system, telling important things about the structure 
	of the project (e.g. its filesystem layout etc.) to the internal 
	components of the project tools, and also the user's own custom 
	tools, usually via "environment variables" of the host OS.

	Note: managing (setting up, communicating etc.) the project 
	environment is the primary goal of this tool.


INSTALLING
----------

Just copy the .prj-tools dir to your project (root) dir, and, optionally also 
the shortcut command "prj" (and possibly also the others, like "save") for 
convenience.

On the first run of the "prj" command, it will check the environment and 
create a default PROJECT.ini in the project root. Edit and move it as needed.
(NOTE: wherever you move it (under the project root), the tool will find it 
and use that directory for storing other project metadata there, too.)


Shortcut (proxy) commands
-------------------------

In the project root dir, you just invoke the "prj" shortcut via the command-
line (or a browser), with its first argument being the project command to run.
(Any further arguments are passed on to that command.)

Note: while the shortcut commands are optional, there is a good reason 
to still copy and use them: the location of the built-in project commands 
(.prj-tools/{your platform}) is normally not on the PATH, and you probably 
would not want to put it there either.

You are free to delete, rename, change or add any shortcuts; the ones
in this package are just some defaults for convenience. The system does
not depend on them in any way.


Platform extension tools
------------------------

Well, not every platform is built equal. Some are better, and one is 
especially devastating ;) for filesystem-based development.

To add the extra command-line tools you miss on the platform you have to 
work on, and also to help you make your project even more portable and 
self-containing, there is a "tools" subdir under .prj-tools, which is put 
on the PATH during any project command session (notably "prj shell").

This stuff comes pre-packaged with InfoZip's zip.exe and unzip.exe, plus 
tar, gzip and bzip2 for Windows, to help cross-platform compatibility, 
saving you from downloading and properly placing them on your Windows 
filesystem.

You can put whatever commands, utilities, development tools there (or 
just their shortcuts) if you need them portable, always at hand, need 
other versions of installed tools, have no permission for installing 
into the host system, or you just want to put them under the version 
control of the project etc.

The price of this is size: even though the prj toolset itself is ridiculously
small (just a bunch of shell scripts, under 100K total on Windows), any 
tools there would be packed up with the rest of the project files.

Note:	The PROJECT.ini options PROJECT_PATH_PREPEND_DIR and 
	PROJECT_PATH_APPEND_DIR can also be used for similar 
	purposes, but the directories given for them are not 
	explicitly considered part of the project (tools), even 
	if they happen to point somewhere into __PROJECT_ROOT.


Upgrade:
--------

For upgrading, just delete .prj-tools/* from your project dir, and copy the
new version there.


CONFIGURATION
-------------

All the configuration is done via the PROJECT.ini file, which contains env.
variables describing the project environment.

This file is automatically created (in the project root folder) with default
settings, when calling the "prj" command for the first time.

If the PROJECT.ini is later moved to a subdir (which is highly recommended, 
e.g. to one named ".prj"), it will also be found there by the project 
commands. Also, that folder will be assumed to be the project metadata 
directory, containing the files belonging to the project metadata.

To save the project environment to an(other) .ini file any time, use the 
"env save [inifile]" command.


WARNING:
	1. Variable names cannot contain whitespace.
	2. Values cannot contain the '=' sign.
	3. Whitespace is trimmed around the '=' sign.
	4. Surrounding whitespace is stripped both from variable
	   names and values.
	5. Unfortunately, escaping special chars is still system-
	   dependent. :(

About parameter names:
---------------------

- PROJECT_... names are set by the user. (All optionally.) 

  They are all prefixed with "PROJECT_" because they will become 
  environment variables and this way it's easier to separate from the
  others and handle them all as a set.

	NOTE: yes, this might conflict with other systems, which
	use this same naming scheme, but we'll find a way around it
	(like custom prefixing) when that really happens.

- __PROJECT_... names are set by the system, and valid within a command
	frame (i.e. throughout the execution of a "project command", 
	including e.g. "prj shell").

- ..._ROOT variables are full paths.

- ..._SUBDIR variables are dir paths relative to the __PROJECT_ROOT.

- ..._DIR variables are either dir paths relative to __PROJECT_ROOT,
	or absolute dir paths.

- ..._FILE variables are either file paths relative to __PROJECT_ROOT, 
	or absolute file paths. (Basically the same as ..._DIR.)


Variables set by the system:
----------------------------

Most of these can be overridden by presetting them before calling the 
prj script.

- __PROJECT_ROOT

	The root dir of the project. 

	It is calculated relative to the location of the prj script
	(which is currently assumed to be at the fixed location:
	__PROJECT_ROOT/.prj-tools/{PLATFORM}).

	Note: if __PROJECT_ROOT is preset by a shortcut command, then of
	course the specified value will be used instead of deducing it.

- __PROJECT_METADATA_DIR

	Set to where the PROJECT.ini file is found, or to __PROJECT_ROOT
	if not.

	Note: (unless preset) the system will set this as a subdir path 
	relative to __PROJECT_ROOT.

- __PROJECT_ARCHIVE_SUBDIR or __PROJECT_ARCHIVE_ROOT

	(Only) one of them is set to PROJECT_ARCHIVE_DIR, depending on
	whether it is a relative or absolute path.

	Note: these cannot be preset; always newly set by the system.


Variables known to the system:
------------------------------

(As already mentioned, every one of these settings is optional. If nothing 
is explicitly set, a default value will be used if one is defined, or the
corresponding feature will just be ignored, if no default is applicable.)

- PROJECT_TAG	(Default: basename(__PROJECT_ROOT), with .ext, if present)


- PROJECT_VERSION_FILE          (Default: %__PROJECT_METADATA_DIR%/VERSION)

	This is the "change counter" for project-level baselining.

	If this file is not empty, its content will be used as the version
	tag for the next package. If it is empty, an automatic timestamp 
	will be used for tagging the packages.

	(It's either an absolute file path, or one relative to _PROJECT_ROOT.)

	If the specified (or default) file is not found, creating it will be
	attempted (and a warning is issued). If it fails, that's an error.


- PROJECT_ARCHIVE_DIR		(Default: .)

	This is where the "save" command will put the snapshot packages.

	(It's either an absolute directory path, or one relative to 
	_PROJECT_ROOT.)

	WARNING: This should *not* be a subdir of __PROJECT_ROOT in a 
	correctly configured environment (unless you want the size of
	every new package doubling after a short while...). Some proper
	backup space should be used instead.


- PROJECT_SHELL_INIT_FILE	(Default: none)

	This command script will be run ("call"-ed) by the shell command 
	("prj shell") on its invokation.

	It will receive the arguments originally passed to the shell command.


- PROJECT_CUSTOM_CMD_DIR	(Default: none)

	The location of any custom (new or modified) project commands.
	If the user wants to override default behavior, the replacement
	commands should be put here.

	(It's either an absolute directory path, or one relative to 
	_PROJECT_ROOT.)

	Notes:

	* This dir is *not* added to the PATH, and no need for it either, 
	  because it will be explicitly checked before executing any 
	  built-in project command. If an executable matching the command 
	  name is found in PROJECT_CUSTOM_CMD_DIR, it will be run. Otherwise, 
	  a built-in command is looked for and executed (if found).

	* This should not be the __PROJECT_ROOT (or, more precisely: the 
	  dir where the shortcut commands are located), as that would most 
	  likely cause infinite loops when trying to run the command scripts!

- PROJECT_PATH_ADD_CMD		(Default: none)

	If set (either to "prepend" or "append"), the project commands
	are added to the PATH, so they can be run via their own names, 
	not only via the "prj" dispatcher command.

	WARNING: The price of this is that some project command names
		will very likely clash with existing commans on your host
		system's PATH. (If so, and you still want this, you can 
		at least decide whether the system or the project commands 
		should take precedence, by setting append or prepend here
		respectively.)


- PROJECT_PATH_PREPEND_DIR	(Default: none)

	Prepended to PATH for the command frame.


- PROJECT_PATH_APPEND_DIR	(Default: none)

	Appended to PATH for the command frame.


Variables *not* known to the system:
------------------------------------

Any variable which is not recognized by the system is just added (unchanged)
to the command frame.

E.g.:
	PRODUCT=SuperhighLativus


COMMAND EXECUTION
-----------------

Command vs. shell mode
----------------------

The project commands can be invoked in two modes:

  a) Command mode: when a command "cmd" is called via the "prj" 
     dispatcher command, like:

        prj cmd [args...]

     or via its shortcut (proxy) command, like:

        cmd [args...]

     This is the preferred mode of operation in cases where a single command 
     needs to be run on a project at a time, instead of a "chain of commands",
     so a longer work session doesn't need to be maintained. (E.g. because 
     it's maintained by some other (like an IDE) tool, or because every
     project command is issued completely independently from each other.)


  b) Shell mode: when "prj shell" has been previously run, so that any 
     further project commands will be executed within that shell session 
     (command frame), with the project environment being maintained across 
     command calls.

     This may be preferred for working "manually" on a project, from the 
     command-line, possibly using various custom scripts and/or tools that 
     use the project environment provided by the shell command.

     An init script can also be configured (see: PROJECT_SHELL_INIT_FILE)
     for calling when the shell is started. This makes it easy to customize 
     the project environment before invoking other commands.

     Note: shell mode also makes it possible to invoke the project commands
     directly, without using the "prj" dispatcher command or any shortcut.
     (This needs to be manually enabled by setting PROJECT_PATH_ADD_CMD, 
     for it possibly being a problematic setup with "random" name clashes.)


Shortcuts
---------

Since the project commands are not (by default) put on the PATH (to avoid
name clashes), there are some convenience shortcuts provided, which can be
put into __PROJECT_ROOT (or to some place on the PATH, but that's only 
meaningful in shell mode, otherwise it would be difficult for a shortcut
to find the project's "prj" (dispatcher) command).

These shortcuts have the same name as the commands they "proxy" for (by 
calling prj internally; like save.cmd on Windows calling "prj save"), and 
have the same syntax and semantics as the original command, e.g.:

	save [version-tag]


Setting the working directory
-----------------------------

For every project command, the system sets the current working directory to 
__PROJECT_ROOT in the command frame. (Of course, restoring it to whatever 
it was before, when the command ends.)

This way references to project component directories can be both relative 
to the project root and set to fixed locations outside of the project root 
(e.g. interfacing with 3rd-party tools or referring to items shared across 
multiple projects etc.).

(Note: the commands themselves can set their working directory to whatever 
they want.)


Command session nesting
-----------------------

!!TODO: not explicitly supported yet. It will be, though, for subprojects etc.


Inheriting variables
--------------------

[!!FIXME: NOT SETTLED YET: Nested command sessions inherit both user-
defined and system-generated variables by default. Sub-projects, however, 
will still override anything as necessary.]


The project commands
--------------------

The (currently) available commands:

- help [command]

	Show the list of (built-in) commands.

- toolversion [changes]

	Show the version of this tool.

- shell

	Start a shell session for issuing further project commands
	within the same project command frame.

- env

	Show the currently effective project environment.

- version

	Show project status and/or identification information.

- pack [version-tag]

	Pack & save the current state (snapshot) of the project.

- new [clone] x:\new\project\root

	!!TODO: NOT YET IMPLEMENTED!

	Create a new project (which is optionally the clone of the current 
	one) in x:\new\project\root. ("Cloning" actually means a "deep copy",
	duplicating the project files themselves, too, not just copying the
	environment. This is essentially "branching" or "forking" a project.)
