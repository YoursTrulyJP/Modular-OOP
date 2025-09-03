// BWIllustrator.cpp is a derived class that maps white pixels to 0 and any non-white pixel to 1,
// then outputs PBM text format (P1).
//
// Note: We consider a pixel white only if R=G=B=255 (exact white), which
// matches how UJImage initialises new pixels to white.

module;
#include <sstream>
#include <string>

export module BWIllustrator;

import LibUtility;
import FlagIllustrator;

export class BWIllustrator : public FlagIllustrator
{
public:
    BWIllustrator();
    BWIllustrator(int intHeight, int intWidth);

    std::string exportImage() const override;
private:
    bool isWhite(const UJPixel& recPixel) const;
};

BWIllustrator::BWIllustrator() : FlagIllustrator() {}
BWIllustrator::BWIllustrator(int intHeight, int intWidth)
: FlagIllustrator(intHeight, intWidth) {}

// exportImage: produce "P1" PBM header then a grid of bits.
std::string BWIllustrator::exportImage() const
{
    std::stringstream ssPBM;
    ssPBM << "P1" << std::endl
          << _image->getWidth() << ' ' << _image->getHeight() << std::endl;

    for(int r = 0; r < _image->getHeight(); ++r)
    {
        for(int c = 0; c < _image->getWidth(); ++c)
        {
            // Bit value: 0 for white, 1 for non-white (black)
            int intBitValue = isWhite(_image->getPixel(r, c)) ? 0 : 1;
            ssPBM << intBitValue << ' ';
        }
        ssPBM << std::endl;
    }
    return ssPBM.str();
}

// isWhite: check whether pixel is exactly white.
bool BWIllustrator::isWhite(const UJPixel& recPixel) const
{
    return recPixel.intRed == 255 && recPixel.intGreen == 255 && recPixel.intBlue == 255;
}