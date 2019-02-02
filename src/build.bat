@echo off

set platform=x64

set debugFlags=/Zi
set encodingFlags=/utf-8 /D UNICODE
set includeFlags=-I..\third_party\SDL2\include\
set linkFlags=/link /ENTRY:WinMainCRTStartup ^
                    /SUBSYSTEM:WINDOWS ^
                    /LIBPATH:..\third_party\SDL2\lib\%platform%\
set libFlags=SDL2.lib SDL2main.lib SDL2test.lib

where cl > nul 2>&1 || call ..\misc\shell.bat

if not exist ..\build mkdir ..\build
if not exist ..\build\SDL2.dll copy ..\third_party\SDL2\lib\%platform%\SDL2.dll ..\build
pushd ..\build
cl ..\src\main.cpp ^
    %debugFlags% %libFlags% %encodingFlags% %includeFlags% %linkFlags%
popd

start ..\build\main.exe