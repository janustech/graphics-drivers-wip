@echo on

for /d %%d in (Install*) do rmdir /S /Q %%d

del /F *.log *.wrn *.err

cd viogpudo
call clean.bat
cd ..

