#pragma once

#include <Windows.h>

class WindowHelper
{
public:    
    bool SetupWindow(HINSTANCE instance, int nCmdShow, HWND& window);

private:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
