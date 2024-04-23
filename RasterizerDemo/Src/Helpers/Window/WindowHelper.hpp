#pragma once

#include <Windows.h>

class WindowHelper
{
public:    
    bool SetupWindow(HINSTANCE instance, UINT width, UINT height, int nCmdShow, HWND & window);

private:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
