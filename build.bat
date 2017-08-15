@echo off

set platform=Debug
set buildtype=build
set nopause=0

:loop
if "%1" == "" goto :begin
if "%1" == "release" (
	set platform=Release
	goto cutargv
)
if "%1" == "clean" (
	set buildtype=rebuild
	goto cutargv
)
if "%1" == "nopause" (
	set nopause=1 
	goto cutargv
)
:cutargv
shift
goto :loop

:begin
set BaseDir=%CD%
set BuildConsoleTool=
goto findpathc

:findpathc
set BuildConsoleTool="C:\Program Files (x86)\Xoreax\IncrediBuild\BuildConsole.exe"
if exist %BuildConsoleTool% goto build

:findpathd
set BuildConsoleTool="D:\Program Files\Xoreax\IncrediBuild\BuildConsole.exe"
if exist %BuildConsoleTool% goto build

:errorexe
echo "找不到BuildConsole.exe"
goto end

:error
if "%nopause%" == "0" ( pause ) else exit /b %1
goto end

:build
cd %BaseDir%\win32\msvc
echo "build oncrpc.sln"
::call %BuildConsoleTool% oncrpc.sln /%buildtype% /cfg="%platform%|X64" /OpenMonitor
call %BuildConsoleTool% oncrpc.sln /%buildtype% /cfg="%platform%|X64"
if %ERRORLEVEL% neq 0 ( goto error %ERRORLEVEL% ) else goto :eof

:end
cd %BaseDir%