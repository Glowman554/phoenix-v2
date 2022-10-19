@echo off
setlocal EnableDelayedExpansion
:: test and acquire admin rights
cd /d %~dp0 & echo/
if not "%1"=="UAC" (
    >nul 2>&1 net file && echo Got admin rights || (echo No admin rights & ^
MSHTA "javascript: var shell = new ActiveXObject('shell.application'); shell.ShellExecute("%~snx0", 'UAC', '', 'runas', 1);close();"))
:: re-test admin rights
echo/ & >nul 2>&1 net file && (echo Got admin rights & echo/) || (echo No admin rights. Exiting... & goto :end)

:usercode
copy assembler\assembler.exe C:\Windows\System32\phoenix-as.exe
copy disassembler\disassembler.exe C:\Windows\System32\phoenix-disas.exe
copy emulator\emulator.exe C:\Windows\System32\phoenix-emu.exe
copy microcode\microcode.exe C:\Windows\System32\phoenix-microcode.exe

:end
exit /b 
