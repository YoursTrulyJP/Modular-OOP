// UJImage.cpp handles a resizable image stored as a dynamically-allocated 2D array of UJPixel.
// Responsibilities:
//  - allocate/deallocate the 2D pixel array
//  - deep-copy semantics (copy ctor clones pixel data)
//  - accessor/mutator with range checks
//  - toPPM() for colour (P3) output (used by ColourIllustrator)
//
// Important invariants:
//  - _image points to an array of _rows pointers, each pointing to an array of _cols UJPixel
//  - dealloc() must safely free this memory (no leaks)

module;
#include <cassert>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>

export module UJImage;

import LibUtility;

export class UJImage
{
public:
    UJImage();
    UJImage(int intRows, int intCols);
    UJImage(const UJImage& objOriginal); // deep copy
    ~UJImage();

    // Convert internal pixel grid to P3 PPM string (colour).
    std::string toPPM() const;

    // Accessors/mutators (with range enforcement)
    int getHeight() const;
    int getWidth()  const;
    UJPixel getPixel(int intRow, int intCol) const;
    void setPixel(int intRow, int intCol, const UJPixel& recPixel);

private:
    // Helpers
    void alloc(int intRows, int intCols);     // allocate the 2D grid
    void clone(const UJImage& objOriginal);   // deep copy
    void dealloc();                           // free grid
    void enforceRange(int intValue, int intMin, int intMax) const;

    // State
    UJPixel** _image = nullptr; // pointer to rows (each row is UJPixel*)
    int _rows = 0;
    int _cols = 0;
};

// ---------- Implementations ----------

UJImage::UJImage() : UJImage(2, 2) // default tiny image
{}

UJImage::UJImage(int intRows, int intCols)
{
    // Validate & allocate
    alloc(intRows, intCols);
}

UJImage::UJImage(const UJImage& objOriginal)
: UJImage(objOriginal._rows, objOriginal._cols) // reuse alloc via delegating ctor
{
    // Deep copy pixel values
    clone(objOriginal);
}

UJImage::~UJImage()
{
    // Ensure resources freed
    dealloc();
}

// toPPM: returns a P3 colour PPM string.
// The header is:
//   P3
//   <width> <height>
//   255
// then pixel triples row-major.
std::string UJImage::toPPM() const
{
    std::stringstream ssPPM;
    ssPPM << "P3" << std::endl
          << _cols << ' ' << _rows << std::endl
          << 255 << std::endl;
    for(int r = 0; r < _rows; ++r)
    {
        for(int c = 0; c < _cols; ++c)
        {
            ssPPM << _image[r][c].intRed << ' '
                  << _image[r][c].intGreen << ' '
                  << _image[r][c].intBlue << ' ';
        }
        ssPPM << std::endl;
    }
    return ssPPM.str();
}

// alloc: create the rows and columns on the heap.
// We always initialise pixels to white (255,255,255).
void UJImage::alloc(int intRows, int intCols)
{
    // store dimensions
    _rows = intRows;
    _cols = intCols;

    // allocate array of row pointers
    _image = new UJPixel*[_rows];
    for(int r = 0; r < _rows; ++r)
    {
        // allocate each row
        _image[r] = new UJPixel[_cols];
        for(int c = 0; c < _cols; ++c)
        {
            // default pixel = white
            _image[r][c] = {255, 255, 255};
        }
    }
}

// clone: deep copy pixel values from original into this->_image.
void UJImage::clone(const UJImage& objOriginal)
{
    assert(_rows == objOriginal._rows);
    assert(_cols == objOriginal._cols);
    for(int r = 0; r < _rows; ++r)
    {
        for(int c = 0; c < _cols; ++c)
            _image[r][c] = objOriginal._image[r][c];
    }
}

// dealloc: free rows then the row-pointer array.
// Important: set pointers to nullptr to avoid accidental reuse.
void UJImage::dealloc()
{
    if(_image != nullptr)
    {
        for(int r = 0; r < _rows; ++r)
        {
            delete [] _image[r];     // free row pixels
            _image[r] = nullptr;
        }
        delete [] _image;           // free array of row pointers
        _image = nullptr;
    }
}

// enforceRange: helper used by accessors/mutators to ensure indexes/values are valid.
// exit(ERROR_RANGE) on invalid value (explicit, loud failure for assignment).
void UJImage::enforceRange(int intValue, int intMin, int intMax) const
{
    if(intValue < intMin || intValue > intMax)
    {
        std::cerr << "ERROR! " << intValue << " must be within [" << intMin << ", " << intMax << "]. Terminating." << std::endl;
        std::exit(ERROR_RANGE);
    }
}

int UJImage::getHeight() const { return _rows; }
int UJImage::getWidth()  const { return _cols; }

UJPixel UJImage::getPixel(int intRow, int intCol) const
{
    enforceRange(intRow, 0, _rows - 1);
    enforceRange(intCol, 0, _cols - 1);
    return _image[intRow][intCol];
}

void UJImage::setPixel(int intRow, int intCol, const UJPixel& recPixel)
{
    // validate the indices and the RGB values before assignment
    enforceRange(intRow, 0, _rows - 1);
    enforceRange(intCol, 0, _cols - 1);
    enforceRange(recPixel.intRed,   0, 255);
    enforceRange(recPixel.intGreen, 0, 255);
    enforceRange(recPixel.intBlue,  0, 255);
    _image[intRow][intCol] = recPixel;
}