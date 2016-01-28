//  *******************************************************************************************************************
//  RandomGenerator   version:  1.0   Ankur Sheel  date: 2011/04/16
//  *******************************************************************************************************************
//  Pseudorandom number generator.-- Thatcher Ulrich 2003
//  This source code has been donated to the Public Domain.  Do
//  whatever you want with it.

//  PRNG code adapted from the complimentary-multiply-with-carry
//  code in the article: George Marsaglia, "Seeds for Random Number
//  Generators", Communications of the ACM, May 2003, Vol 46 No 5,
//  pp90-93.
//
//  The article says:
//
//  "Any one of the choices for seed table size and multiplier will
//  provide a RNG that has passed extensive tests of randomness,
//  particularly those in [3], yet is simple and fast --
//  approximately 30 million random 32-bit integers per second on a
//  850MHz PC.  The period is a*b^n, where a is the multiplier, n
//  the size of the seed table and b=2^32-1.  (a is chosen so that
//  b is a primitive root of the prime a*b^n + 1.)"
//
//  [3] Marsaglia, G., Zaman, A., and Tsang, W.  Toward a universal
//  random number generator.  _Statistics and Probability Letters
//  8_ (1990), 35-39.

//  const Uint64 a = 18782;  // for SEED_COUNT=4096, period approx 2^131104 (from Marsaglia usenet post 2003-05-13)
//  const Uint64 a = 123471786;  // for SEED_COUNT=1024, period approx 2^32794
//  const Uint64 a = 123554632;  // for SEED_COUNT=512, period approx 2^16410
//  const Uint64 a = 8001634;  // for SEED_COUNT=256, period approx 2^8182
//  const Uint64 a = 8007626;  // for SEED_COUNT=128, period approx 2^4118
//  const Uint64 a = 647535442;  // for SEED_COUNT=64, period approx 2^2077
//  const Uint64 a = 547416522;  // for SEED_COUNT=32, period approx 2^1053
//  const Uint64 a = 487198574;  // for SEED_COUNT=16, period approx  2^540
//  const Uint64 a = 716514398;  // SEED_COUNT=8, period approx 2^285
//  *******************************************************************************************************************
#ifndef RandomGenerator_h__
#define RandomGenerator_h__

#include "RandomGenerator.hxx"

namespace Utilities
{
  class cRandomGenerator : public IRandomGenerator
  {
  public:
    cRandomGenerator();
    int VRandomRange(const int start, const int end) OVERRIDE;
    float VRandom() OVERRIDE;
    void VSetRandomSeed(const unsigned int seed) OVERRIDE;
    unsigned int VGetRandomSeed() OVERRIDE { return m_Seed; }
  private:
    unsigned int GetNextRandomNumber();

  private:
    static const int SeedCount = 8;

    unsigned int m_Seed;
    unsigned int _q[SeedCount];
    unsigned int _c;
    unsigned int _i;
  };
}  // namespace Utilities
#endif  // RandomGenerator_h__
