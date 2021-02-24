#pragma once

#ifdef __INTELLISENSE__
#include "pch.h"
#endif // __INTELLISENSE__

#define ALPHA_VANTAGE_ENV_VAR_NAME L"ALPHA_VANTAGE_API_KEY_PATH"

class Engine
{
public:

	void Setup(HWND mainWindow);

	void DoFrame();

	void Cleanup();
private:

	void Update();

	ID3D11Texture2D* GetBackbufferTexture();

	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;
	IDXGISwapChain* mySwapChain;
	ID3D11RenderTargetView* myRenderTarget;

	FiskJSON::Object response;

	float myLastUpdate;
	std::string myAlphaVantageAPIKey;
};

