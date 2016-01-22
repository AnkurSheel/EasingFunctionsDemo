#include "stdafx.h"
#include "FuzzyRule.h"
#include "AIDefines.h"
#include "FuzzyTerm.h"

using namespace Base;

//  *******************************************************************************************************************
cFuzzyRule::cFuzzyRule()
	: m_pAntecedent(NULL)
	, m_pConsequence(NULL)
{
}

//  *******************************************************************************************************************
cFuzzyRule::~cFuzzyRule()
{
	SafeDelete(&m_pAntecedent);
	SafeDelete(&m_pConsequence);
}

//  *******************************************************************************************************************
cFuzzyRule::cFuzzyRule(cFuzzyTerm* ante, cFuzzyTerm* conse)
	: m_pAntecedent(ante->Clone())
	, m_pConsequence(conse->Clone())
{
}

//  *******************************************************************************************************************
void cFuzzyRule::SetConfidenceOfConsequenceToZero()
{
	m_pConsequence->ClearDOM();
}

//  *******************************************************************************************************************
void cFuzzyRule::Calculate()
{
	m_pConsequence->ORwithDOM(m_pAntecedent->GetDOM());
}
