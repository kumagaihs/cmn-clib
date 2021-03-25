#include <iostream>

#include "cmn-tools/win32/Skeleton.hpp"


namespace cmnclib {
namespace tools {
namespace win32 {

	static LPCWSTR WND_CLASS_NAME = L"kumagaihs/cmn-clib";
	static LPCWSTR WND_NAME = L"cmn-tools/win32/Skeleton";

	LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	Skeleton::Skeleton(HINSTANCE hInstance)
	{
		m_hInstance = hInstance;
	}

	Skeleton::~Skeleton()
	{
	}

	WPARAM Skeleton::Run()
	{
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return msg.wParam;
	}

	BOOL Skeleton::InitApp()
	{

		WNDCLASSEX WCEX;

		WCEX.cbSize = sizeof(WNDCLASSEX);
		WCEX.hInstance = m_hInstance;
		WCEX.lpszClassName = WND_CLASS_NAME;
		WCEX.lpfnWndProc = MainWindowProc;
		WCEX.style = 0;

		// Icon, Cursor, Menu, Extra, Background
		WCEX.hIcon = LoadIcon(NULL, IDI_APPLICATION);  // 標準アイコン
		WCEX.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   // 小さいアイコン
		WCEX.hCursor = LoadCursor(NULL, IDC_ARROW);
		WCEX.lpszMenuName = NULL;
		WCEX.cbClsExtra = 0;
		WCEX.cbWndExtra = 0;
		WCEX.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

		// Entry Window Class
		return(RegisterClassEx(&WCEX));

	}

	BOOL Skeleton::InitInstance(int mode)
	{

		HWND hwnd;

		// Make Window
		hwnd = CreateWindow(
			WND_CLASS_NAME,         // ウィンドウクラスの名前
			WND_NAME,               // タイトル
			WS_OVERLAPPEDWINDOW,    // ウィンドウスタイル
			(GetSystemMetrics(SM_CXSCREEN) - 640) >> 1, // X座標 ( ウィンドウを中央に )
			(GetSystemMetrics(SM_CYSCREEN) - 480) >> 1, // Y座標
			640,                    // 幅
			480,                    // 高さ
			HWND_DESKTOP,           // 親ウィンドウ
			NULL,                   // メニュー
			m_hInstance,            // プログラムのこのインスタンスのハンドル
			NULL                    // このほかの引数
		);

		// Show Window
		ShowWindow(hwnd, mode);
		UpdateWindow(hwnd);

		return TRUE;

	}

	LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	{

		switch (msg) {
		case WM_PAINT:

			break;

		case WM_COMMAND:

			break;

		case WM_CREATE:

			break;

		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, msg, wp, lp);
		}

		return(0);

	}

}
}
}