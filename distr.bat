@echo off
mkdir distr
copy logo.bmp distr
mkdir distr\ru
copy lang.mo distr\ru
copy *.exe distr
copy *.ico distr
copy Release\*.exe distr
copy wx2\*30u_*.dll distr
copy libcurl\*.dll distr
del distr\libeay32.dll
del distr\ssleay32.dll
copy ms\*.dll distr
copy winsparkle\Release\*.dll distr
copy *.txt distr


REM cd distr
REM zip -r -m -q MagnumFlasher-%today%.zip *.*
REM cd ..
