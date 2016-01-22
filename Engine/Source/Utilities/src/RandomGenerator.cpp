#include "stdafx.h"
#include "RandomGenerator.h"
#include <time.h>
#include "ServiceLocator.h"

using namespace Utilities;
using namespace Base;

//  *******************************************************************************************************************
cRandomGenerator::cRandomGenerator()
{
	VSetRandomSeed(987654321);
}

//  *******************************************************************************************************************
float cRandomGenerator::VRandom()
{
	int r = VRandomRange(0, 0xffffffff);
	float divisor = static_cast<float>(0xffffffff);
	return (r / divisor) + 0.5f;
}

//  *******************************************************************************************************************
int cRandomGenerator::VRandomRange(const int start, const int end)
{
	SP_ASSERT(end > start)(start)(end).SetCustomMessage("End is less than start");

	unsigned int val = 0;
	unsigned int range_diff = 0;
	if (end < 0)
	{
		range_diff = start - end;
		val = GetNextRandomNumber();
		if ((range_diff + 1) != 0)
		{
			val %= (range_diff + 1);
			val += end;
		}
	}
	else
	{
		range_diff = end - start;
		val = GetNextRandomNumber();
		if ((range_diff + 1) != 0)
		{
			val %= (range_diff + 1);
			val += start;
		}
	}
	return val;
}

//  *******************************************************************************************************************
void cRandomGenerator::VSetRandomSeed(const unsigned int seed)
{
	if (seed == 0)
	{
		m_Seed = 12345;
	}
	else
	{
		m_Seed = seed;
	}

	// Simple pseudo-random to reseed the seeds.
	unsigned int j = m_Seed;
	for (int i = 0; i < SeedCount; i++)
	{
		j = j ^ (j << 13);
		j = j ^ (j >> 17);
		j = j ^ (j << 5);
		_q[i] = j;
	}

	_c = 362436;
	_i = SeedCount - 1;
}

//  *******************************************************************************************************************
unsigned int cRandomGenerator::GetNextRandomNumber()
{
	const unsigned int r = 0xfffffffe;
	const unsigned int a = 716514398;  // for SEED_COUNT=8, period approx 2^285

	_i = (_i + 1) & (SeedCount - 1);
	UINT64 t = a * _q[_i] + _c;
	_c = (unsigned int)(t >> 32);
	unsigned int x = (unsigned int)(t + _c);
	if (x < _c)
	{
		x++;
		_c++;
	}

	unsigned int val = r - x;
	_q[_i] = val;
	return val;
}

//  *******************************************************************************************************************
void IRandomGenerator::CreateAsService()
{
	cServiceLocator::GetInstance()->Register<IRandomGenerator>(shared_ptr<IRandomGenerator>(DEBUG_NEW cRandomGenerator()));
}
