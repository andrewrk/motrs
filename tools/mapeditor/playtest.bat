@echo on
cd ..\..
call make.bat playtest
motrs.exe %*
