#include "pch.h"
#include "WindowHandler.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

WindowHandler::WindowHandler()
{
}

void WindowHandler::Setup()
{
	
	WNDCLASSW windowClass = {};
	windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = WindowHandler::WinProc;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = L"GalacticExpresss";
	RegisterClassW(&windowClass);
	myWindowStyle = WS_POPUP | WS_VISIBLE;

#if USEFULLSCREEN
	myWindowStyle |= WS_MAXIMIZE | WS_OVERLAPPED;
	aWindowData.myX = 0;
	aWindowData.myY = 0;
	aWindowData.myWidth = CAST(unsigned short, GetSystemMetrics(SM_CXSCREEN));
	aWindowData.myHeight = CAST(unsigned short, GetSystemMetrics(SM_CYSCREEN));
#else
	myWindowStyle |= WS_OVERLAPPEDWINDOW;
#endif


	unsigned short windowX = rand() % 200;
	unsigned short windowY = rand() % 200;
	unsigned short windowWidth = 800;
	unsigned short windowHeight = 600;


	myHWND = CreateWindowW(L"GalacticExpresss", L"Fisk Stock", myWindowStyle,
		windowX, windowY, windowWidth, windowHeight,
		nullptr, nullptr, nullptr, this);


	myEngine.Setup(myHWND);
}

int WindowHandler::Run()
{
	MSG windowMessage;
	WIPE(windowMessage);
	bool shouldRun = true;

	while (shouldRun)
	{
		while (PeekMessage(&windowMessage, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&windowMessage);
			DispatchMessage(&windowMessage);

			if (windowMessage.message == WM_QUIT || windowMessage.message == WM_DESTROY)
			{
				shouldRun = false;
			}
		}

		myEngine.DoFrame();

	}
	return 0;
}

LRESULT WindowHandler::WinProc(HWND aHWND, UINT aUMsg, WPARAM aWParam, LPARAM aLParam)
{
	static WindowHandler* windowHandler = nullptr;
	static bool hwndIsSet = false;


	static bool isClosing = false;
	if (aUMsg == WM_DESTROY || aUMsg == WM_CLOSE)
	{
		isClosing = true;
		PostQuitMessage(0);
		return 0;
	}
	if (isClosing)
	{
		return 0;
	}

	if (aUMsg == WM_CREATE)
	{
		CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(aLParam);
		windowHandler = reinterpret_cast<WindowHandler*>(createStruct->lpCreateParams);
	}
	else if (aUMsg == WM_SIZE)
	{
		//myFrameWork->Resize(aHWND);
	}

	if (ImGui_ImplWin32_WndProcHandler(aHWND, aUMsg, aWParam, aLParam))
	{
		return TRUE;
	}

#ifdef _DEBUG
#if ESCAPEQUITSGAME
	if (myInputHandler.IsKeyDown(CommonUtilities::InputHandler::Key_Escape))
	{
		PostQuitMessage(0);
		return 0;
	}
#endif
#endif // DEBUG


	return DefWindowProc(aHWND, aUMsg, aWParam, aLParam);
}
