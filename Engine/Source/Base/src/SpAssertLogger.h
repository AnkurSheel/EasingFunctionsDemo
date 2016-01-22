//  *******************************************************************************************************************
//  SpAssertLogger   version:  1.0   Ankur Sheel  date: 2014/11/03
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __SPASSERTLOGGER_H__
#define __SPASSERTLOGGER_H__

#include "SpAssertLogger.hxx"

namespace Base
{
  class cFileIO;
  class ISpAssertContext;
}

namespace Base
{
  class cSpAssertLogger : public ISpAssertLogger
  {
  public:
    cSpAssertLogger();
    ~cSpAssertLogger();

  private:
    void VInitialize() OVERRIDE;
    void VLog(const ISpAssertContext* const pContext) OVERRIDE;
    cString GetAssertLevelAsString(const int level);

  private:
    std::unique_ptr<cFileIO> m_pFile;
  };
}  // namespace Base
#endif  // __SPASSERTLOGGER_H__
