@echo off

set IZARC="C:\Program Files\IZArc\IZARCC.exe"
set TEMP=tmp
set CFG=%TEMP%\appinfo.json

mkdir %TEMP%
mkdir %TEMP%\web

setLocal
setLocal EnableDelayedExpansion

call :UPDATE_APP_INFO_VERSION appinfo.json "version="
call :UPDATE_REPOSITORY_VERSION RepositoryInfo.xml "Version"

echo Copying required files:
xcopy /Q /I /E ..\img %TEMP%\web\img
xcopy /Q /I ..\*.inc %TEMP%\web
xcopy /Q /I ..\*.php %TEMP%\web
xcopy /Q /I ..\*.txt %TEMP%\web
xcopy /Q /I appinfo.json %TEMP%

echo Packing files...
%IZARC% -a -p -r -w .\KartinaTV.tar %TEMP% > nul
del KartinaTV-installer.zip
%IZARC% -a -cx .\KartinaTV-installer.zip .\KartinaTV.tar > nul

echo Cleaning...
del KartinaTV.tar
rmdir /S /Q %TEMP%

echo Done.
pause
goto :EOF


:UPDATE_APP_INFO_VERSION
set oldVersion=
for /F "tokens=2 delims==," %%A in ('findstr /c:%2 %1') do (
    set oldVersion=%%~A
)
if "%oldVersion%" NEQ "" call :INCREASE_VERSION %1 %oldVersion%
goto :EOF


:UPDATE_REPOSITORY_VERSION
set oldVersion=
for /F "tokens=2 delims=<> " %%A in ('findstr /c:%2 %1') do (
    set oldVersion=%%A
)
if "%oldVersion%" NEQ "" call :INCREASE_VERSION %1 %oldVersion%
goto :EOF


:INCREASE_VERSION
set major=
set minor=
set build=
for /F "tokens=1-8 delims=.-" %%A in ("%2" ) do (
    set major=%%A
    set minor=%%B
    set /A build=%%C+1
)
set newVersion=%major%.%minor%.%build%
if "%newVersion%" EQU ".." goto :EOF

rem Multiple occurence support: only the wished one should be changed
set entryNumber=%3
if "%entryNumber%" EQU "" set entryNumber=1

echo Version update %2 to %newVersion% in %1
for /F "tokens=* delims=" %%A in (%1) do (
    set str=%%A
    set newStr=!str:%2=%newVersion%!
    if "!newStr!" NEQ "!str!" set /A entryNumber=!entryNumber!-1
    if "!entryNumber!" EQU "0" (
        set entryNumber=-1
        set str=!newStr!
    )
    echo !str!>> temp.tmp
)
move temp.tmp %1
goto :EOF


:EOF
