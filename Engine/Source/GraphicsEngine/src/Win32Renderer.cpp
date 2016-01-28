#include "stdafx.h"
#include "Win32Renderer.h"
#include "Optional.h"
#include "GraphicStructures.h"

using namespace Graphics;
using namespace std;
using namespace Base;

class cWin32Renderer::cWin32RendererImpl
{
public:
  cWin32RendererImpl();
  ~cWin32RendererImpl();
  bool Initialize(const stInitializationParameters& params);
  void BeginRender();
  void EndRender();
  void SetFullScreenMode(const bool isFullScreen);
  Base::tOptional<bool> OnWindowResized(const int width, const int height);
  void Cleanup();

private:
  void ResizeDIBSection(const int width, const int height);
  void DrawRectangle(float minX, float maxX, float minY, float maxY, const cColor& color);

private:
  HWND m_windowHandle;
  BITMAPINFO m_BitmapInfo;
  void* m_pBackBuffer;
  int m_BitmapWidth;
  int m_BitmapHeight;
  const int m_BytesPerPixel;
};

std::unique_ptr<cWin32Renderer> cWin32Renderer::s_pWin32Renderer = nullptr;

//  *******************************************************************************************************************
cWin32Renderer::cWin32Renderer()
  : m_pImpl(DEBUG_NEW cWin32RendererImpl())
{
}

//  *******************************************************************************************************************
cWin32Renderer::~cWin32Renderer()
{
}

//  *******************************************************************************************************************
bool cWin32Renderer::Initialize(const stInitializationParameters& params)
{
  return m_pImpl->Initialize(params);
}

//  *******************************************************************************************************************
void cWin32Renderer::BeginRender()
{
  m_pImpl->BeginRender();
}

//  *******************************************************************************************************************
void cWin32Renderer::EndRender()
{
  m_pImpl->EndRender();
}

//  *******************************************************************************************************************
Base::tOptional<bool> cWin32Renderer::OnWindowResized(const int width, const int height)
{
  return m_pImpl->OnWindowResized(width, height);
}

//  *******************************************************************************************************************
void cWin32Renderer::SetFullScreenMode(const bool isFullScreen)
{
  m_pImpl->SetFullScreenMode(isFullScreen);
}

//  *******************************************************************************************************************
const unique_ptr<cWin32Renderer>& cWin32Renderer::Instance()
{
  if (s_pWin32Renderer == NULL)
  {
    s_pWin32Renderer = unique_ptr<cWin32Renderer>(DEBUG_NEW cWin32Renderer());
  }
  return s_pWin32Renderer;
}

//  *******************************************************************************************************************
void cWin32Renderer::Destroy()
{
  s_pWin32Renderer.reset();
}

//  *******************************************************************************************************************
cWin32Renderer::cWin32RendererImpl::cWin32RendererImpl()
  : m_pBackBuffer(nullptr)
  , m_BitmapWidth(0)
  , m_BitmapHeight(0)
  , m_BytesPerPixel(4)
{
}

//  *******************************************************************************************************************
cWin32Renderer::cWin32RendererImpl::~cWin32RendererImpl()
{
  Cleanup();
}

//  *******************************************************************************************************************
bool cWin32Renderer::cWin32RendererImpl::Initialize(const stInitializationParameters& params)
{
  m_windowHandle = params.windowHandle;
  ResizeDIBSection(params.width, params.height);
  return true;
}

//  *******************************************************************************************************************
Base::tOptional<bool> cWin32Renderer::cWin32RendererImpl::OnWindowResized(const int width, const int height)
{
  return true;
}

//  *******************************************************************************************************************
void cWin32Renderer::cWin32RendererImpl::BeginRender()
{
  DrawRectangle(0.0f, m_BitmapWidth, 0.0f, m_BitmapHeight, cColor::RED);
  DrawRectangle(10.0f, 100.0f, 10.0f, 100.0f, cColor::ORANGE);
}

//  *******************************************************************************************************************
void cWin32Renderer::cWin32RendererImpl::EndRender()
{
  RECT windowRect;
  GetClientRect(m_windowHandle, &windowRect);

  int windowWidth = windowRect.right - windowRect.left;
  int windowHeight = windowRect.bottom - windowRect.top;

  HDC deviceContext = GetDC(m_windowHandle);

  StretchDIBits(deviceContext, 0, 0, windowWidth, windowHeight, 0, 0, m_BitmapWidth, m_BitmapHeight, m_pBackBuffer,
                &m_BitmapInfo, DIB_RGB_COLORS, SRCCOPY);

  ReleaseDC(m_windowHandle, deviceContext);
}

//  *******************************************************************************************************************
void cWin32Renderer::cWin32RendererImpl::SetFullScreenMode(const bool isFullScreen)
{
}

//  *******************************************************************************************************************
void cWin32Renderer::cWin32RendererImpl::Cleanup()
{
}

//  *******************************************************************************************************************
void cWin32Renderer::cWin32RendererImpl::ResizeDIBSection(const int width, const int height)
{
  m_BitmapWidth = width;
  m_BitmapHeight = height;

  if (m_pBackBuffer != nullptr)
  {
    VirtualFree(m_pBackBuffer, 0, MEM_RELEASE);
  }
  ZeroMemory(&m_BitmapInfo, sizeof(m_BitmapInfo));
  m_BitmapInfo.bmiHeader.biSize = sizeof(m_BitmapInfo.bmiHeader);
  m_BitmapInfo.bmiHeader.biWidth = width;
  m_BitmapInfo.bmiHeader.biHeight = -height;  // top-left is 0,0
  m_BitmapInfo.bmiHeader.biPlanes = 1;
  m_BitmapInfo.bmiHeader.biBitCount = 32;
  m_BitmapInfo.bmiHeader.biCompression = BI_RGB;

  int bitmapMemorySize = m_BytesPerPixel * m_BitmapWidth * m_BitmapHeight;
  m_pBackBuffer = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

//  *******************************************************************************************************************
void cWin32Renderer::cWin32RendererImpl::DrawRectangle(float realMinX, float realMaxX, float realMinY, float realMaxY,
                                                       const cColor& color)
{
  int32 minX = RoundReal32ToInt32(realMinX);
  int32 maxX = RoundReal32ToInt32(realMaxX);
  int32 minY = RoundReal32ToInt32(realMinY);
  int32 maxY = RoundReal32ToInt32(realMaxY);

  if (minX < 0)
  {
    minX = 0;
  }
  if (maxX > m_BitmapWidth)
  {
    maxX = m_BitmapWidth;
  }
  if (minY < 0)
  {
    minY = 0;
  }
  if (maxY > m_BitmapHeight)
  {
    maxY = m_BitmapHeight;
  }

  int pitch = m_BitmapWidth * m_BytesPerPixel;
  uint8* pRow = reinterpret_cast<uint8*>(m_pBackBuffer) + minX * m_BytesPerPixel + minY * pitch;
  // LittleEndian = AA RR GG BB
  uint32 argb = color.GetAsARGB();
  for (int y = minY; y < maxY; ++y)
  {
    uint32* pPixel = reinterpret_cast<uint32*>(pRow);
    for (int x = minX; x < maxX; ++x)
    {
      *pPixel++ = argb;
    }
    pRow += pitch;
  }
}
