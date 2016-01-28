#include "stdafx.h"
#include "SpAssert.h"
#include "SpAssertHandler.h"
#include "SpAssertSettings.hxx"
#include "SpAssertContext.h"
#include "SpAssertLogger.hxx"
#include "SpAssertException.h"

using namespace Base;
using namespace std;

#undef SMART_ASSERT_A
#undef SMART_ASSERT_B
#undef SMART_ASSERT_OP

//  *******************************************************************************************************************
cSpAssert::cSpAssert(int level, const cString& expr, const bool onlyLog)
  : SMART_ASSERT_A(*this)
  , SMART_ASSERT_B(*this)
  , m_pContext(NULL)
  , m_bOnlyLog(onlyLog)
{
  const shared_ptr<ISpAssertSettings> pSettings =
      MakeStrongPtr<ISpAssertSettings>(cServiceLocator::GetInstance()->GetService<ISpAssertSettings>());
  if (pSettings == NULL || !pSettings->VGetIgnoreAll())
  {
    m_pContext = ISpAssertContext::Create();
    m_pContext->VSetExpression(expr);
    m_pContext->VSetAssertLevel(level);
  }
}

//  *******************************************************************************************************************
cSpAssert::cSpAssert(const cSpAssert& other)
  : SMART_ASSERT_A(*this)
  , SMART_ASSERT_B(*this)
  , m_pContext(other.m_pContext)
  , m_bOnlyLog(other.m_bOnlyLog)
{
}

//  *******************************************************************************************************************
cSpAssert::~cSpAssert()
{
  Handle();
  SafeDelete(&m_pContext);
}

//  *******************************************************************************************************************
void cSpAssert::Handle() const
{
  bool handled = false;
  const shared_ptr<ISpAssertSettings> pSettings =
      MakeStrongPtr<ISpAssertSettings>(cServiceLocator::GetInstance()->GetService<ISpAssertSettings>());
  if (pSettings != NULL)
  {
    const unique_ptr<ISpAssertLogger>& pLogger = pSettings->VGetLogger();
    if (pLogger != NULL)
    {
      pLogger->VLog(m_pContext);
    }

    if (m_bOnlyLog || pSettings->VGetIgnoreAll() || m_pContext->VGetAssertLevel() < AssertLevels::LEVEL_DEBUG || pSettings->VIsIgnored(m_pContext) )
    {
      handled = true;
    }
    else
    {
      const unique_ptr<cSpAssertHandler>& pHandler = pSettings->VGetHandler();
      if (pHandler != NULL)
      {
        pHandler->VHandleAssert(m_pContext);
        handled = true;
      }
    }
  }
  if (!handled)
  {
    DefaultHandle();
  }
}

//  *******************************************************************************************************************
cSpAssert cSpAssert::Make(int level, const cString& expr, const bool onlyLog)
{
  return cSpAssert(level, expr, onlyLog);
}

//  *******************************************************************************************************************
cSpAssert& cSpAssert::AddContext(const cString& key, const cString& value)
{
  if (m_pContext != NULL)
  {
    m_pContext->VAddContextValue(key, value);
  }
  return *this;
}

//  *******************************************************************************************************************
cSpAssert& cSpAssert::SetCustomMessage(const cString& message)
{
  if (m_pContext != NULL)
  {
    m_pContext->VSetCustomMessage(message);
  }
  return *this;
}

//  *******************************************************************************************************************
void cSpAssert::DefaultHandle() const
{
  if (m_pContext->VGetAssertLevel() >= AssertLevels::LEVEL_FATAL)
  {
    PostQuitMessage(0);
  }
  else if (m_pContext->VGetAssertLevel() >= AssertLevels::LEVEL_ERROR)
  {
    throw cSpAssertException(m_pContext);
  }
}
