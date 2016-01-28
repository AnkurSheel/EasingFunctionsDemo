//  *******************************************************************************************************************
//  Random   version:  1.0   Ankur Sheel  date: 2011/04/16
//  ----------------------------------------------------------------------------
//
//  ----------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
//  *******************************************************************************************************************
// adapted from Game Coding Complete
//  *******************************************************************************************************************
#ifndef RandomGenerator_hxx__
#define RandomGenerator_hxx__

#include "UtilitiesDefines.h"

namespace Utilities
{
  class IRandomGenerator
  {
  public:
    virtual ~IRandomGenerator() {}
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns a number in the range [start:end]
    ///
    /// @param[in] start The min number
    /// @param[in] end The max number
    /// @return a number in the range [start:end]
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual int VRandomRange(const int start, const int end) = 0;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns a random float between 0.0f - 1.0f
    ///
    /// @return a random float between 0.0f - 1.0f
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual float VRandom() = 0;
    virtual void VSetRandomSeed(const unsigned int seed) = 0;
    virtual unsigned int VGetRandomSeed() = 0;
    UTILITIES_API static void CreateAsService();
  };
}  // namespace Utilities
#endif  // RandomGenerator_hxx__
