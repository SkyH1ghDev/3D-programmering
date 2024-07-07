// Memory Leak Debugging
#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>

// Application
#include <Windows.h>
#include <d3d11.h>

#include "DirectXMath.h"
#include "Application.hpp"

namespace DX = DirectX;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{

	// Braces to let the application go out of scope before checking memory leaks
	{	
		Application app(hInstance, nCmdShow);
	
		app.Run();
	}

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();

	return 0;
}