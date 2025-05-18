@echo off
setlocal

:: Compiler settings
set CC=gcc
set CFLAGS=-Iinclude -Llib -lraylib -lopengl32 -lgdi32 -lwinmm
set SRC=src\main.c
set OUT=bin\game.exe

:: Make bin folder if not exist
if not exist bin mkdir bin

:: Compile
%CC% %SRC% %CFLAGS% -o %OUT%
if errorlevel 1 (
    echo Compilation Failed
    exit /b 1
)
:: Done
echo Build complete!
.\bin\game.exe
pause
