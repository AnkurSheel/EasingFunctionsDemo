#include "stdafx.h"
#include "BaseApp.h"
#include "MainWindow.hxx"
#include "Timer.h"
#include "HumanView.h"
#include "ParamLoaders.hxx"
#include "Optional.h"
#include "SystemChecker.hxx"
#include "EntityManager.hxx"
#include "MessageDispatchManager.hxx"
#include "ResourceManager.hxx"
#include "GraphicsClass.h"
#include "ProcessManager.hxx"
#include "GameDirectories.h"
#include "GameOptions.h"
#include "EventManager.hxx"
#include "GameControls.h"
#include "Color.h"
#include "ComponentFactory.h"
#include "EventFactory.h"
#include "SpAssertSettings.hxx"
#include "Logger.hxx"
#include "Physics.hxx"
#include "Config.h"
#include "TweenManager.h"
#include "RandomGenerator.hxx"
#include "GraphicStructures.h"
#include "Audio.hxx"
#include "SpAssertHandler_Win32.h"

using namespace GameBase;
using namespace Base;
using namespace Utilities;
using namespace Graphics;
using namespace Physics;
using namespace std;
using namespace Sound;

IBaseApp * cBaseApp::m_spInstance = NULL;

//  *******************************************************************************************************************
cBaseApp::cBaseApp(const cString & Name)
	: m_pGameTimer(nullptr)
	, m_pHumanView(NULL)
	, m_pParamLoader(NULL)
	, m_pHighScoreTable(NULL)
	, m_pGameControls(NULL)
	, m_Quitting(false)
	, m_pProcessManager(NULL)
	, m_pConfig(NULL)
	, m_IsEditorRunning(false)
{
	// make sure our memory leak checker is working
#if _DEBUG
	// cppcheck-suppress unusedAllocatedMemory
	int * p = DEBUG_NEW int();
#endif

	if (m_spInstance == NULL)
	{
		m_spInstance = this;
	}
	// cppcheck-suppress memleak
}

//  ********************************************************************************************************************
cBaseApp::~cBaseApp()
{
	VCleanup();
}

//  *******************************************************************************************************************
void cBaseApp::VOnInitialization(const HINSTANCE& hInstance, const int inCmdShow, const cString & inOptionsFile)
{
	RegisterServices();

	SP_ASSERT(m_pParamLoader == NULL).SetCustomMessage("Param Loader should be NULL");
	if (m_pParamLoader == NULL)
	{
		m_pParamLoader = IParamLoader::CreateParamLoader();
		m_pParamLoader->VLoadParametersFromFile(inOptionsFile);
	}

	unique_ptr<ILogger> pLogger(ILogger::CreateLogger());
	pLogger->VInitialize();
	SetLoggerOptions(pLogger);
	const shared_ptr<ISpAssertSettings> pAssertSettings = MakeStrongPtr<ISpAssertSettings>(cServiceLocator::GetInstance()->GetService<ISpAssertSettings>());
	if (pAssertSettings != nullptr)
	{
		pAssertSettings->VSetLogger(std::move(pLogger));
	}

  unique_ptr<cSpAssertHandler> pHandler(DEBUG_NEW cSpAssertHandler_Win32());
  if (pAssertSettings != nullptr)
  {
    pAssertSettings->VSetHandler(std::move(pHandler));
  }

	m_Title = m_pParamLoader->VGetParameterValueAsString("-title", "Game");

	if (!MeetsMinimumSystemRequirements())
	{
		OnInitializationError();
		return;
	}


	// initialize resource manager
	cString assetsPath = m_pParamLoader->VGetParameterValueAsString("-AssetsPath", "");
	cString resourceFile = m_pParamLoader->VGetParameterValueAsString("-ResourceFile", "");

	if (!InitializeResourceManager(assetsPath, resourceFile))
	{
		OnInitializationError();
		return;
	}

	cGameDirectories::Initialize(assetsPath + resourceFile);
	VInitializeGameOptions();

	VCreateAndSetEntityFactory();
	VCreateAndSetComponentFactory();
	VCreateAndSetEventFactory();
	VCreateHumanView();

	HWND windowHandle = VInitializeApplicationWindow(hInstance, inCmdShow);
	if (windowHandle == NULL)
	{
		OnInitializationError();
		return;
	}

	if (!VInitializeGraphics(windowHandle))
	{
		OnInitializationError();
		return;
	}

	m_pProcessManager = IProcessManager::CreateProcessManager();
	m_pHumanView->VOnCreateDevice(this);

	shared_ptr<IAudio> pAudioService = MakeStrongPtr(cServiceLocator::GetInstance()->GetService<IAudio>());
	if (pAudioService != nullptr)
	{
		pAudioService->VInitialize(windowHandle);
	}
}

//  *******************************************************************************************************************
void cBaseApp::VCreateHumanView()
{
	m_pHumanView = DEBUG_NEW cHumanView();
}

