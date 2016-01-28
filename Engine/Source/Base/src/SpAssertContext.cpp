#include "stdafx.h"
#include "SpAssertContext.h"
#include "Optional.h"

using namespace Base;

//  ********************************************************************************************************************
cSpAssertContext::cSpAssertContext()
  : m_DebugLevel(0)
{
}

//  ********************************************************************************************************************
cSpAssertContext::~cSpAssertContext()
{
  m_Variables.clear();
  m_ContextCollection.clear();
}

//  ********************************************************************************************************************
void cSpAssertContext::VAddContextValue(const cString& key, const cString& value)
{
  m_ContextCollection[key] = value;
}

//  ********************************************************************************************************************
const cString& cSpAssertContext::GetContextValue(const cString& key) const
{
  auto context = m_ContextCollection.find(key);
  if (context != m_ContextCollection.end())
  {
    return context->second;
  }
  return cString::EmptyString;
}

//  ********************************************************************************************************************
const cString& cSpAssertContext::VGetFileName() const
{
  return GetContextValue("file");
}

//  ********************************************************************************************************************
int cSpAssertContext::VGetLineNumber() const
{
  return GetContextValue("line").ToInt().GetValue();
}

//  ********************************************************************************************************************
const cString& cSpAssertContext::VGetFunctionName() const
{
  return GetContextValue("function");
}

//  ********************************************************************************************************************
int cSpAssertContext::VGetPriority() const
{
  cString priority = GetContextValue("priority");
  if (!priority.IsEmpty())
  {
    return priority.ToInt().GetValue();
  }
  return 0;
}

//  ********************************************************************************************************************
cString cSpAssertContext::VGetAssertLevelAsString() const
{
  switch (m_DebugLevel)
  {
    case AssertLevels::LEVEL_WARNING:
      return "Warning";

    case AssertLevels::LEVEL_DEBUG:
      return "Assertion failed";

    case AssertLevels::LEVEL_ERROR:
      return "Assertion failed (Error)";

    case AssertLevels::LEVEL_FATAL:
      return "Assertion failed (FATAL)";

    default:
      return cStringUtilities::MakeFormatted("Assertion failed (level = %d)", m_DebugLevel);
  };
}

//  ********************************************************************************************************************
ISpAssertContext* const ISpAssertContext::Create()
{
  return DEBUG_NEW cSpAssertContext();
}

//  ********************************************************************************************************************
ISpAssertContext* const ISpAssertContext::Duplicate(const ISpAssertContext* const pContext)
{
  return DEBUG_NEW cSpAssertContext(*(static_cast<const cSpAssertContext*>(pContext)));
}
