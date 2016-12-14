#pragma once
#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <Windows.h>

class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	// Initializes the Window
	bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);

	const HWND getWinHandle();

private:
	LPCTSTR WndClassName = "Project";
	HWND hwnd = nullptr;
};

#endif
