#include <windows.h> 
#include <stdlib.h> 
#include <string.h> 
#include <tchar.h> 
#include <stdio.h>
#include <math.h>

void DDA(HDC hdc, int x0, int y0, int xn, int yn)
{
	/*ȷ������dx,dy*/
	int max;
	double dx, dy;
	int d_x = abs(xn - x0);
	int d_y = abs(yn - y0);
	if (d_x >= d_y)
		max = d_x;
	else
		max = d_y;
	dx = (xn - x0) / max;
	dy = (yn - y0) / max;
	/*���*/
	double xi = x0, yi = y0;
	int pix_x = floor(xi), pix_y = floor(yi);
	SetPixel(hdc, pix_x, pix_y, RGB(23, 24, 23));
	for (int i = 0; i != max; ++i) {
		xi += dx;
		yi += dy;
		pix_x = floor(xi);
		pix_y = floor(yi);
		SetPixel(hdc, pix_x, pix_y, RGB(23, 24, 23));
	}

}

void Bresenham(HDC hdc, int x1, int y1, int x2, int y2)
{
	int x, y, dx, dy, e;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	e = 2 * dy - dx;
	x = x1;
	y = y1;
	for (int i = 0; i < dx; i++) {
		SetPixel(hdc, x, y, RGB(0, 0, 0));
		if (e >= 0) {
			y++;
			e = e - 2 * dx;
		}
		x++;
		e = e + 2 * dy;
	}

	return;
}

void ColorFill(HDC hdc, int x, int y, COLORREF old_color, COLORREF new_color)
{

	if (x >= 100 && x <= 200 && y >= 100 && y <= 200 && GetPixel(hdc, x, y) == old_color) {
		SetPixel(hdc, x, y, new_color);
		ColorFill(hdc, x + 1, y, old_color, new_color);
		ColorFill(hdc, x, y + 1, old_color, new_color);
		ColorFill(hdc, x, y - 1, old_color, new_color);
		ColorFill(hdc, x - 1, y, old_color, new_color);
	}
}

static TCHAR szWindowClass[] = _T("win32app");
// Ӧ�ó�������������ֵ��ַ���
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");

HINSTANCE hInst;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	//���Ѵ����Ĵ��������ע�ᡣ ʹ�� RegisterClassEx ����������������ṹ��Ϊ�������ݡ�
	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Win32 Guided Tour"), NULL);
		return 1;
	}

	// �����ʵ���洢��ȫ�ֱ�����
	hInst = hInstance;

	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 100, NULL, NULL, hInstance, NULL);
	if (!hWnd) {
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Win32 Guided Tour"), NULL);
		return 1;
	}


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ���������������ϵͳ��������Ϣ����Ϣѭ���� 
	// ��Ӧ�ó����յ�һ����Ϣʱ����ѭ��������Ϣ���ȵ� WndProc �����Խ��д��� 
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg); //������Ϣ
		DispatchMessage(&msg);  //��ǲ��Ϣ
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, World!");
	RECT         rect;

	switch (message) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);

		//��Ӱ���
		DDA(hdc, 100, 100, 200, 100);
		DDA(hdc, 100, 100, 100, 200);
		DDA(hdc, 100, 200, 200, 200);
		DDA(hdc, 200, 200, 200, 100);
		ColorFill(hdc, 150, 150, RGB(255, 255, 255), RGB(0, 0, 255));
		

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}
