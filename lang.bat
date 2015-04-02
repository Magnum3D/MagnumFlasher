@echo off
dir /B *.c* > files.txt
bin\xgettext --from-code=UTF-8 --keyword=_ --keyword=wxT --no-wrap --escape -ffiles.txt -olang.pot
del files.txt

