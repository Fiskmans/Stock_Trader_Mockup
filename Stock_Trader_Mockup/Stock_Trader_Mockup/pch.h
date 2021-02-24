#pragma once

//windows
#include <WinSock2.h>
#include <Windows.h>

//Direct x
#include <d3d11.h>

//STD
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <any>
#include <variant>
#include <vector>
#include <unordered_map>
#include <stack>

//Curl
#include "curlpp\cURLpp.hpp"
#include "curlpp\Easy.hpp"
#include "curlpp\Options.hpp"

//Imgui
#define IMGUI_ENABLED
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

//Custom
#include "FiskJSON.h"
#include "TimeHelper.h"

#define WIPE(arg) memset(&arg,0,sizeof(arg));
#define STRING(arg) #arg