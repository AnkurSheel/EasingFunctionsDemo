//  *******************************************************************************************************************
//  FuzzyOperators   version:  1.0   Ankur Sheel  date: 2015/05/19
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef FUZZYOPERATORS_H
#define FUZZYOPERATORS_H

#include "FuzzyTerm.h"
#include <vector>

class cFuzzySetProxy;
class cFuzzySet;

class cFuzzyOperatorVery :
	public cFuzzyTerm
{
public:
	virtual ~cFuzzyOperatorVery();
	cFuzzyOperatorVery(const cFuzzySetProxy& set);
	cFuzzyTerm* Clone();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Returns the DOM of the term
	///
  /// @return DOM of the term
	///
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	float GetDOM();
	void ClearDOM();
	void ORwithDOM(const float val);

protected:
	cFuzzyOperatorVery(const cFuzzyOperatorVery& instance);
	cFuzzyOperatorVery operator=(const cFuzzyOperatorVery& instance);

protected:
	cFuzzySet* m_Set;
};

class cFuzzyOperatorFairly :
	public cFuzzyTerm
{
public:
	virtual ~cFuzzyOperatorFairly();
	cFuzzyOperatorFairly(cFuzzySetProxy& set);
	cFuzzyTerm * Clone();
	float GetDOM();
	void ClearDOM();
	void ORwithDOM(const float val);

protected:
	cFuzzyOperatorFairly(const cFuzzyOperatorFairly& instance);
	cFuzzyOperatorFairly& operator=(const cFuzzyOperatorFairly& instance);

protected:
	cFuzzySet* m_Set;
};

class cFuzzyOperatorAnd :
	public cFuzzyTerm
{
public:
	cFuzzyOperatorAnd();
	virtual ~cFuzzyOperatorAnd();
	cFuzzyOperatorAnd(const cFuzzyOperatorAnd& instance);
	cFuzzyOperatorAnd(cFuzzyTerm& op1, cFuzzyTerm& op2);
	cFuzzyOperatorAnd(cFuzzyTerm& op1, cFuzzyTerm& op2, cFuzzyTerm& op3);
	cFuzzyOperatorAnd(cFuzzyTerm& op1, cFuzzyTerm& op2, cFuzzyTerm& op3
		, cFuzzyTerm& op4);
	cFuzzyTerm* Clone();
	float GetDOM();
	void ClearDOM();
	void ORwithDOM(const float val);

protected:
	cFuzzyOperatorAnd& operator=(const cFuzzyOperatorAnd& instance);

protected:
	std::vector<cFuzzyTerm *> m_Terms;
};

class cFuzzyOperatorOr :
	public cFuzzyTerm
{
public:
	virtual ~cFuzzyOperatorOr();
	cFuzzyOperatorOr(const cFuzzyOperatorOr& instance);
	cFuzzyOperatorOr(cFuzzyTerm& op1, cFuzzyTerm& op2);
	cFuzzyOperatorOr(cFuzzyTerm& op1, cFuzzyTerm& op2, cFuzzyTerm& op3);
	cFuzzyOperatorOr(cFuzzyTerm& op1, cFuzzyTerm& op2, cFuzzyTerm& op3
		, cFuzzyTerm& op4);
	cFuzzyTerm* Clone();
	float GetDOM();
	void ClearDOM();
	void ORwithDOM(const float val);

protected:
	cFuzzyOperatorOr& operator=(const cFuzzyOperatorOr& instance);

protected:
	std::vector<cFuzzyTerm *> m_Terms;
};
#endif  // FUZZYOPERATORS_H
