#include "stdafx.h"
#include "FuzzyOperators.h"
#include "AIDefines.h"
#include "FuzzySetProxy.h"
#include "FuzzySet.h"
#include <math.h>

using namespace Base;

//  *******************************************************************************************************************
cFuzzyOperatorVery::~cFuzzyOperatorVery()
{
}

//  *******************************************************************************************************************
cFuzzyOperatorVery::cFuzzyOperatorVery(const cFuzzyOperatorVery& instance)
	: m_Set(instance.m_Set)
{
}

//  *******************************************************************************************************************
cFuzzyOperatorVery::cFuzzyOperatorVery(const cFuzzySetProxy& set)
	: m_Set(set.m_Set)
{
}

//  *******************************************************************************************************************
cFuzzyTerm* cFuzzyOperatorVery::Clone()
{
	return(DEBUG_NEW cFuzzyOperatorVery(*this));
}

//  *******************************************************************************************************************
float cFuzzyOperatorVery::GetDOM()
{
	float temp = m_Set->GetDOM();
	return (temp * temp);
}

//  *******************************************************************************************************************
void cFuzzyOperatorVery::ClearDOM()
{
	m_Set->ClearDOM();
}

//  *******************************************************************************************************************
void cFuzzyOperatorVery::ORwithDOM(const float val)
{
	m_Set->ORwithDOM(val * val);
}

//  *******************************************************************************************************************
cFuzzyOperatorFairly::~cFuzzyOperatorFairly()
{
}

cFuzzyOperatorFairly::cFuzzyOperatorFairly(const cFuzzyOperatorFairly& instance)
	: m_Set(instance.m_Set)
{
}

//  *******************************************************************************************************************
cFuzzyOperatorFairly::cFuzzyOperatorFairly(cFuzzySetProxy& set)
	: m_Set(set.m_Set)
{
}

//  *******************************************************************************************************************
cFuzzyTerm * cFuzzyOperatorFairly::Clone()
{
	return(DEBUG_NEW cFuzzyOperatorFairly(*this));
}

//  *******************************************************************************************************************
float cFuzzyOperatorFairly::GetDOM()
{
	return (sqrt(m_Set->GetDOM()));
}

//  *******************************************************************************************************************
void cFuzzyOperatorFairly::ClearDOM()
{
	m_Set->ClearDOM();
}

//  *******************************************************************************************************************
void cFuzzyOperatorFairly::ORwithDOM(const float val)
{
	m_Set->ORwithDOM(sqrt(val));
}

//  *******************************************************************************************************************
cFuzzyOperatorAnd::cFuzzyOperatorAnd()
{
}

//  *******************************************************************************************************************
cFuzzyOperatorAnd::~cFuzzyOperatorAnd()
{
	for(auto curTerm = m_Terms.begin(); curTerm != m_Terms.end(); curTerm++)
	{
		SafeDelete(&(*curTerm));
	}
}

//  *******************************************************************************************************************
cFuzzyOperatorAnd::cFuzzyOperatorAnd(const cFuzzyOperatorAnd& instance)
{
	for(auto curTerm = instance.m_Terms.begin(); curTerm != instance.m_Terms.end(); curTerm++)
	{
		m_Terms.push_back((*curTerm)->Clone());
	}
}

//  *******************************************************************************************************************
cFuzzyOperatorAnd::cFuzzyOperatorAnd(cFuzzyTerm& op1, cFuzzyTerm& op2)
{
	m_Terms.push_back(op1.Clone());
	m_Terms.push_back(op2.Clone());
}

//  *******************************************************************************************************************
cFuzzyOperatorAnd::cFuzzyOperatorAnd(cFuzzyTerm& op1, cFuzzyTerm& op2, cFuzzyTerm& op3)
{
	m_Terms.push_back(op1.Clone());
	m_Terms.push_back(op2.Clone());
	m_Terms.push_back(op3.Clone());
}

