@echo off
call distr
cd distr

echo %date%
set today=%date:~6,4%%date:~3,2%%date:~-0,2%
echo %today%

zip -r -m -q MagnumFlasher-%today%.zip *.*
cd ..
