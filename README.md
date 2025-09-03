# Modular-OOP
C++ project demonstrating and modular programming and OOP principles. 

C++ coursework demonstrating polymorphism and modular OOP. FlagIllustrator is an abstract base class; the project provides three concrete illustrators that export flag images in different formats:
BWIllustrator — exports PBM (black & white)
GrayscaleIllustrator — exports PGM (grayscale)
ColourIllustrator — exports PPM (colour)
The program illustrates (draws) one of three flags (Austria, Japan, Nigeria) and prints the image data to stdout.

Quick usage:
./flagillustrator <FlagType>
<FlagType> must be one of: 0, 1 or 2 corresponding to:
0 — AUSTRIA
1 — JAPAN
2 — NIGERIA


Example:
./flagillustrator 1 # draws and prints the Japan flag (PGM/PPM/PBM depending on illustrator)
Note that if the wrong number of arguments is supplied, the program exits with an error message.

Build:
Requires a C++20-capable compiler with module support. Build system is project-dependent; a simple example (may need adjustment for your toolchain):
# using a compiler with module flags (example only — toolchain-specific):
g++ -std=c++20 -fmodules-ts -o flagillustrator src/*.cpp

Or use CMake configured for C++20 modules if available in your environment.


Project layout:
- LibUtility — utility types (UJPixel, ExitCode, FlagType, convToFlagType)
- FlagIllustrator — abstract base class with illustrate() and pure virtual exportImage()
- BWIllustrator, GrayscaleIllustrator, ColourIllustrator — concrete derived classes
- UJImage (used internally) — image storage & toPPM() helper
- main.cpp — entry point (parses argument, creates an illustrator and calls illustrate() and exportImage() polymorphically)
