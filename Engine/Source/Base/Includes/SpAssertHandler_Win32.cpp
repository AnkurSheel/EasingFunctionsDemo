#include "stdafx.h"
#include "SpAssertHandler_Win32.h"

using namespace Base;
static HINSTANCE m_hInstRichEdit20 = NULL;

int __stdcall DialogBoxMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//  *******************************************************************************************************************
cSpAssertHandler_Win32::cSpAssertHandler_Win32()
  : m_pContext(nullptr)
  , hBtnQuit(nullptr)
  , hBtnIgnoreOnce(nullptr)
  , hBtnIgnoreAll(nullptr)
  , hBtnIgnoreForever(nullptr)
  , hBtnBreakPoint(nullptr)
  , hBtnIgnoreForeverPersistent(nullptr)
{
}

//  *******************************************************************************************************************
cSpAssertHandler_Win32::~cSpAssertHandler_Win32()
{
}

//  *******************************************************************************************************************
AssertAction::AssertAction cSpAssertHandler_Win32::VDisplayMessageAndGetAction(const ISpAssertContext* const pContext)
{
  HINSTANCE hInstance = GetModuleHandle(NULL);
  HWND hwnd = CreateWindowEx(0, "Message", "Assert Window", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);

  if (m_hInstRichEdit20 == NULL)
  {
    m_hInstRichEdit20 = LoadLibraryA("riched20.dll");
  }

  typedef struct
  {
    DLGTEMPLATE dlt;  //  In a standard template for a dialog box, the DLGTEMPLATE structure is always immediately
                      //  followed by three variable-length arrays that specify the menu, class, and title for the
    //  dialog box. When the DS_SETFONT style is specified, these arrays are also followed by a 16-bit value specifying
    //  point size
    // and another variable-length array specifying a typeface name.
    // Each array consists of one or more 16-bit elements. The menu, class, title, and font arrays must be aligned on
    // WORD boundaries.
    WORD m;  // menu
    WORD c;  // class
    WCHAR title[8];
    // The font data begins on the WORD boundary that follows the title array.
    // The font data specifies a 16-bit point size value and a Unicode font name string. If possible, the system creates
    // a font according to the specified values. Then the system sends a WM_SETFONT message to the dialog box and to
    // each control to provide a handle to the font. If DS_SETFONT is not specified, the dialog box template does not
    // include the font data.
    WORD pt;  // 16-bit value specifying point size
    WCHAR font[6];
  } TDlgData;

  TDlgData dtp = {{DS_3DLOOK | DS_SETFONT | DS_CENTER | DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_VISIBLE | WS_SYSMENU,
                   0, 0, 0, 0, 500, 100},
                  0,
                  0,
                  L"NoName",
                  8,
                  L"Arial"};

  BOOL isValidHwnd = IsWindow(hwnd);
  if (isValidHwnd == FALSE)
  {
    SP_LOG(1, "Invalid hwnd")(hwnd);
    hwnd = NULL;
  }

  m_pContext = pContext;

  ShowCursor(true);
  DLGPROC dialogProc = (DLGPROC)cSpAssertHandler_Win32::StaticDialogBoxMsgProc;

  DLGTEMPLATE* dlg = reinterpret_cast<DLGTEMPLATE*>(&dtp);
  AssertAction::AssertAction action =
      (AssertAction::AssertAction)DialogBoxIndirectParam(hInstance, dlg, hwnd, dialogProc, LPARAM(this));
  ShowCursor(false);
  DestroyWindow(hwnd);

  return action;
}

int __stdcall cSpAssertHandler_Win32::StaticDialogBoxMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  if (msg == WM_INITDIALOG)
  {
    cSpAssertHandler_Win32* pDialog = reinterpret_cast<cSpAssertHandler_Win32*>(lParam);
    SetWindowLongPtr(hWnd, DWLP_USER, reinterpret_cast<LONG_PTR>(pDialog));
    pDialog->AddItems(hWnd);
    char* fullCmdLine = GetCommandLineA();
    SetWindowTextA(hWnd, cStringUtilities::MakeFormatted("%s - An error has occurred...", fullCmdLine).GetData());

    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    SetForegroundWindow(hWnd);
    SetActiveWindow(hWnd);
  }
  else
  {
    cSpAssertHandler_Win32* pDialog = reinterpret_cast<cSpAssertHandler_Win32*>(GetWindowLongPtr(hWnd, DWLP_USER));
    if (pDialog)
    {
      return pDialog->DialogBoxMsgProc(hWnd, msg, wParam, lParam);
    }
  }
  return FALSE;
}

