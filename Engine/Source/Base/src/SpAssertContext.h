//  *******************************************************************************************************************
//  SpAssertContext   version:  1.0   Ankur Sheel  date: 2014/10/10
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __SPASSERTCONTEXT_H__
#define __SPASSERTCONTEXT_H__

#include <map>
#include "SpAssertContext.hxx"

namespace Base
{
  class cSpAssertContext : public ISpAssertContext
  {
    typedef std::map<cString, cString> ContextMap;

  public:
    ~cSpAssertContext();
    void VAddContextValue(const cString& key, const cString& value) OVERRIDE;
    const cString& VGetFileName() const OVERRIDE;
    const cString& VGetFunctionName() const OVERRIDE;
    int VGetLineNumber() const OVERRIDE;
    int VGetPriority() const OVERRIDE;
    cString VGetAssertLevelAsString() const OVERRIDE;
    const VariablesArray& VGetVariablesArray() const OVERRIDE { return m_Variables; }
    const cString& VGetCustomMessage() const OVERRIDE { return m_CustomMessage; }
    void VSetCustomMessage(cString message) OVERRIDE { m_CustomMessage = message; }
    int VGetAssertLevel() const OVERRIDE { return m_DebugLevel; }
    void VSetAssertLevel(int level) OVERRIDE { m_DebugLevel = level; }
    const Base::cString& VGetExpression() const OVERRIDE { return m_Expression; }
    void VSetExpression(const cString& expr) OVERRIDE { m_Expression = expr; }
    void VAddVariableValues(const cString& variableName, const cString& variableValue) OVERRIDE
    {
      m_Variables.push_back(stVariables(variableName, variableValue));
    }

  private:
    cSpAssertContext();
    const cString& GetContextValue(const cString& key) const;

  private:
    ContextMap m_ContextCollection;
    VariablesArray m_Variables;
    cString m_Expression;
    cString m_CustomMessage;
    int m_DebugLevel;

    friend static ISpAssertContext* const ISpAssertContext::Create();
    friend static ISpAssertContext* const Duplicate(const ISpAssertContext* const pContext);
  };

  inline bool operator<(const cSpAssertContext& first, const cSpAssertContext& second)
  {
    int n = first.VGetFileName().Compare(second.VGetFileName());
    if (n != 0)
    {
      return n < 0;
    }
    else
    {
      return first.VGetLineNumber() < second.VGetLineNumber();
    }
  }
}  // namespace Base
#endif  // __SPASSERTCONTEXT_H__
