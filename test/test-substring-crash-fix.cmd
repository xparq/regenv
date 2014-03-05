@echo off
set aaa=xx;zzzz
vc\regenv set aaa xx;zzzz
vc\regenv set -s aaa zzzz
set aaa
echo Check above: "aaa" should be "xx;zzzz"!
echo Check with Regedit: HKEY_CURRENT_USER\Volatile Environment\aaa should also be "xx;zzzz"!