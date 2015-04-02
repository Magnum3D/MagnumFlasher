@echo off
mkdir distr
copy logo.bmp distr
mkdir distr\ru
copy lang.mo distr\ru
copy *.exe distr
copy *.ico distr
copy Release\*.exe distr
copy wx\*.dll distr
copy libcurl\*.dll distr

echo %date%
set today=%date:~6,4%%date:~3,2%%date:~-0,2%
echo %today%

cd distr
zip -r -m -q MagnumFlasher-%today%.zip *.*

