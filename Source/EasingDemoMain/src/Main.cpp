#include "stdafx.h"
#include "Main.h"
#include "BaseApp.hxx"
#include "Game.hxx"
#include "vld.h"

using namespace GameBase;
using namespace Base;

static IBaseApp* pGame = NULL;

//// ***************************************************************
//-// Checks for memory leaks
//  -// ***************************************************************
//  -void CheckForMemoryLeaks()
//  -{
//    -#ifdef	_DEBUG
//      -	// Get Current flag
//      -	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) ;
//    -
//      -	// Turn on leak-checking bit
//      -	flag |= (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF) ;
//    -
//      -	// Set flag to the new value
//      -	_CrtSetDbgFlag(flag) ;
//    -#endif	_DEBUG
//      -}

// ***************************************************************
// Main function
// ***************************************************************
int WINAPI WinMain(const HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  pGame = IGame::CreateGame("Game");

  pGame->VOnInitialization(hInstance, nCmdShow, "Options.ini");
  pGame->VRun();
  Cleanup();

  cServiceLocator::Destroy();
  // _CrtDumpMemoryLeaks()
  return 0;
}

// ***************************************************************
void Cleanup()
{
  SafeDelete(&pGame);
}
