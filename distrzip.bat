@echo off
call distr
cd distr

echo %date%
REM if format %date% = DD-MM-YYYY
REM set today=%date:~6,4%%date:~3,2%%date:~-0,2%
REM if format %date% = YYYY-MM-DD
set today=%date:~0,4%%date:~5,2%%date:~8,2%
echo %today%

zip -r -m -q MagnumFlasher-1.4-%today%.zip *.*
cd ..
