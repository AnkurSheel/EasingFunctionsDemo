#include "stdafx.h"
#include "Texture.h"
#include "ResourceManager.hxx"
#include "ResCache.hxx"
#include "DxBase.hxx"
#include <D3DX11tex.h>

using namespace Base;
using namespace Graphics;
using namespace Utilities;

//  *******************************************************************************************************************
cTexture::cTexture()
	: m_pTexture(NULL)
{
}

//  *******************************************************************************************************************
cTexture::~cTexture()
{
	Cleanup();
}

//  *******************************************************************************************************************
bool cTexture::Initialize(const cString& texturePath)
{
	m_TexturePath = texturePath;

	SafeRelease(&m_pTexture);
	shared_ptr<IResource> pResource = shared_ptr<IResource>(IResource::CreateResource(texturePath));
	shared_ptr<IResHandle> texture = IResourceManager::GetInstance()->VGetResourceCache()->GetHandle(*pResource);

	if (texture.get() != NULL)
	{
		SP_LOG(3, "Could not find in cache : " + texturePath);

		// Create the texture associated with this sprite
		HRESULT result = D3DX11CreateShaderResourceViewFromMemory(IDXBase::GetInstance()->VGetDevice(),
			texture->GetBuffer(), texture->GetSize(), NULL, NULL, &m_pTexture, NULL);
		if (FAILED(result))
		{
			result = D3DX11CreateShaderResourceViewFromFile(IDXBase::GetInstance()->VGetDevice(),
				texturePath.GetData(), NULL, NULL, &m_pTexture, NULL);
			if (FAILED(result))
			{
				SP_ASSERT_ERROR(false)(texturePath)(DXGetErrorString(result))(DXGetErrorDescription(result)).SetCustomMessage("Texture Creation failed");
				return false;
			}
		}
		return true;
	}
	return false;
}

//  *******************************************************************************************************************
void cTexture::Cleanup()
{
	SafeRelease(&m_pTexture);
}

//  *******************************************************************************************************************
shared_ptr<ITexture> ITexture::CreateTexture(const Base::cString & strTexturePath)
{
	shared_ptr<cTexture> ptr(DEBUG_NEW cTexture());
	if (!ptr->Initialize(strTexturePath))
	{
		return shared_ptr<ITexture>();
	}
	return ptr;
}
