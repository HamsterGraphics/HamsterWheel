﻿/*
 * Copyright (c) 2024-2025 HamsterGraphics
 *
 * https://github.com/HamsterGraphics/HamsterWheel
 *
 */

#include <IWindow.h>

#include <Base/TypeTraits.h>

#include "WindowsUtils.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        break;
    }
    }

    return ::DefWindowProc(hwnd, message, wParam, lParam);
}

bool Window_Init()
{
	WNDCLASSW windowClass;
	memset(&windowClass, 0, sizeof(windowClass));
    windowClass.style = 0;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = (HINSTANCE)::GetModuleHandle(NULL);
    windowClass.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = HG_WINDOW_CLASS_NAME;
    ::RegisterClassW(&windowClass);

    return true;
}

bool Window_HandleMessages()
{
    MSG msg;
    msg.message = NULL;
    bool quit = false;
    while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);

        if (WM_CLOSE == msg.message ||
            WM_QUIT == msg.message)
        {
            quit = true;
        }
    }
    return quit;
}

void Window_Create(WindowInfo* pWindowDesc)
{
    size_t charConverted = 0;
    WCHAR appName[256] = {};
    mbstowcs_s(&charConverted, appName, pWindowDesc->Name, COUNTOF(appName));

    auto& windowRect = pWindowDesc->WindowRect;
    HWND hwnd = ::CreateWindowW(HG_WINDOW_CLASS_NAME, appName, WS_OVERLAPPEDWINDOW,
        windowRect.X, windowRect.Y, windowRect.Width, windowRect.Height, NULL, NULL, (HINSTANCE)GetModuleHandle(NULL), 0);
    pWindowDesc->Handle = hwnd;

    if (!pWindowDesc->Hide)
    {
        if (pWindowDesc->Maxmized)
        {
            ::ShowWindow(hwnd, SW_MAXIMIZE);
        }
        else if (pWindowDesc->Minimized)
        {
            ::ShowWindow(hwnd, SW_MINIMIZE);
        }
        else
        {
            ::ShowWindow(hwnd, SW_SHOW);
        }
    }
    else
    {
        ::ShowWindow(hwnd, SW_HIDE);
    }
}

void Window_Destroy(WindowInfo* pWindowDesc)
{
    ::DestroyWindow((HWND)pWindowDesc->Handle);
    pWindowDesc->Handle = NULL;
}