//  *******************************************************************************************************************
int __stdcall cSpAssertHandler_Win32::DialogBoxMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
    case WM_COMMAND:
      if (HIWORD(wParam) == BN_CLICKED && (HWND)lParam)
      {
        HWND hBut = (HWND)lParam;
        AssertAction::AssertAction action = AssertAction::None;

        if (hBut == hBtnQuit)
        {
         action =  AssertAction::Abort;
        }
        else if (hBut == hBtnIgnoreOnce)
        {
          action = AssertAction::Ignore;
        }
        else if (hBut == hBtnIgnoreAll)
        {
          action = AssertAction::IgnoreAll;
        }
        else if (hBut == hBtnIgnoreForever)
        {
          action = AssertAction::IgnoreForever;
        }
        else if (hBut == hBtnIgnoreForeverPersistent)
        {
          action = AssertAction::IgnoreForeverPersistent;
        }
        else if (hBut == hBtnBreakPoint)
        {
          action = AssertAction::Break;
        }
        EndDialog(hWnd, action);
      }
      else
      {
        switch (LOWORD(wParam))
        {
          case IDCANCEL:
          {
            EndDialog(hWnd, AssertAction::Abort);
            return TRUE;
          }
        }
        break;
      }
  }
  return FALSE;
}

//  *******************************************************************************************************************
void cSpAssertHandler_Win32::AddItems(HWND parentWindow)
{
  HINSTANCE instance = GetModuleHandle(NULL);

  int x = 10;
  int y = 130;
  int w = 90;
  int h = 20;

  int assertLevel = m_pContext->VGetAssertLevel();
  if (assertLevel >= AssertLevels::LEVEL_DEBUG)
  {
    hBtnQuit = CreateWindowA("BUTTON", "Abort", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, x, y, w, h,
                             parentWindow, NULL, instance, NULL);
    x += 100;
  }

  if (assertLevel < AssertLevels::LEVEL_FATAL)
  {
    hBtnIgnoreOnce = CreateWindowA("BUTTON", "Ignore Once", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, x, y, w,
                                   h, parentWindow, NULL, instance, NULL);
    x += 100;

    hBtnIgnoreAll = CreateWindowA("BUTTON", "Ignore All", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, x, y, w,
      h, parentWindow, NULL, instance, NULL);
    x += 100;

    //hBtnIgnoreForever = CreateWindowA("BUTTON", "Ignore Forever", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, x, y, w,
    //  h, parentWindow, NULL, instance, NULL);
    //x += 100;

    //hBtnIgnoreForeverPersistent = CreateWindowA("BUTTON", "Ignore Forever Persistent", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, x, y, w,
    //  h, parentWindow, NULL, instance, NULL);
    //x += 100;
  }
  hBtnBreakPoint = CreateWindowA("BUTTON", "Break", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, x, y, w,
    h, parentWindow, NULL, instance, NULL);
  x += 100;

  x = 10;
  y = 30;
  w = 730;
  h = 80;

  cString strChooseMsg;
  strChooseMsg = m_pContext->VGetAssertLevelAsString();
  strChooseMsg += " [ " + m_pContext->VGetExpression();
  strChooseMsg += "] \nin : \nfile: " + m_pContext->VGetFileName();
  strChooseMsg += cStringUtilities::MakeFormatted(" : line: %d\n", m_pContext->VGetLineNumber());
  strChooseMsg += "function " + m_pContext->VGetFunctionName() + "\n";
  if (!m_pContext->VGetCustomMessage().IsEmpty())
  {
    strChooseMsg += "Message : " + m_pContext->VGetCustomMessage();
  }
  if (m_pContext->VGetVariablesArray().size() > 0)
  {
    strChooseMsg += "\nValues:\n";

    for (auto iter = m_pContext->VGetVariablesArray().begin(); iter != m_pContext->VGetVariablesArray().end(); ++iter)
    {
      strChooseMsg += iter->m_VariableName + " = " + iter->m_VariableValue + "\n";
    }
  }
  strChooseMsg += "\n";

  HWND hwndRichedit =
      CreateWindowExA(WS_EX_CLIENTEDGE, "RichEdit20A", "",
                      ES_LEFT | ES_MULTILINE | ES_READONLY | WS_VSCROLL | WS_CHILD | ES_AUTOVSCROLL | WS_VISIBLE, x, y,
                      w, h, parentWindow, NULL, instance, NULL);

  if (hwndRichedit)
  {
    SetWindowTextA(hwndRichedit, strChooseMsg.GetData());
    SendMessage(hwndRichedit, EM_SCROLLCARET, 0, 0);
    HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    SendMessage(hwndRichedit, WM_SETFONT, (WPARAM)hFont, TRUE);
  }
  //else  // if(hwndRichedit == NULL)	//because dll cound not be loaded ?
  //{
  //  hTextMessage = CreateWindowA("STATIC", gTempAssertText, SS_LEFT | WS_THICKFRAME | WS_CHILD | WS_VISIBLE, x, y, w, h,
  //                               hWnd, NULL, instance, NULL);
  //  RKErrorDial_SetFont(hTextMessage);
  //}
}
