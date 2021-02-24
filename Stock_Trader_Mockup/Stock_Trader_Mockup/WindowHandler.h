#pragma once

#ifdef __INTELLISENSE__
#include "pch.h"
#endif


#include "Engine.h"

class WindowHandler
{
public:
	WindowHandler();

	void Setup();

	int Run();


private:

	static LRESULT WinProc(HWND aHWND, UINT aUMsg, WPARAM aWParam, LPARAM aLParam);

	Engine myEngine;
	DWORD myWindowStyle;
	HWND myHWND = NULL;
};

