#include "stdafx.h"
#include "resource\resource.h"

static NOTIFYICONDATA ni;

void TrayiconShow(HWND hWnd, LPCWSTR tipText)
{
	HINSTANCE hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
	HICON hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_TTRACK), IMAGE_ICON, 16, 16, LR_SHARED);

	ni.cbSize = sizeof(ni);
	ni.hWnd = hWnd;
	ni.uID = 1;
	ni.uCallbackMessage = WM_USER + 10;
	ni.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	ni.hIcon = hIcon;
	lstrcpy(ni.szTip, tipText);
	Shell_NotifyIcon(NIM_ADD, &ni);
}

void TrayiconDelete(void)
{
	Shell_NotifyIcon(NIM_DELETE, &ni);
}