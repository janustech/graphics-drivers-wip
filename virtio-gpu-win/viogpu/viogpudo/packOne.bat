@echo off
: Param1 - Win10 | Win8
setlocal
: Param2 - x86|x64
: Param3 - sys name
: Param4 - dbg|none
 
if "%2"=="x64" set %%2=amd64

if /i "%1"=="Win10" goto :checkarch
if /i "%1"=="Win8" goto :checkarch

goto :printerr
:checkarch
if /i "%2"=="x86" goto :makeinstall
if /i "%2"=="x64" goto :makeinstall
:printerr
echo wrong parameters (1)%1 (2)%2 (3)%3
pause
goto :eof

:makeinstall
set INST_OS=%1
set INST_ARC=%2
set SYS_NAME=%3
set INST_FLAVOR=fre
set INST_SUFFIX=_DO_Release


if /i "%4"=="" goto :flavor_done
set INST_FLAVOR=chk
set INST_SUFFIX=_DO_Debug
:flavor_done

if /i "%INST_ARC%"=="x64" goto :set_x64

set INST_EXT=i386
goto :startcopy

:set_x64
set INST_ARC=amd64
set INST_EXT=amd64

:startcopy
set SYS_PATH_AND_NAME=obj%INST_FLAVOR%_%INST_OS%_%INST_ARC%\%INST_EXT%\%SYS_NAME%.sys
set PDB_PATH_AND_NAME=obj%INST_FLAVOR%_%INST_OS%_%INST_ARC%\%INST_EXT%\%SYS_NAME%.pdb
set INF_PATH_AND_NAME=obj%INST_FLAVOR%_%INST_OS%_%INST_ARC%\%INST_EXT%\%SYS_NAME%.inf
set DVL_PATH_AND_NAME=%SYS_NAME%.DVL.xml
set DESTDIR=..\Install%INST_SUFFIX%\%INST_OS%\%INST_ARC%

mkdir %DESTDIR%
del /Q %DESTDIR%\%FILE_NAME%.*
copy /Y %SYS_PATH_AND_NAME% %DESTDIR%
copy /Y %PDB_PATH_AND_NAME% %DESTDIR%
copy /Y %INF_PATH_AND_NAME% %DESTDIR%

:create_cat
echo "Setting OS mask for:" %1 %2

if /i "%1"=="Win10" goto create_win10
if /i "%1"=="Win8" goto create_win8
goto error_inf2cat

:create_win8
if not exist %DVL_PATH_AND_NAME% goto do_the_job
if /i "%2"=="x64" copy /Y %DVL_PATH_AND_NAME% %DESTDIR%
:do_the_job
if /i "%2"=="x86" set _OSMASK_=8_X86
if /i "%2"=="x64" set _OSMASK_=8_X64,Server8_X64
goto run_inf2cat

:create_win10
setlocal
if not exist %DVL_PATH_AND_NAME% goto do_the_job
if /i "%2"=="x64" copy /Y %DVL_PATH_AND_NAME% %DESTDIR%
:do_the_job
if /i "%2"=="x86" set _OSMASK_=10_X86
if /i "%2"=="x64" set _OSMASK_=10_X64,Server10_X64
goto run_inf2cat

:error_inf2cat 
echo "Error setting OS mask for inf2cat"
goto after_inf2cat

:run_inf2cat
setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" %INST_ARC%
inf2cat /driver:%DESTDIR% /os:%_OSMASK_%
pushd %DESTDIR%
signtool sign /a *.cat *.sys
popd
endlocal

:after_inf2cat
