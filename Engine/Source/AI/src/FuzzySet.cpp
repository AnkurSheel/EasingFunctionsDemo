#include "stdafx.h"
#include "FuzzySet.h"
#include "AIDefines.h"
#include <assert.h>

//  *******************************************************************************************************************
cFuzzySet::cFuzzySet()
	: m_fDOM(0)
	, m_fRepresentativeValue(0)
{
}

//  *******************************************************************************************************************
cFuzzySet::cFuzzySet(const float fRepVal)
	: m_fDOM(0)
	, m_fRepresentativeValue(fRepVal)
{
}

//  *******************************************************************************************************************
cFuzzySet::~cFuzzySet()
{
}

//  *******************************************************************************************************************
void cFuzzySet::ORwithDOM(const float val)
{
	if (val >m_fDOM)
	{
		m_fDOM = val;
	}
}

//  *******************************************************************************************************************
float cFuzzySet::GetRepresentativeValue()
{
	return m_fRepresentativeValue;
}

//  *******************************************************************************************************************
void cFuzzySet::ClearDOM()
{
	m_fDOM = 0.0;
}

//  *******************************************************************************************************************
void cFuzzySet::CreateFuzzySet(const float val)
{
	m_fDOM = 0.0f;
	m_fRepresentativeValue = val;
}

//  *******************************************************************************************************************
void cFuzzySet::SetDOM(const float val)
{
#ifdef _DEBUG
	assert((val <=1) && (val >=0) && " cFuzzySet::SetDOM invalid value");
#endif  // _DEBUG
	m_fDOM= val;
}

//  *******************************************************************************************************************
float cFuzzySet::GetDOM()
{
	return m_fDOM;
}

//  *******************************************************************************************************************
cFuzzySet* cFuzzySet::GetAddress()
{
	return this;
}
