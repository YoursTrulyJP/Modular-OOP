// LibUtility.cpp is  a small utility module used across the project.
// Exports:
//  - UJPixel (pixel struct)
//  - ExitCode (enum used as return codes)
//  - FlagType (enum of supported flags)
//  - convToFlagType (helper to parse command-line args)


//  - This module centralises small common definitions so other modules include this.
//  - We include iostream because convToFlagType can print error messages.

module;
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

export module LibUtility;

// ------------ Data types exported ------------
export struct UJPixel
{
    int intRed;   // 0..255
    int intGreen; // 0..255
    int intBlue;  // 0..255
};

export enum ExitCode
{
    SUCCESS = 0,
    ERROR_RANGE,
    ERROR_ARGS,
    ERROR_CONV
};

export enum FlagType
{
    AUSTRIA = 0,
    JAPAN   = 1,
    NIGERIA = 2
};

// ------------ Helper function ------------
// convToFlagType:
// Convert string (from argv) to FlagType enum.
// Exits with ERROR_CONV if invalid input.
export FlagType convToFlagType(const std::string& strArg)
{
    std::stringstream ssConv{strArg};
    int intTemp = 0;
    ssConv >> intTemp;
    if(ssConv.fail() || intTemp < 0 || intTemp > 2)
    {
        // Clear error message, then exit.
        std::cerr << "ERROR! Could not convert command line argument to a flag type. Terminating." << std::endl;
        std::exit(ERROR_CONV);
    }
    return static_cast<FlagType>(intTemp);
}