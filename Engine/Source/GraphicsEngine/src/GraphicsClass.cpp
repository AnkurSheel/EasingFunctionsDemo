//  *******************************************************************************************************************
//  GraphicsClass   version:  1.0   Ankur Sheel  date: 2012/09/13
//  *******************************************************************************************************************
//  purpose
//  *******************************************************************************************************************
#include "stdafx.h"
#include "GraphicsClass.h"
#include "DxBase.hxx"
#include "ShaderManager.hxx"
#include "TextureManager.hxx"
#include "Camera.hxx"
#include "GraphicUtils.h"
#include "FontManager.hxx"
#include "ObjModelLoader.hxx"
#include "CollisionChecker.hxx"
#include "Optional.h"
#include "UiManager.hxx"
#include "GraphicStructures.h"
#include "Win32Renderer.h"

using namespace Graphics;
using namespace Utilities;
using namespace Base;
using namespace std;

class cGraphics::cGraphicsImpl
{
public:
	cGraphicsImpl();
	~cGraphicsImpl();
	bool Initialize(const stInitializationParameters& params);
	void BeginRender();
	void EndRender();
	void SetFullScreenMode(bool bIsFullScreen);
	Base::tOptional<bool> OnWindowResized(int width, const int height);
	void Cleanup();
};

std::unique_ptr<cGraphics> cGraphics::s_pGraphic(nullptr);

//  *******************************************************************************************************************
cGraphics::cGraphics()
	: m_pImpl(DEBUG_NEW cGraphicsImpl())
{
}

//  *******************************************************************************************************************
cGraphics::~cGraphics()
{
}

//  *******************************************************************************************************************
bool cGraphics::Initialize(const stInitializationParameters& params)
{
	return m_pImpl->Initialize(params);
}

//  *******************************************************************************************************************
void cGraphics::BeginRender()
{
	m_pImpl->BeginRender();
}

//  *******************************************************************************************************************
void cGraphics::EndRender()
{
	m_pImpl->EndRender();
}

//  *******************************************************************************************************************
Base::tOptional<bool> cGraphics::OnWindowResized(int width, int height)
{
	return m_pImpl->OnWindowResized(width, height);
}

//  *******************************************************************************************************************
void cGraphics::SetFullScreenMode(bool isFullScreen)
{
	m_pImpl->SetFullScreenMode(isFullScreen);
}

//  *******************************************************************************************************************
cGraphics::cGraphicsImpl::cGraphicsImpl()
{
}

//  *******************************************************************************************************************
cGraphics::cGraphicsImpl::~cGraphicsImpl()
{
	Cleanup();
}

//  *******************************************************************************************************************
bool cGraphics::cGraphicsImpl::Initialize(const stInitializationParameters& params)
{
	IUiManager::CreateAsService();

#ifdef DIRECTX
	SP_ASSERT_FATAL(IDXBase::GetInstance() != NULL);
	if (IDXBase::GetInstance() != NULL)
	{
		return IDXBase::GetInstance()->VInitialize(params);
	}
#else
	cWin32Renderer::Instance()->Initialize(params);
#endif
	return true;
}

//  *******************************************************************************************************************
tOptional<bool> cGraphics::cGraphicsImpl::OnWindowResized(int width, int height)
{
#ifdef DIRECTX
	return IDXBase::GetInstance()->VOnWindowResized(width, height);
#else
	return cWin32Renderer::Instance()->OnWindowResized(width, height);
#endif
}

//  *******************************************************************************************************************
void cGraphics::cGraphicsImpl::SetFullScreenMode(bool bIsFullScreen)
{
#ifdef DIRECTX
	return IDXBase::GetInstance()->VSetFullScreenMode(bIsFullScreen);
#endif
}

//  *******************************************************************************************************************
void cGraphics::cGraphicsImpl::Cleanup()
{
	IFontManager::Destroy();
	IShaderManager::Destroy();
	ITextureManager::Destroy();
	IObjModelLoader::Destroy();
	ICollisionChecker::Destroy();
	IGraphicUtils::Destroy();
#ifdef DIRECTX
	IDXBase::Destroy();
#else
	cWin32Renderer::Destroy();
#endif
}

//  *******************************************************************************************************************
void cGraphics::cGraphicsImpl::BeginRender()
{
#ifdef DIRECTX
	IDXBase::GetInstance()->VBeginRender();
#else
	cWin32Renderer::Instance()->BeginRender();
#endif
}

//  *******************************************************************************************************************
void cGraphics::cGraphicsImpl::EndRender()
{
#ifdef DIRECTX
	IDXBase::GetInstance()->VEndRender();
#else
	cWin32Renderer::Instance()->EndRender();
#endif
}

//  *******************************************************************************************************************
const unique_ptr<cGraphics>& cGraphics::Instance()
{
	if (s_pGraphic == NULL)
	{
		s_pGraphic = unique_ptr<cGraphics>(DEBUG_NEW cGraphics());
	}
	return s_pGraphic;
}

//  *******************************************************************************************************************
void cGraphics::Destroy()
{
	s_pGraphic.reset();
}
