#pragma once
#include <Windows.h>
#include <Dwmapi.h> 
#include <cstdio>
#include <process.h>
#include <iostream>
#include "hack.h"
#pragma comment(lib,"Dwmapi.lib")
#pragma warning(disable : 4996)

extern void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

	D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

	std::memset(&d3dpp, 0, sizeof(d3dpp)); // clear out the struct for use
	d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
	d3dpp.hDeviceWindow = hWnd;    // set the window to be used by Direct3D
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;     // set the back buffer format to 32-bit
	d3dpp.BackBufferWidth = GetDisplaySize().x;    // set the width of the buffer
	d3dpp.BackBufferHeight = GetDisplaySize().y;    // set the height of the buffer

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
	D3DXCreateFontA(d3ddev, 25, 0, FW_HEAVY, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Malgun Gothic", &pFont);
	D3DXCreateLine(d3ddev, &d3dLine);
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		if (d3ddev != NULL && wParam != SIZE_MINIMIZED)
		{
			g_d3dpp.BackBufferWidth = LOWORD(lParam);
			g_d3dpp.BackBufferHeight = HIWORD(lParam);
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

WPARAM MainLoop()
{
	MSG msg;
	RECT rc;
	std::memset(&rc, 0, sizeof(RECT));

	GetWindowRect(gWnd, &rc);

	//GNamesPtr = 0x0;
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			exit(0);

		std::memset(&rc2, 0, sizeof(RECT));
		GetWindowRect(gWnd, &rc2);
		if (rc.bottom != rc2.bottom || rc.left != rc2.left || rc.right != rc2.right || rc.top != rc2.top) {
			rc = rc2;
			initD3D(hWnd);
			MoveWindow(hWnd, rc.left + 6, rc.top + 2, GetDisplaySize().x - 13, GetDisplaySize().y - 6, true);
		}

		d3ddev->SetRenderState(D3DRS_ZENABLE, false);
		d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		//d3ddev->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		//D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*255.0f), (int)(clear_color.y*255.0f), (int)(clear_color.z*255.0f), (int)(clear_color.w*255.0f));
		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		//d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
		d3ddev->BeginScene();
		static bool Init = true;
		if (Init)
		{
			Offsets::Init();
			Decrypt::Init();
			Init = false;
		}
		Hack::MainLoop();
		d3ddev->EndScene();
		HRESULT result = d3ddev->Present(NULL, NULL, NULL, NULL);

		// Handle loss of D3D9 device
		if (result == D3DERR_DEVICELOST && d3ddev->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			d3ddev->Reset(&g_d3dpp);
	}
	Sleep(1);
	return msg.wParam;
}

void MainThread()
{
	__try {
		//MessageBoxA(nullptr, "Success!", "secret.club", MB_OK);

		if (AllocConsole()) {

			freopen("conin$", "rb", stdin);

			freopen("conout$", "wb", stdout);

			freopen("conout$", "wb", stderr);
		}

		while (!gWnd)
			gWnd = FindWindow(L"UnrealWindow", L"PLAYERUNKNOWN'S BATTLEGROUNDS ");

		RECT rc;

		if (gWnd != NULL)
		{
			GetWindowRect(gWnd, &rc);
		}
		else
		{
			std::cout << "게임이 종료되었습니다." << GetLastError() << std::endl;
			Sleep(3000);
			Cleanup();
			ExitProcess(0);
		}

		WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"Chrome", NULL };
		RegisterClassEx(&wc);

		hWnd = CreateWindowEx(0, wc.lpszClassName, L"Chrome", WS_POPUP, 0, 0, GetDisplaySize().x, GetDisplaySize().y, NULL, NULL, wc.hInstance, NULL);

		HRGN rgn;
		DWM_BLURBEHIND blur;
		rgn = CreateRectRgn(0, 0, 1, 1);
		blur.dwFlags = 0x1 | 0x2;
		blur.fEnable = true;
		blur.fTransitionOnMaximized = true;
		blur.hRgnBlur = rgn;
		//SetWindowPos(hWnd, NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
		DwmEnableBlurBehindWindow(hWnd, &blur);
		SetWindowDisplayAffinity(hWnd, WDA_EXCLUDEFROMCAPTURE);
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);

		initD3D(hWnd);
		MainLoop();
	}
	__except (1) {

	}
}

void RemovePeHeader(HMODULE hModule)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((PBYTE)pDosHeader + (DWORD)pDosHeader->e_lfanew);

	if (pNTHeader->FileHeader.SizeOfOptionalHeader)
	{
		DWORD Protect;
		WORD Size = pNTHeader->FileHeader.SizeOfOptionalHeader;
		VirtualProtect((void*)hModule, Size, PAGE_EXECUTE_READWRITE, &Protect);
		SecureZeroMemory((void*)hModule, Size);
		VirtualProtect((void*)hModule, Size, Protect, &Protect);
	}
}

BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpReserved)
{

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hInst);
		RemovePeHeader(hInst);
		while (true)
		{
			MainThread();
		}
	}
	return TRUE;
}