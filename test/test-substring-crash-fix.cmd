@echo off
::
:: NOTE: All of the below tests were repeatable only in one boot session.
:: After rebooting, I could no longer make it crash at all.
:: So, it seems to be a pointer overrun issue somewhere.
::
exit

set prg=vc\regenv.exe

:: NO CRASH

::%prg% set    v x
::
::%prg% set -s v x
::%prg% set -s v xx
::%prg% set -s v xxx

::%prg% set v xx
::%prg% set -s v x

::%prg% set v xx;y
::%prg% set v xx;yy
::%prg% set v xx;yyyyy
::%prg% set v xx;yyyyyy
::%prg% set v xx;yyyyyyy
::%prg% set v xx;yyyyyyyy
::%prg% set v x;yyy
::Also, if no previous value was set at all.
::
::%prg% set -s v xxxx

::%prg% set    v ;yyyy
::%prg% set -s v xxxx

::%prg% set v x;yyy
::%prg% set -s v x

::%prg% set    v ;yyyy
::%prg% set -s v xxxx

::%prg% set    v xx;yyy
::%prg% set -s v this_one_ok
::%prg% set -s v this_one_works

::%prg% set    v x;y
::%prg% set -s v this_not_crashed

::%prg% set    v 123456
::%prg% set -s v 123456_123456OK
::%prg% set    v 123
::%prg% set -s v 1234

::%prg% set    v 12345
::%prg% set -s v 123456_123456XXX


:: CRASH 

%prg% set    v 123456
%prg% set -s v 123456_123456XXX

%prg% set    v 1234567
%prg% set -s v 123456_123456XXX


%prg% set    v xx;yyy
%prg% set -s v anything_at_all

%prg% set v 666666
%prg% set -s v x


%prg% set v xx;yyyy
%prg% set -s v x

%prg% set    v 123456
%prg% set -s v x
%prg% set -s v xx
%prg% set -s v xxxxxxxxxxxxxxxxx


::OK

::%prg% set    v 1;x
::%prg% set -s v x
::%prg% set -s v x
::%prg% set -s v xx
::%prg% set -s v xxx
::%prg% set -s v xxxx

::%prg% set    v 1;1234
::%prg% set -s v xxxxxxxxx



echo Check with Regedit: HKEY_CURRENT_USER\Volatile Environment\v should also be "xx;zzzz"!

