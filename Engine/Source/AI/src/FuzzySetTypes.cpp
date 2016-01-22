#include "stdafx.h"
#include "FuzzySetTypes.h"

//  *******************************************************************************************************************
cFuzzySetangle::cFuzzySetangle()
	: m_fPeakPoint(0)
	, m_fLeftOffset(0)
	, m_fRightOffset(0)
{
}

//  *******************************************************************************************************************
cFuzzySetangle::cFuzzySetangle(const float fPeak, const float fLeftOffset, const float fRightOffset)
{
	CreateangleSet(fPeak, fLeftOffset, fRightOffset);
}

//  *******************************************************************************************************************
cFuzzySetangle::~cFuzzySetangle()
{
}

//  *******************************************************************************************************************
void cFuzzySetangle::CreateangleSet(const float fMid, const float fLeft, const float fRight)
{
	// m_fRepresentativeValue = fMid;
	// cFuzzySet::cFuzzySet(fMid);
	CreateFuzzySet(fMid);
	m_fPeakPoint = fMid;
	m_fLeftOffset = fLeft;
	m_fRightOffset = fRight;
}

//  *******************************************************************************************************************
float cFuzzySetangle::CalculateDOM(const float val)
{
	if (((m_fRightOffset == 0.0) && (m_fPeakPoint == val))
		|| ((m_fLeftOffset == 0.0) && (m_fPeakPoint == val)))
	{
		return 1.0;
	}

	// find DOM if left of center
	if ((val <= m_fPeakPoint) && (val > (m_fPeakPoint - m_fLeftOffset)))
	{
		float grad = 1.0f / m_fLeftOffset;
		return (grad * (val - (m_fPeakPoint - m_fLeftOffset)));
	}

	// find DOM if right of center
	if ((val >= m_fPeakPoint) && (val < (m_fPeakPoint + m_fRightOffset)))
	{
		float grad = 1.0f / -m_fRightOffset;
		return (grad * (val - m_fPeakPoint) + 1.0f);
	}

	// out of range of this FLV, return zero
	return 0.0;
}

//  *******************************************************************************************************************
cFuzzySetLtShoulder::cFuzzySetLtShoulder()
	: m_fPeakPoint(0)
	, m_fLeftOffset(0)
	, m_fRightOffset(0)
{
}

//  *******************************************************************************************************************
cFuzzySetLtShoulder::cFuzzySetLtShoulder(const float fPeak, const float fLeftOffset, const float fRightOffset)
{
	CreateLtShoulderSet(fPeak, fLeftOffset, fRightOffset);
}
//  *******************************************************************************************************************

cFuzzySetLtShoulder::~cFuzzySetLtShoulder()
{
}

//  *******************************************************************************************************************
void cFuzzySetLtShoulder::CreateLtShoulderSet(const float fMid, const float fLeft, const float fRight)
{
	CreateFuzzySet(((fMid - fLeft)+fMid)/2);
	m_fPeakPoint = fMid;
	m_fLeftOffset = fLeft;
	m_fRightOffset = fRight;
}

//  *******************************************************************************************************************
float cFuzzySetLtShoulder::CalculateDOM(const float val)
{
	// test for the case where the left or right offsets are zero to prevent divide by zero errors below
	if (((m_fRightOffset == 0.0) && (m_fPeakPoint == val))
		|| ((m_fLeftOffset == 0.0) && (m_fPeakPoint == val)))
	{
		return 1.0;
	}

	// find DOM if right of center
	if ((val >= m_fPeakPoint) && (val < (m_fPeakPoint + m_fRightOffset)))
	{
		float grad = 1.0f / -m_fRightOffset;
		return (grad * (val - m_fPeakPoint) + 1.0f);
	}

	// find DOM if left of center
	if ((val < m_fPeakPoint) && (val >= m_fPeakPoint-m_fLeftOffset))
	{
		return 1.0;
	}

	// out of range of this FLV, return zero
	return 0.0;
}

//  *******************************************************************************************************************
cFuzzySetRtShoulder::cFuzzySetRtShoulder()
	: m_fPeakPoint(0)
	, m_fLeftOffset(0)
	, m_fRightOffset(0)
{
}

//  *******************************************************************************************************************
cFuzzySetRtShoulder::cFuzzySetRtShoulder(const float fPeak, const float fLeftOffset, const float fRightOffset)
{
	CreateRtShoulderSet(fPeak, fLeftOffset, fRightOffset);
}

//  *******************************************************************************************************************
cFuzzySetRtShoulder::~cFuzzySetRtShoulder()
{
}

//  *******************************************************************************************************************
void cFuzzySetRtShoulder::CreateRtShoulderSet(const float fMid, const float fLeft, const float fRight)
{
	CreateFuzzySet(((fMid +fRight)+fMid)/2);
	m_fPeakPoint = fMid;
	m_fLeftOffset = fLeft;
	m_fRightOffset = fRight;
}

//  *******************************************************************************************************************
float cFuzzySetRtShoulder::CalculateDOM(const float val)
{
	// test for the case where the left or right offsets are zero to prevent  divide by zero errors below
	if (((m_fRightOffset == 0.0) && (m_fPeakPoint == val))
		|| ((m_fLeftOffset == 0.0) && (m_fPeakPoint == val)))
	{
		return 1.0;
	}

	// find DOM if left of center
	if ((val <= m_fPeakPoint) && (val > (m_fPeakPoint - m_fLeftOffset)))
	{
		float grad = 1.0f / m_fLeftOffset;
		return (grad * (val - (m_fPeakPoint - m_fLeftOffset)));
	}

	// find DOM if right of center
	if ((val > m_fPeakPoint) && (val <= m_fPeakPoint+m_fRightOffset))
	{
		return 1.0;
	}

	// out of range of this FLV, return zero
	return 0.0;
}
