#include "stdafx.h"
#include "FuzzySetProxy.h"
#include "FuzzySet.h"

//  *******************************************************************************************************************
cFuzzySetProxy::cFuzzySetProxy()
{
}

//  *******************************************************************************************************************
cFuzzySetProxy::cFuzzySetProxy(cFuzzySet& fs)
	: m_Set(fs.GetAddress())
{
}

//  *******************************************************************************************************************
cFuzzySetProxy::~cFuzzySetProxy()
{
}

//  *******************************************************************************************************************
cFuzzyTerm* cFuzzySetProxy::Clone()
{
	cFuzzyTerm *temp = DEBUG_NEW cFuzzySetProxy(*this);

	// cFuzzySet *a = temp->GetAddress();
	// cFuzzySet *b = this->GetAddress();
	return temp;
	// return (new cFuzzySetProxy(*this));
}

//  *******************************************************************************************************************
float cFuzzySetProxy::GetDOM()
{
	return m_Set->GetDOM();
}

//  *******************************************************************************************************************
void cFuzzySetProxy::ClearDOM()
{
	m_Set->ClearDOM();
}

//  *******************************************************************************************************************
void cFuzzySetProxy::ORwithDOM(const float val)
{
	m_Set->ORwithDOM(val);
}
