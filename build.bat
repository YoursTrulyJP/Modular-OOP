@echo off
REM Prac6 build script

REM Create bin and output directories if missing
if not exist "..\bin" mkdir "..\bin"
if not exist "..\output" mkdir "..\output"

echo Compiling...
g++ --std=c++20 -fmodules-ts -c LibUtility.cpp
g++ --std=c++20 -fmodules-ts -c UJImage.cpp
g++ --std=c++20 -fmodules-ts -c FlagIllustrator.cpp
g++ --std=c++20 -fmodules-ts -c ColourIllustrator.cpp
g++ --std=c++20 -fmodules-ts -c GrayscaleIllustrator.cpp
g++ --std=c++20 -fmodules-ts -c BWIllustrator.cpp
g++ --std=c++20 -fmodules-ts -c main.cpp

if %errorlevel% neq 0 (
    echo Compilation failed.
    pause
    exit /b %errorlevel%
)

echo Linking...
g++ LibUtility.o UJImage.o FlagIllustrator.o ColourIllustrator.o GrayscaleIllustrator.o BWIllustrator.o main.o -o "..\bin\prog.exe"

if %errorlevel% neq 0 (
    echo Linking failed.
    pause
    exit /b %errorlevel%
)

echo Cleaning up object files...
del *.o 2>nul

echo Generating colour PPMs (default illustrator = Colour)
"..\bin\prog.exe" 0 > "..\output\image_au.ppm"
"..\bin\prog.exe" 1 > "..\output\image_jp.ppm"
"..\bin\prog.exe" 2 > "..\output\image_ng.ppm"

echo Generating a grayscale PGM and a PBM (BW)
"..\bin\prog.exe" 0 1 > "..\output\image_au_gray.pgm"
"..\bin\prog.exe" 2 2 > "..\output\image_ng_bw.pbm"

echo Images created in ..\output
pause