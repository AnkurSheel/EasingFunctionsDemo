//  *******************************************************************************************************************
//  SpAssertHandler_Win32   version:  1.0   Ankur Sheel  date: 2015/12/08
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef SPASSERTHANDLER_WIN32_H__
#define SPASSERTHANDLER_WIN32_H__

#include "SpAssertHandler.h"
namespace Base
{
  class cSpAssertHandler_Win32 : public cSpAssertHandler
  {
  public:
    BASE_API cSpAssertHandler_Win32();
    BASE_API ~cSpAssertHandler_Win32();

  protected:
    virtual AssertAction::AssertAction VDisplayMessageAndGetAction(const ISpAssertContext* const pContext) OVERRIDE;
    static int __stdcall StaticDialogBoxMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    int __stdcall DialogBoxMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void AddItems(HWND parentWindow);

  protected:
    const ISpAssertContext* m_pContext;
    HWND hBtnQuit;
    HWND hBtnIgnoreOnce;
    HWND hBtnIgnoreAll;
    HWND hBtnIgnoreForever;
    HWND hBtnIgnoreForeverPersistent;
    HWND hBtnBreakPoint;
  };
}
#endif  // SPASSERTHANDLER_WIN32_H__
