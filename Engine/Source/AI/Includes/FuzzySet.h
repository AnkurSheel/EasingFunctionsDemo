//  *******************************************************************************************************************
//  FuzzySet   version:  1.0   Ankur Sheel  date: 04/05/2009
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef FUZZYSET_H
#define FUZZYSET_H

class cFuzzySet
{
public:
  cFuzzySet();
  virtual ~cFuzzySet();
  virtual float CalculateDOM(const float val) { return 0; }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// If this fuzzy set is part of a consequent FLV, and it is fired by a rule then this method sets the DOM to the
  /// maximum of the parameter value or the set's  existing m_fDOM value
  ///
  /// @param[in] val
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  void ORwithDOM(const float val);
  float GetRepresentativeValue();
  void ClearDOM();
  void CreateFuzzySet(const float val);
  void SetDOM(const float val);
  cFuzzySet(const float fRepVal);
  float GetDOM();
  cFuzzySet* GetAddress();

protected:
  float m_fDOM;
  float m_fRepresentativeValue;
};
#endif  // FUZZYSET_H