//  *******************************************************************************************************************
cFuzzyOperatorAnd::cFuzzyOperatorAnd(cFuzzyTerm& op1, cFuzzyTerm& op2, cFuzzyTerm& op3, cFuzzyTerm& op4)
{
	m_Terms.push_back(op1.Clone());
	m_Terms.push_back(op2.Clone());
	m_Terms.push_back(op3.Clone());
	m_Terms.push_back(op4.Clone());
}

//  *******************************************************************************************************************
cFuzzyTerm* cFuzzyOperatorAnd::Clone()
{
	return(DEBUG_NEW cFuzzyOperatorAnd(*this));
}

//  *******************************************************************************************************************
float cFuzzyOperatorAnd::GetDOM()
{
	float smallest = 1000000.0f;

	for(auto curTerm = m_Terms.begin(); curTerm != m_Terms.end(); curTerm++)
	{
		float temp = (*curTerm)->GetDOM();
		if (temp < smallest)
		{
			smallest = temp;
		}
	}
	return smallest;
}

//  *******************************************************************************************************************
void cFuzzyOperatorAnd::ClearDOM()
{
	for(auto curTerm = m_Terms.begin(); curTerm != m_Terms.end(); curTerm++)
	{
		(*curTerm)->ClearDOM();
	}
}

//  *******************************************************************************************************************
void cFuzzyOperatorAnd::ORwithDOM(const float val)
{
	for(auto curTerm = m_Terms.begin(); curTerm != m_Terms.end(); curTerm++)
	{
		(*curTerm)->ORwithDOM(val);
	}
}

//  *******************************************************************************************************************
cFuzzyOperatorOr::~cFuzzyOperatorOr()
{
	for(auto curTerm = m_Terms.begin(); curTerm != m_Terms.end(); curTerm++)
	{
		SafeDelete(&(*curTerm));
	}
}

//  *******************************************************************************************************************
cFuzzyOperatorOr::cFuzzyOperatorOr(const cFuzzyOperatorOr& instance)
{
	for (auto curTerm = instance.m_Terms.begin(); curTerm != instance.m_Terms.end(); ++curTerm)
	{
		m_Terms.push_back((*curTerm)->Clone());
	}
}

//  *******************************************************************************************************************
cFuzzyOperatorOr::cFuzzyOperatorOr(cFuzzyTerm& op1, cFuzzyTerm& op2)
{
	m_Terms.push_back(op1.Clone());
	m_Terms.push_back(op2.Clone());
}

//  *******************************************************************************************************************
cFuzzyOperatorOr::cFuzzyOperatorOr(cFuzzyTerm& op1, cFuzzyTerm& op2, cFuzzyTerm& op3)
{
	m_Terms.push_back(op1.Clone());
	m_Terms.push_back(op2.Clone());
	m_Terms.push_back(op3.Clone());
}

//  *******************************************************************************************************************
cFuzzyOperatorOr::cFuzzyOperatorOr(cFuzzyTerm& op1, cFuzzyTerm& op2, cFuzzyTerm& op3, cFuzzyTerm& op4)
{
	m_Terms.push_back(op1.Clone());
	m_Terms.push_back(op2.Clone());
	m_Terms.push_back(op3.Clone());
	m_Terms.push_back(op4.Clone());
}

//  *******************************************************************************************************************
cFuzzyTerm* cFuzzyOperatorOr::Clone()
{
	return (DEBUG_NEW cFuzzyOperatorOr(*this));
}

//  *******************************************************************************************************************
float cFuzzyOperatorOr::GetDOM()
{
	float largest = -1.0f;

	for(auto curTerm = m_Terms.begin(); curTerm != m_Terms.end(); curTerm++)
	{
		float temp = (*curTerm)->GetDOM();
		if (temp > largest)
		{
			largest = temp;
		}
	}
	return largest;
}

//  *******************************************************************************************************************
void cFuzzyOperatorOr::ClearDOM()
{
	_ASSERT(0);
}

//  *******************************************************************************************************************
void cFuzzyOperatorOr::ORwithDOM(const float val)
{
	_ASSERT(0);
}
