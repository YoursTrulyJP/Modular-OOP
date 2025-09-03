// GrayscaleIllustrator.cpp is a derived class that converts colour pixels to an average-intensity grayscale
// value and outputs in the P2 (PGM) text format.

module;
#include <sstream>
#include <string>

export module GrayscaleIllustrator;

import LibUtility;
import FlagIllustrator;

export class GrayscaleIllustrator : public FlagIllustrator
{
public:
    GrayscaleIllustrator();
    GrayscaleIllustrator(int intHeight, int intWidth);

    // Override exportImage to provide P2 output (grayscale).
    std::string exportImage() const override;
private:
    int average(const UJPixel& recPixel) const; // integer average of RGB
};

GrayscaleIllustrator::GrayscaleIllustrator() : FlagIllustrator() {}
GrayscaleIllustrator::GrayscaleIllustrator(int intHeight, int intWidth)
: FlagIllustrator(intHeight, intWidth) {}

// Convert each pixel to an intensity 0..255 and write P2 format.
std::string GrayscaleIllustrator::exportImage() const
{
    std::stringstream ssPGM;
    ssPGM << "P2" << std::endl
          << _image->getWidth() << ' ' << _image->getHeight() << std::endl
          << 255 << std::endl; // max intensity

    for(int r = 0; r < _image->getHeight(); ++r)
    {
        for(int c = 0; c < _image->getWidth(); ++c)
        {
            int intIntensity = average(_image->getPixel(r, c));
            ssPGM << intIntensity << ' ';
        }
        ssPGM << std::endl;
    }
    return ssPGM.str();
}

// average: (R + G + B) / 3
// Using integer arithmetic is fine for PGM (0..255).
int GrayscaleIllustrator::average(const UJPixel& recPixel) const
{
    return (recPixel.intRed + recPixel.intGreen + recPixel.intBlue) / 3;
}