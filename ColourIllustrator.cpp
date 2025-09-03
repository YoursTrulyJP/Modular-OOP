// ColourIllustrator.cpp is a concrete derived class that outputs the full-colour P3 PPM.
// Purpose: demonstrate polymorphism — exportImage() returns a P3 string
// that is different from Grayscale and BW derived classes.

module;
#include <sstream>
#include <string>

export module ColourIllustrator;

import LibUtility;
import FlagIllustrator;

export class ColourIllustrator : public FlagIllustrator
{
public:
    ColourIllustrator();
    ColourIllustrator(int intHeight, int intWidth);

    // Polymorphic override of the pure virtual exportImage in FlagIllustrator.
    std::string exportImage() const override;
};

// Default ctor: uses base default size via FlagIllustrator()
ColourIllustrator::ColourIllustrator() : FlagIllustrator() {}

// Parametrised ctor: delegates size to FlagIllustrator(int,int)
ColourIllustrator::ColourIllustrator(int intHeight, int intWidth)
: FlagIllustrator(intHeight, intWidth)
{}

// exportImage: delegates to UJImage::toPPM() which produces a P3 colour image.
// Because derived classes override exportImage, main can call it polymorphically.
std::string ColourIllustrator::exportImage() const
{
    // No transformations needed — just convert internal image to PPM string.
    return _image->toPPM();
}