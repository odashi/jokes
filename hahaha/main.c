#include <windows.h>


int WINAPI WinMain(HINSTANCE hInstahce, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HDC hdc = GetDC(NULL);
	int i;
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);

	HGDIOBJ br = SelectObject(hdc, GetStockObject(BLACK_BRUSH));
	HGDIOBJ pen = SelectObject(hdc, GetStockObject(NULL_PEN));

	Rectangle(hdc, 0, 0, w+1, h+1);

	for (i = 0; i < 2000; i++)
	{
		Rectangle(hdc, rand() % w, rand() % h, rand() % w, rand() % h);
		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		Rectangle(hdc, rand() % w, rand() % h, rand() % w, rand() % h);
		SelectObject(hdc, GetStockObject(GRAY_BRUSH));
		Rectangle(hdc, rand() % w, rand() % h, rand() % w, rand() % h);
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		Sleep(1);
	}

	SelectObject(hdc, br);
	SelectObject(hdc, pen);
	ReleaseDC(NULL, hdc);
	InvalidateRect(NULL, NULL, TRUE);

	return 0;
}