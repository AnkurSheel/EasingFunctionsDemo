//  *******************************************************************************************************************
//  SpAssertSettings   version:  1.0   Ankur Sheel  date: 2014/10/23
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __SPASSERTSETTINGS_HXX__
#define __SPASSERTSETTINGS_HXX__

#include "BaseDefines.h"
#include "SpAssertFwd.h"

namespace Base
{
  class ISpAssertLogger;
  class cAssertHandlerFactory;
}

namespace Base
{
  class ISpAssertSettings
  {
  public:
    virtual ~ISpAssertSettings() {}
    virtual bool VGetIgnoreAll() const = 0;
    virtual void VSetIgnoreAll(bool ignoreAll) = 0;
    virtual void VSetHandler(std::unique_ptr<cSpAssertHandler> pHandler) = 0;
    virtual void VSetLogger(std::unique_ptr<ISpAssertLogger> logger) = 0;
    virtual void VAddToIgnoreList(const ISpAssertContext* const pContext) = 0;
    virtual bool VIsIgnored(const ISpAssertContext* const pContext) const = 0;
    virtual const std::unique_ptr<cSpAssertHandler>& VGetHandler() const = 0;
    virtual const std::unique_ptr<ISpAssertLogger>& VGetLogger() const = 0;
    BASE_API static void CreateAsService();
  };
}  // namespace Base
#endif  // __SPASSERTSETTINGS_HXX__
