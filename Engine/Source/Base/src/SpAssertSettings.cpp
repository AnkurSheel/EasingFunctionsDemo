#include "stdafx.h"
#include "SpAssertSettings.h"
#include "DefaultConsoleHandler.h"
#include "SpAssertLogger.h"

using namespace Base;
using namespace std;

//  ********************************************************************************************************************
cSpAssertSettings::cSpAssertSettings()
  : m_bIgnoreAll(false)
  , m_pLogger(nullptr)
{
  SetDefaultLogger();
}

//  ********************************************************************************************************************
cSpAssertSettings::~cSpAssertSettings()
{
}

//  ********************************************************************************************************************
void cSpAssertSettings::VAddToIgnoreList(const ISpAssertContext* const pContext)
{
  m_IgnoredAsserts.insert(*(static_cast<const cSpAssertContext* const>(pContext)));
}

//  ********************************************************************************************************************
bool cSpAssertSettings::VIsIgnored(const ISpAssertContext* const pContext) const
{
  return m_IgnoredAsserts.find(*(static_cast<const cSpAssertContext*>(pContext))) != m_IgnoredAsserts.end();
}

//  ********************************************************************************************************************
void cSpAssertSettings::SetDefaultLogger()
{
  m_pLogger = unique_ptr<ISpAssertLogger>(DEBUG_NEW cSpAssertLogger());
  m_pLogger->VInitialize();
}

//  ********************************************************************************************************************
void ISpAssertSettings::CreateAsService()
{
  cServiceLocator::GetInstance()->Register<ISpAssertSettings>(
      shared_ptr<ISpAssertSettings>(DEBUG_NEW cSpAssertSettings()));
}
