//  *******************************************************************************************************************
//  FuzzyRule   version:  1.0   Ankur Sheel  date: 2015/05/19
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef FUZZYRULE_H
#define FUZZYRULE_H

class cFuzzyTerm;

class cFuzzyRule
{
public:
	cFuzzyRule();
	virtual ~cFuzzyRule();
	cFuzzyRule(cFuzzyTerm* ante, cFuzzyTerm* conse);
	void SetConfidenceOfConsequenceToZero();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Updates the DOM of the consequent with the DOM of the Antecedent
	///
	///
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Calculate();

protected:
	cFuzzyTerm* m_pAntecedent;
	cFuzzyTerm* m_pConsequence;
	cFuzzyRule(const cFuzzyRule& instance);
	cFuzzyRule& operator=(const cFuzzyRule& instance);
};
#endif  // FUZZYRULE_H
