
#include <windows.h>
#include <math.h>

#define LIMIT 200
#define R 84

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;

int APIENTRY WinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	MyRegisterClass(hInstance);

	hInst = hInstance;

	if (!InitInstance (hInstance, nCmdShow))
		return FALSE;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= 0;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "RUNAWAYWND";
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hWnd = CreateWindowEx(WS_EX_TOOLWINDOW, "RUNAWAYWND", "runaway", WS_POPUP | WS_BORDER,
		GetSystemMetrics(SM_CXSCREEN)/2 - R,
		GetSystemMetrics(SM_CYSCREEN)/2 - R,
		2*R,
		2*R,
		NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool bEscape = false;

	switch (message)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			HBITMAP hBmp = LoadBitmap(hInst, "BMP8556");
			BITMAP bmp;
			GetObject(hBmp, sizeof(BITMAP), &bmp);
			HDC hdcBmp = CreateCompatibleDC(hdc);
			SelectObject(hdcBmp, hBmp);
			RECT rc;
			GetClientRect(hWnd, &rc);

			BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcBmp, 0, 0, SRCCOPY);

			DeleteDC(hdcBmp);
			DeleteObject(hBmp);

			EndPaint(hWnd, &ps);
		}
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
			break;
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_CLOSE:
		break;
	case WM_CREATE:
		SetTimer(hWnd, 1, 20, NULL);
		break;
	case WM_TIMER:
		{
			if ((GetKeyState(VK_ESCAPE) & 0x80) && (GetKeyState(VK_CONTROL) & 0x80))
				DestroyWindow(hWnd);

			POINT pt;
			RECT rw;
			int cx, cy;
			GetCursorPos(&pt);
			GetWindowRect(hWnd, &rw);
			cx = rw.left + R;
			cy = rw.top + R;

			if ((pt.x-cx)*(pt.x-cx)+(pt.y-cy)*(pt.y-cy) < LIMIT*LIMIT)
			{
				double d = (double)LIMIT / sqrt((double)((pt.x-cx)*(pt.x-cx)+(pt.y-cy)*(pt.y-cy)));
				double nx = (double)(pt.x) - (double)(pt.x-cx) * d;
				double ny = (double)(pt.y) - (double)(pt.y-cy) * d;
				int scx = GetSystemMetrics(SM_CXSCREEN);
				int scy = GetSystemMetrics(SM_CYSCREEN);

				if (nx < 0.0 || ny < 0.0 || nx >= (double)scx || ny >= (double)scy)
					SetWindowPos(hWnd, HWND_TOPMOST, scx/2 - R, scy/2 - R, 0, 0, SWP_NOSIZE);
				else
					SetWindowPos(hWnd, HWND_TOPMOST, (int)nx - R, (int)ny - R, 0, 0, SWP_NOSIZE);
			}
			else
			{
				SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			}
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