//  *******************************************************************************************************************
void cBaseApp::VRun()
{
	MSG msg;

	m_pGameTimer = cTimer::CreateTimer();
	m_pGameTimer->StartTimer();

	// run till completed
	while (m_Quitting == false)
	{
		// is there a message to process?
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				m_Quitting = true;
			}
			// dispatch the message
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// No message to process?
		// Then do your game stuff here
		VOnUpdate();
		VRender(m_pGameTimer->GetRunningTicks(), m_pGameTimer->GetDeltaTime());
	}
}

//  *******************************************************************************************************************
void cBaseApp::VOnUpdate()
{
	if (m_Quitting)
	{
		return;
	}
	if (m_pGameTimer != NULL)
	{
		m_pGameTimer->OnUpdate();
	}
	shared_ptr<IMessageDispatchManager> pMessageDispatcher = MakeStrongPtr(cServiceLocator::GetInstance()->GetService<IMessageDispatchManager>());
	if (pMessageDispatcher != NULL)
	{
		pMessageDispatcher->VOnUpdate();
	}

	float deltaTime = 0.0f;
	if (m_pGameTimer != NULL)
	{
		deltaTime = m_pGameTimer->GetDeltaTime();
	}
	if (m_pProcessManager)
	{
		m_pProcessManager->VUpdateProcesses(deltaTime);
	}

	shared_ptr<IEventManager> pEventManager = MakeStrongPtr(cServiceLocator::GetInstance()->GetService<IEventManager>());
	if (pEventManager != NULL)
	{
		pEventManager->VUpdate();
	}
}

//  *******************************************************************************************************************
void cBaseApp::VCleanup()
{
	m_pGameTimer.reset();
	SafeDelete(&m_pParamLoader);
	SafeDelete(&m_pProcessManager);
	SafeDelete(&m_pHumanView);
	SafeDelete(&m_pGameControls);
	SafeDelete(&m_pConfig);

	cGraphics::Destroy();
	IResourceManager::Destroy();
	IMainWindow::Destroy();
	cServiceLocator::Destroy();
}

//  *******************************************************************************************************************
float cBaseApp::GetRunningTime() const
{
	if (m_pGameTimer)
	{
		return m_pGameTimer->GetRunningTime();
	}

	return 0.f;
}

//  *******************************************************************************************************************
TICK cBaseApp::GetRunningTicks() const
{
	if (m_pGameTimer != NULL)
	{
		return m_pGameTimer->GetRunningTicks();
	}
	return 0;
}

//  *******************************************************************************************************************
// Display the FPS
//  *******************************************************************************************************************
float cBaseApp::VGetFPS() const
{
	if (m_pGameTimer != NULL)
	{
		return m_pGameTimer->GetFPS();
	}
	return 0.0f;
}

//  *******************************************************************************************************************
bool cBaseApp::VOnMsgProc(const AppMsg & msg)
{
	if (m_Quitting)
	{
		return false;
	}
	switch (msg.message)
	{
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		bool altKeyDown = ((msg.m_lParam & (1 << 29)) != 0);
		if (msg.m_wParam == VK_F4 && altKeyDown)
		{
			m_Quitting = true;
			return true;
		}
	}
	return m_pHumanView->VOnMsgProc(msg);
}

//  *******************************************************************************************************************
void cBaseApp::VRender(TICK tickCurrent, float fElapsedTime)
{
	if (m_Quitting)
	{
		return;
	}
	m_pHumanView->VOnRender(tickCurrent, fElapsedTime);
}

//  ********************************************************************************************************************
bool cBaseApp::MeetsMinimumSystemRequirements()
{
	SP_ASSERT(m_pParamLoader != NULL).SetCustomMessage("Parameter Loader does not exist");
	bool minRequirementsSatified = true;

	bool multipleInstances = (m_pParamLoader != NULL && m_pParamLoader->VGetParameterValueAsBool("-multipleinstances", false));
	if (multipleInstances)
	{
		minRequirementsSatified = ISystemChecker::Instance()->VIsOnlyInstance(m_Title);
	}

	ISystemChecker::Instance()->VCheckMemory(Megabytes(32), Megabytes(64));
	ISystemChecker::Instance()->VCheckHardDiskSpace(Megabytes(6));
	ISystemChecker::Instance()->VCheckCPUSpeed(266);

	ISystemChecker::Destroy();
	return minRequirementsSatified;
}

//  ********************************************************************************************************************
void cBaseApp::SetLoggerOptions(const unique_ptr<ILogger>& pLogger)
{
	SP_ASSERT(m_pParamLoader != NULL).SetCustomMessage("Parameter Loader does not exist");

	bool showConsoleLog = m_pParamLoader != NULL && m_pParamLoader->VGetParameterValueAsBool("-showconsolelog", false);
	bool logToText = m_pParamLoader != NULL && m_pParamLoader->VGetParameterValueAsBool("-logtotext", true);
	bool logToXML = m_pParamLoader != NULL && m_pParamLoader->VGetParameterValueAsBool("-logtoxml", false);
	unsigned int priorityLevel = m_pParamLoader == NULL ? 1 : m_pParamLoader->VGetParameterValueAsInt("-loglevel", 1);

	pLogger->VSetLogOptions(showConsoleLog, logToText, logToXML, priorityLevel);
}

