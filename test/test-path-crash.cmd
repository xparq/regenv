@echo off

set prg=vc\regenv.exe

set kaka=xx;pisi
call %prg% set kaka xx;c:\pisi

call %prg% set -s kaka nagyonhosszu-string-de-el-kellene-erni-vele-a-256-bajtot-ugyhogy-ezt-tobbszor-is-hozza-kell-adni.........
call %prg% set -s kaka nagyonhosszu-string-de-el-kellene-erni-vele-a-256-bajtot-ugyhogy-ezt-tobbszor-is-hozza-kell-adni.........
call %prg% set -s kaka nagyonhosszu-string-de-el-kellene-erni-vele-a-256-bajtot-ugyhogy-ezt-tobbszor-is-hozza-kell-adni.........
call %prg% set -s kaka nagyonhosszu-string-de-el-kellene-erni-vele-a-256-bajtot-ugyhogy-ezt-tobbszor-is-hozza-kell-adni.........

:: print kaka from the current env (should be invisible!)
set kaka

rem call %prg% get kaka
echo Check the value in the Registry!