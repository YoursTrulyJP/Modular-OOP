// FlagIllustrator.cpp is an abstract base class for producing flags as images.
// Responsibilities:
//  - maintain a UJImage on the heap (_image)
//  - provide drawing helpers for flags (AUSTRIA, JAPAN, NIGERIA)
//  - own allocation/deallocation of _image
//  - declare pure virtual exportImage() so derived classes implement different
//    output formats (colour P3, grayscale P2, PBM P1).
//
// This file contains the implementation for non-virtual helpers and the ctor/dtor.
// exportImage() is declared pure virtual so this class is abstract.
// The virtual destructor calls dealloc() so deleting via base pointer frees memory.

module;
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

export module FlagIllustrator;

import LibUtility;
import UJImage;

export class FlagIllustrator
{
public:
    //public
    FlagIllustrator(); // default dims
    FlagIllustrator(int intHeight, int intWidth);
    FlagIllustrator(const FlagIllustrator& objOriginal);

    // Virtual destructor: required because we delete derived objects via base pointer.
    virtual ~FlagIllustrator();

    // The drawing function: this will set pixels in _image according to eType.
    // Derived classes then choose how to export the image bytes / values.
    void illustrate(FlagType eType);

    // ---- PURE VIRTUAL ----
    // Requirement: exportImage is pure virtual, making this an abstract base class.
    // Derived classes must override exportImage() to produce different formats.
    virtual std::string exportImage() const = 0;

    static constexpr int DEF_HEIGHT = 480;
    static constexpr int DEF_WIDTH  = 640;

protected:
    // Protected so derived classes can read _image to produce outputs.
    UJImage* _image;

private:
    // Drawing helpers are implementation details (private).
    void drawAuFlag();
    void drawJPFlag();
    void drawNGFlag();
    double distance(int intY1, int intY2, int intX1, int intX2) const;
    void alloc(int intRows, int intCols);
    void clone(const FlagIllustrator& objOriginal);
    void dealloc();
    void enforceRange(int intArg, int intMin, int intMax) const;
};

// -------- implementations --------

// ctor delegates to the parameterised ctor (constructor chaining)
FlagIllustrator::FlagIllustrator() : FlagIllustrator(DEF_HEIGHT, DEF_WIDTH) {}

FlagIllustrator::FlagIllustrator(int intHeight, int intWidth)
{
    // Range checks are defensive programming to match the course style.
    enforceRange(intHeight, 0, 10000);
    enforceRange(intWidth,  0, 10000);
    alloc(intHeight, intWidth); // allocate UJImage on heap
}

FlagIllustrator::FlagIllustrator(const FlagIllustrator& objOriginal)
: FlagIllustrator(objOriginal._image->getHeight(), objOriginal._image->getWidth())
{
    // After allocation, deep-copy the pixel data
    clone(objOriginal);
}

// Virtual destructor: deallocates the _image (so deleting base pointer deletes UJImage).
FlagIllustrator::~FlagIllustrator()
{
    dealloc();
}

// illustrate: choose the correct helper based on FlagType
void FlagIllustrator::illustrate(FlagType eType)
{
    switch(eType)
    {
        case AUSTRIA: drawAuFlag(); break;
        case JAPAN:   drawJPFlag(); break;
        case NIGERIA: drawNGFlag(); break;
    }
}

// ----- Drawing helpers -----
// Each helper fills _image with the appropriate pixels for the flag.

void FlagIllustrator::drawAuFlag()
{
    // Austria = horizontal stripes: red, white, red
    int intThickness = _image->getHeight() / 3;
    UJPixel recRed   = {239, 51, 64};   // approximate Austria red
    UJPixel recWhite = {255, 255, 255}; // white

    for(int r = 0; r < _image->getHeight(); ++r)
    {
        for(int c = 0; c < _image->getWidth(); ++c)
        {
            if(r > intThickness && r < intThickness * 2)
                _image->setPixel(r, c, recWhite); // middle stripe
            else
                _image->setPixel(r, c, recRed);
        }
    }
}

void FlagIllustrator::drawJPFlag()
{
    // Japan = white background with a central red circle (diameter = 60% of height)
    UJPixel recRed   = {188, 0, 45};
    UJPixel recWhite = {255, 255, 255};
    double dblRadius = 0.3 * static_cast<double>(_image->getHeight());
    int intCR = _image->getHeight() / 2; // center row
    int intCC = _image->getWidth()  / 2; // center column

    for(int r = 0; r < _image->getHeight(); ++r)
    {
        for(int c = 0; c < _image->getWidth(); ++c)
        {
            // distance from (r,c) to circle center
            if(distance(r, intCR, c, intCC) <= dblRadius)
                _image->setPixel(r, c, recRed);
            else
                _image->setPixel(r, c, recWhite);
        }
    }
}

void FlagIllustrator::drawNGFlag()
{
    // Nigeria = vertical stripes: green, white, green
    int intThickness = _image->getWidth() / 3;
    UJPixel recGreen = {27, 115, 57};
    UJPixel recWhite = {255, 255, 255};

    for(int r = 0; r < _image->getHeight(); ++r)
    {
        for(int c = 0; c < _image->getWidth(); ++c)
        {
            if(c > intThickness && c < intThickness * 2)
                _image->setPixel(r, c, recWhite);
            else
                _image->setPixel(r, c, recGreen);
        }
    }
}

// Euclidean distance helper for the Japan flag circle calculation.
double FlagIllustrator::distance(int intY1, int intY2, int intX1, int intX2) const
{
    return std::sqrt(std::pow(intX2 - intX1, 2) + std::pow(intY2 - intY1, 2));
}

// alloc: puts a new UJImage on the heap (must be freed later).
void FlagIllustrator::alloc(int intHeight, int intWidth)
{
    _image = new UJImage(intHeight, intWidth);
}

// clone: deep copy pixel data from another FlagIllustrator's _image.
void FlagIllustrator::clone(const FlagIllustrator& objOriginal)
{
    assert(_image->getHeight() == objOriginal._image->getHeight());
    assert(_image->getWidth()  == objOriginal._image->getWidth());
    for(int r = 0; r < _image->getHeight(); ++r)
    {
        for(int c = 0; c < _image->getWidth(); ++c)
            _image->setPixel(r, c, objOriginal._image->getPixel(r, c));
    }
}

// dealloc: deletes the heap UJImage (calls UJImage::~UJImage which frees the 2D arrays).
// Called from FlagIllustrator::~FlagIllustrator() -> ensures no memory leaks from _image.
void FlagIllustrator::dealloc()
{
    if(_image != nullptr)
    {
        delete _image;  // UJImage destructor handles inner arrays
        _image = nullptr;
    }
}

// enforceRange: defensive checks used throughout the class to catch invalid values early.
void FlagIllustrator::enforceRange(int intArg, int intMin, int intMax) const
{
    if(intArg < intMin || intArg > intMax)
    {
        std::cerr << intArg << " must be in [" << intMin << ", " << intMax << "]" << std::endl;
        std::exit(ERROR_RANGE);
    }
}