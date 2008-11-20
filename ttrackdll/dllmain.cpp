// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "stdafx.h"

#define DllExport    __declspec( dllexport )

DllExport   void CALLBACK SetHook(HWND);
DllExport   void CALLBACK FreeHook(void);

//共有領域
#pragma data_seg(".sharedata")
HHOOK hHookWnd = 0;
HWND  hOwnerWnd = NULL;
#pragma data_seg()

static HINSTANCE hDll = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hDll = hModule;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode==HC_ACTION){
		if(((CWPSTRUCT *)lParam)->message == WM_ACTIVATE && LOWORD(((CWPSTRUCT *)lParam)->wParam) != WA_INACTIVE){
			PostMessage(hOwnerWnd, WM_USER + 1, NULL, (LPARAM)((CWPSTRUCT *)lParam)->hwnd);
		}
	}
	return CallNextHookEx(hHookWnd, nCode, wParam, lParam);
}

void CALLBACK SetHook(HWND hWnd)
{
	hOwnerWnd = hWnd;
	hHookWnd = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, hDll, 0);
}

void CALLBACK FreeHook(void)
{
	UnhookWindowsHookEx(hHookWnd);
}