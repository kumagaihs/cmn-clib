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
		WCEX.hIcon = LoadIcon(NULL, IDI_APPLICATION);  // �W���A�C�R��
		WCEX.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   // �������A�C�R��
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
			WND_CLASS_NAME,         // �E�B���h�E�N���X�̖��O
			WND_NAME,               // �^�C�g��
			WS_OVERLAPPEDWINDOW,    // �E�B���h�E�X�^�C��
			(GetSystemMetrics(SM_CXSCREEN) - 640) >> 1, // X���W ( �E�B���h�E�𒆉��� )
			(GetSystemMetrics(SM_CYSCREEN) - 480) >> 1, // Y���W
			640,                    // ��
			480,                    // ����
			HWND_DESKTOP,           // �e�E�B���h�E
			NULL,                   // ���j���[
			m_hInstance,            // �v���O�����̂��̃C���X�^���X�̃n���h��
			NULL                    // ���̂ق��̈���
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