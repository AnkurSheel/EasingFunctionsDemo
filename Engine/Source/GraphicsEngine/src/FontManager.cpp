#include "stdafx.h"
#include "FontManager.h"
#include "Font.hxx"

using namespace Graphics;
using namespace Utilities;
using namespace Base;

IFontManager * cFontManager::s_pFontManager = NULL;

//  *******************************************************************************************************************
cFontManager::cFontManager()
{
}

//  *******************************************************************************************************************
cFontManager::~cFontManager()
{
	m_pFonts.clear();
}

//  *******************************************************************************************************************
shared_ptr<IMyFont> cFontManager::VGetFont(const Base::cString & strFontName)
{
	UINT64 hash = cHashedString::CalculateChecksum(strFontName);
	shared_ptr<IMyFont> ptr = Find(hash);

	if (ptr == NULL)
	{
		ptr = shared_ptr<IMyFont>(IMyFont::CreateMyFont());
		ptr->VInitialize(strFontName);
		m_pFonts[hash] = ptr;
	}
	else
	{
		SP_LOG(3, "Font already loaded: " + strFontName);
	}
	return ptr;
}

//  *******************************************************************************************************************
shared_ptr<IMyFont> cFontManager::Find(const UINT64 ulFontHash)
{
	auto itr = m_pFonts.find(ulFontHash);
	if (itr == m_pFonts.end())
	{
		return shared_ptr<IMyFont>();
	}

	return (*itr).second;
}

//  *******************************************************************************************************************
IFontManager * IFontManager::GetInstance()
{
	if (cFontManager::s_pFontManager == NULL)
		cFontManager::s_pFontManager = DEBUG_NEW cFontManager();
	return cFontManager::s_pFontManager;
}

//  *******************************************************************************************************************
void IFontManager::Destroy()
{
	SafeDelete(&cFontManager::s_pFontManager);
}