//  ********************************************************************************************************************
void cBaseApp::OnInitializationError()
{
	PostQuitMessage(0);
	m_Quitting = true;
}

//  ********************************************************************************************************************
bool cBaseApp::InitializeResourceManager(const cString & assetsPath, const cString& resourceFile)
{
	SP_ASSERT_FATAL(m_pParamLoader != NULL).SetCustomMessage("Parameter Loader does not exist");

	bool useDevelopmentDirectory = m_IsEditorRunning || m_pParamLoader->VGetParameterValueAsBool("-UseDevelopmentDirectory", false);
	IResourceManager::GetInstance()->VInitialize(30, assetsPath, resourceFile, useDevelopmentDirectory);
	return true;
}

//  ********************************************************************************************************************
void cBaseApp::VInitializeGameOptions()
{
	SP_ASSERT_FATAL(m_pParamLoader != NULL).SetCustomMessage("Parameter Loader does not exist");
	cGameOptions::InitializeGameOptions(cGameDirectories::GetMediaDirectory() + "PlayerOptions.xml");

#if _DEBUG
	cGameOptions::GameOptions().bFullScreen = m_pParamLoader->VGetParameterValueAsBool("-fullscreen", cGameOptions::GameOptions().bFullScreen);
	cGameOptions::GameOptions().iWidth = m_pParamLoader->VGetParameterValueAsInt("-WindowWidth", cGameOptions::GameOptions().iWidth);
	cGameOptions::GameOptions().iHeight = m_pParamLoader->VGetParameterValueAsInt("-WindowHeight", cGameOptions::GameOptions().iHeight);
#endif
}

//  ********************************************************************************************************************
HWND cBaseApp::VInitializeApplicationWindow(const HINSTANCE & hInstance, const int cmdShow)
{
	SP_ASSERT_FATAL(m_pParamLoader != NULL).SetCustomMessage("Parameter Loader does not exist");
	return IMainWindow::GetInstance()->VOnInitialization(hInstance, cmdShow, this);
}

//  ********************************************************************************************************************
bool cBaseApp::VInitializeGraphics(const HWND& hwnd)
{
	SP_ASSERT_FATAL(m_pParamLoader != NULL).SetCustomMessage("Parameter Loader does not exist");

	vector<int> vBGColor;
	m_pParamLoader->VGetParameterValueAsIntList("-BackGroundColor", vBGColor);
	cColor bgColor = cColor::BLACK;
	if (!vBGColor.empty() && vBGColor.size() == 4)
	{
		bgColor = cColor(vBGColor[0], vBGColor[1], vBGColor[2], vBGColor[3]);
	}
	bool vSyncEnabled = m_pParamLoader->VGetParameterValueAsBool("-VSyncEnabled", false);
	float screenFar = m_pParamLoader->VGetParameterValueAsFloat("-ScreenFar", 1000.0f);
	float screenNear = m_pParamLoader->VGetParameterValueAsFloat("-ScreenNear", 0.1f);

	stInitializationParameters params = { hwnd, screenFar, screenNear, bgColor, cGameOptions::GameOptions().iWidth,
		cGameOptions::GameOptions().iHeight, cGameOptions::GameOptions().bFullScreen, vSyncEnabled };
	bool success = cGraphics::Instance()->Initialize(params);

	return success;
}

//  ********************************************************************************************************************
void cBaseApp::VCreateAndSetComponentFactory()
{
	shared_ptr<IEntityManager> pEntityManager = (MakeStrongPtr<IEntityManager>(cServiceLocator::GetInstance()->GetService<IEntityManager>()));
	if (pEntityManager != NULL)
	{
		pEntityManager->VSetAndInitializeComponentFactory(unique_ptr<cComponentFactory>(DEBUG_NEW cComponentFactory));
	}
}

//  ********************************************************************************************************************
void cBaseApp::VCreateAndSetEventFactory()
{
	shared_ptr<IEventManager> pEventManager = MakeStrongPtr(cServiceLocator::GetInstance()->GetService<IEventManager>());
	if (pEventManager != NULL)
	{
		pEventManager->VSetAndInitializeEventFactory(std::unique_ptr<cEventFactory>(DEBUG_NEW cEventFactory));
	}
}

//  *******************************************************************************************************************
void cBaseApp::RegisterServices()
{
	ISpAssertSettings::CreateAsService();
	IPhysics::CreateAsService();
	IEntityManager::CreateAsService();
	IEventManager::CreateAsService();
	cTweenManager::CreateAsService();
	IRandomGenerator::CreateAsService();
	IAudio::CreateAsService();
}

//  *******************************************************************************************************************
const IBaseApp * const IBaseApp::GetInstance()
{
	return cBaseApp::m_spInstance;
}
