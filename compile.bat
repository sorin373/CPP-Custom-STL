@echo off

set SOURCE_FILE=src\main.cpp
set OUTPUT_FILE=build\main.exe

if exist %OUTPUT_FILE% (
    echo === del %OUTPUT_FILE%
    del %OUTPUT_FILE%
)

REM
set FLAGS=-Os -flto

set COMMAND=g++ -o %OUTPUT_FILE% %SOURCE_FILE% %FLAGS%
set STRIP_COMMAND=strip %OUTPUT_FILE%

echo === %COMMAND%

%COMMAND%

if exist %OUTPUT_FILE% (
    echo *** Compilation successful. Executable '%OUTPUT_FILE%' created.

    echo === %STRIP_COMMAND%
    %STRIP_COMMAND%

    echo === %OUTPUT_FILE%
    %OUTPUT_FILE%
) else (
    echo Compilation failed.
)

pause