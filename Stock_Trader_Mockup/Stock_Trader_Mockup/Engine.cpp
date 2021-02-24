#include "pch.h"
#include "Engine.h"


void SaveImGuiStyle()
{
	static_assert(sizeof(char) == 1, "Double check");
	std::ofstream stream;
	stream.open("ImGuiStyle.ini", std::ios::binary | std::ios::out);
	stream.write(reinterpret_cast<char*>(&ImGui::GetStyle()), sizeof(*(&ImGui::GetStyle())));
}

void LoadOrDefaultImGuiStyle()
{
	static_assert(sizeof(char) == 1, "Double check");
	std::ifstream stream;
	stream.open("ImGuiStyle.ini", std::ios::binary | std::ios::in);
	if (stream)
	{
		stream.read(reinterpret_cast<char*>(&ImGui::GetStyle()), sizeof(*(&ImGui::GetStyle())));
	}
	else
	{
		ImGui::StyleColorsDark();
	}
}

void Engine::Setup(HWND mainWindow)
{
	DXGI_SWAP_CHAIN_DESC desc = {  };
	//ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = mainWindow;
	desc.SampleDesc.Count = 1;
	desc.Windowed = true;

	HRESULT result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION,
		&desc, &mySwapChain, &myDevice, nullptr, &myContext);

	if (FAILED(result))
	{
		exit(-1);
	}

	auto* backbuffer = GetBackbufferTexture();
	result = myDevice->CreateRenderTargetView(backbuffer, nullptr, &myRenderTarget);
	backbuffer->Release();



	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	LoadOrDefaultImGuiStyle();
	ImGui_ImplDX11_Init(myDevice, myContext);
	ImGui_ImplWin32_Init(mainWindow);



	{
		const size_t buffersize = MAX_PATH;
		WCHAR buffer[buffersize];
		WIPE(buffer);
		DWORD result = GetEnvironmentVariable(ALPHA_VANTAGE_ENV_VAR_NAME, buffer, buffersize);
		if (result < buffersize)
		{
			std::wstring path = std::wstring(buffer, result);
			std::fstream avantGuard(path);
			std::getline(avantGuard, myAlphaVantageAPIKey);
		}
		else
		{
			std::cout << "buffersize " STRING(MAX_PATH) " was too small for avante guard path" << std::endl;
		}
	}

	response.Parse("{\"Hello\":123,\"World\":[1,3,2,3]}");
	//Update();
}

void Engine::DoFrame()
{
	float color[] = {1.f,1.f,1.f,1.f};
	myContext->ClearRenderTargetView(myRenderTarget, color);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("hello");
	ImGui::Json("Response",response);
	ImGui::End();


	myContext->OMSetRenderTargets(1, &myRenderTarget, nullptr);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	mySwapChain->Present(1, 0);
}

void Engine::Cleanup()
{
	myRenderTarget->Release();
	mySwapChain->Release();
	myDevice->Release();
	myContext->Release();
	curlpp::Cleanup cleanup;
}


void Engine::Update()
{
	curlpp::Easy request;

	std::string url;
	std::string symbol = "IBM";

	url = "www.https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" + symbol + "&apikey=" + myAlphaVantageAPIKey;

	request.setOpt<curlpp::options::Url>(url.c_str());
	std::ostringstream ss;
	curlpp::options::WriteStream ws(&ss);

	request.perform();

	response.Parse(ss.str());

	myLastUpdate = Tools::GetTotalTime();
}

ID3D11Texture2D* Engine::GetBackbufferTexture()
{
	HRESULT result;
	if (mySwapChain)
	{
		ID3D11Texture2D* texture;
		result = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&texture);
		if (FAILED(result))
		{
			return nullptr;
		}
		return texture;
	}
}