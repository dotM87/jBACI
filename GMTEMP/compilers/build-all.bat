cd lib
set save=%path%
set path=c:\mingw\bin;%path%
call build-lib
cd ..\ccomp
call build-c
copy bacc.exe \jbaci\bin
cd ..\pascomp
call build-pas
copy bapas.exe \jbaci\bin
set path=%save%
cd ..
