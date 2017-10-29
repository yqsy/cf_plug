#pragma once

#pragma warning (disable:4101)
#pragma warning (disable:4102)
#pragma warning (disable:4244)
#pragma warning (disable:4305)
#pragma warning (disable:4309)
#pragma warning (disable:4996)

#include <Windows.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <shlwapi.h>
#include <tlhelp32.h>
#include <fstream>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>   
#include <cctype> 
#include <vector>

using namespace std;
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "shlwapi.lib")

#include "detours.h"
#include "cAddress.h"
#include "Class.h"
#include "Colors.h"
#include "cMenu.h"
#include "pDevice.h"
#include "cRender.h"
#include "cVars.h"
#include "cTools.h"
#include "cMain.h"
#include "cEngine.h"
#include "cESP.h"
