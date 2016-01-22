//  *******************************************************************************************************************
//  SpAssertSettings   version:  1.0   Ankur Sheel  date: 2014/10/16
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __SPASSERTSETTINGS_H__
#define __SPASSERTSETTINGS_H__

#include "SpAssertContext.h"
#include "SpAssertSettings.hxx"

namespace Base
{
	class cSpAssertHandler;
}

namespace Base
{
	class cSpAssertSettings
		: public ISpAssertSettings
	{
		typedef std::map< int, cSpAssertHandlerStrongPtr> AssertHandlerMap;
		typedef std::set<cSpAssertContext, std::less<cSpAssertContext> > ContextSet;

	public:
		~cSpAssertSettings();

		bool VGetIgnoreAll() const OVERRIDE { return m_bIgnoreAll; }
		void VSetIgnoreAll(bool ignoreAll) OVERRIDE { m_bIgnoreAll = ignoreAll; }
		cSpAssertHandlerStrongPtr VGetHandler(int level) const OVERRIDE;
		void VSetHandler(int level, cSpAssertHandlerStrongPtr handler) OVERRIDE;
		void VAddToIgnoreList(const ISpAssertContext * const pContext) OVERRIDE;
		bool VIsIgnored(const ISpAssertContext * const pContext) const OVERRIDE;

		const std::unique_ptr<ISpAssertLogger>& VGetLogger() const OVERRIDE { return m_pLogger; }
		void VSetLogger(std::unique_ptr<ISpAssertLogger> pLogger) OVERRIDE { m_pLogger = std::move(pLogger); }

	private:
		cSpAssertSettings();
		void SetDefaultLogger();

	private:
		AssertHandlerMap m_handlers;
		ContextSet m_IgnoredAsserts;
		std::unique_ptr<ISpAssertLogger> m_pLogger;
		bool m_bIgnoreAll;

		friend void ISpAssertSettings::CreateAsService();
	};
}  // namespace Base
#endif  // __SPASSERTSETTINGS_H__
