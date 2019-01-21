#include <windows.h>
#include <math.h>
#include<stdlib.h>
#include <stack>
struct P {
	int x;
	int y;
};
std::stack<P> s;
void DDA(HDC hdc, int x0, int y0, int xn, int yn)
{
	/*确定增量dx,dy*/
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
	/*绘点*/
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

void RoundArc(HDC hdc, int x, int y, int r)
{
	int X, Y, d;
	X = x;
	Y = y - r;
	d = (y - Y)*(y - Y) + (Y + 1 - y)*(Y + 1 - y) - 2 * r * r;

	while ((X - x) <= (y - Y))
	{
		if (d >= 0)
		{
			Y++;
			d = d + 4 * (X + Y) - 4 * (x + y) + 10;
		}
		else
		{
			d = d + 4 * X - 4 * x + 6;
		}

		SetPixel(hdc, X, Y, RGB(0, 0, 0));
		SetPixel(hdc, (2 * x - X), Y, RGB(0, 0, 0));
		SetPixel(hdc, X, (2 * y - Y), RGB(0, 0, 0));
		SetPixel(hdc, (2 * x - X), (2 * y - Y), RGB(0, 0, 0));
		SetPixel(hdc, (x + (y - Y)), (y - (X - x)), RGB(0, 0, 0));
		SetPixel(hdc, (x + (y - Y)), (y + (X - x)), RGB(0, 0, 0));
		SetPixel(hdc, (x - (y - Y)), (y - (X - x)), RGB(0, 0, 0));
		SetPixel(hdc, (x - (y - Y)), (y + (X - x)), RGB(0, 0, 0));

		X++;
	}
}


void EllipseArc(HDC hdc, int x, int y, int a, int b)
{
	float t = 0, dt = 0.001;
	int x1 = x + a, y1 = y, x2, y2;
	while (t < 2 * 3.14159) {
		t += dt;
		x2 = x + (int)(a * cos(t));
		y2 = y + (int)(b * sin(t));
		Bresenham(hdc, x1, y1, x2, y2);
		x1 = x2;
		y1 = y2;
	}
}

void bubbleSort(double array[][2], int size)
{
	bool changed = true;
	int n = 0;
	do {
		changed = false;
		for (int i = 0; i < size - 1 - n; i++) {
			if (array[i][0] > array[i + 1][0]) {
				int temp;
				temp = array[i][0];
				array[i][0] = array[i + 1][0];
				array[i + 1][0] = temp;
				temp = array[i][1];
				array[i][1] = array[i + 1][1];
				array[i + 1][1] = temp;
				changed = true;
			}
		}
		n++;
	} while (changed);
}


void shadowFill(HDC hdc, int P[][2], int mn, int m)
{
	const double k = 1.0;
	const double db = 3.0;
	double B[8][2];
	int i, j;
	for (i = 0; (i + 1) <= (m - 1); ++i) {
		B[i][0] = (double)(P[i][1] - k * P[i][0]);
		B[i][1] = (double)(P[i + 1][1] - k * P[i + 1][0]);
		if (B[i][0] > B[i][1]) {
			double temp = B[i][0];
			B[i][0] = B[i][1];
			B[i][1] = temp;
		}
	}
	B[m - 1][0] = (double)(P[m - 1][1] - k * P[m - 1][0]);
	B[m - 1][1] = (double)(P[0][1] - k * P[0][0]);
	if (B[m - 1][0] > B[m - 1][1]) {
		double temp = B[m - 1][0];
		B[m - 1][0] = B[m - 1][1];
		B[m - 1][1] = temp;
	}
	for (i = m; (i + 1) <= (mn - 1); ++i) {
		B[i][0] = (double)(P[i][1] - k * P[i][0]);
		B[i][1] = (double)(P[i + 1][1] - k * P[i + 1][0]);
		if (B[i][0] > B[i][1]) {
			double temp = B[i][0];
			B[i][0] = B[i][1];
			B[i][1] = temp;
		}
	}
	B[mn - 1][0] = (double)(P[mn - 1][1] - k * P[mn - 1][0]);
	B[mn - 1][1] = (double)(P[m][1] - k * P[m][0]);
	if (B[mn - 1][0] > B[mn - 1][1]) {
		double temp = B[mn - 1][0];
		B[mn - 1][0] = B[mn - 1][1];
		B[mn - 1][1] = temp;
	}

	double Bmin = B[0][0], Bmax = B[0][1];
	for (i = 1; i < mn; i++) {
		if (B[i][0] < Bmin)
			Bmin = B[i][0];
		if (B[i][1] > Bmax)
			Bmax = B[i][1];
	}

	double b = Bmin + db;
	double D[8][2];
	double xp, yp, xq, yq, x, y;
	while (b < Bmax) {
		for (i = 0; i <= mn - 1; ++i)
			for (j = 0; j <= 1; ++j)
				D[i][j] = 10000.0;
		for (i = 0; (i + 1) <= (m - 1); ++i) {
			if ((B[i][0] <= b) && (b < B[i][1])) {
				xp = P[i][0];
				yp = P[i][1];
				xq = P[i + 1][0];
				yq = P[i + 1][1];
				x = (xp*yq - yp * xq + b * (xq - xp)) / (yq - yp - k * (xq - xp));
				y = k * x + b;
				D[i][0] = x;
				D[i][1] = y;
			}
		}
		if ((B[m - 1][0] <= b) && (b < B[m - 1][1])) {
			xp = P[m - 1][0];
			yp = P[m - 1][1];
			xq = P[0][0];
			yq = P[0][1];
			x = (xp*yq - yp * xq + b * (xq - xp)) / (yq - yp - k * (xq - xp));
			y = k * x + b;
			D[i][0] = x;
			D[i][1] = y;
		}
		for (i = m; (i + 1) <= (mn - 1); ++i) {
			if ((B[i][0] <= b) && (b < B[i][1])) {
				xp = P[i][0];
				yp = P[i][1];
				xq = P[i + 1][0];
				yq = P[i + 1][1];
				x = (xp*yq - yp * xq + b * (xq - xp)) / (yq - yp - k * (xq - xp));
				y = k * x + b;
				D[i][0] = x;
				D[i][1] = y;
			}
		}
		if ((B[mn - 1][0] <= b) && (b < B[mn - 1][1])) {
			xp = P[mn - 1][0];
			yp = P[mn - 1][1];
			xq = P[m][0];
			yq = P[m][1];
			x = (xp*yq - yp * xq + b * (xq - xp)) / (yq - yp - k * (xq - xp));
			y = k * x + b;
			D[i][0] = x;
			D[i][1] = y;
		}

		bubbleSort(D, 8);
		for (int k = 0; (D[2 * k][0] != 10000) && (D[2 * k + 1][0] != 10000); k++) {
			DDA(hdc, D[2 * k][0], D[2 * k][1], D[2 * k + 1][0], D[2 * k + 1][1]);
		}
		b = b + db;
	}
}
void myPolygon(HDC hdc, int p[][2], int mn, int m)
{
	int i;
	for (i = 0; (i + 1) <= (m - 1); ++i)
		DDA(hdc, p[i][0], p[i][1], p[i + 1][0], p[i + 1][1]);
	DDA(hdc, p[0][0], p[0][1], p[m - 1][0], p[m - 1][1]);
	for (i = m; (i + 1) <= (mn - 1); ++i)
		DDA(hdc, p[i][0], p[i][1], p[i + 1][0], p[i + 1][1]);
	DDA(hdc, p[m][0], p[m][1], p[mn - 1][0], p[mn - 1][1]);
}

void ColorFill(HDC hdc, int x, int y, COLORREF old_color, COLORREF new_color)
{
	COLORREF col = GetPixel(hdc, x, y);
	if (col == old_color) {
		SetPixel(hdc, x, y, new_color);
		ColorFill(hdc, x + 1, y, old_color, new_color);
		ColorFill(hdc, x, y + 1, old_color, new_color);
		ColorFill(hdc, x, y - 1, old_color, new_color);
		ColorFill(hdc, x - 1, y, old_color, new_color);
	}
	/*for (int i = 100; i <= 200; i++) {
		for (int j = 100; j <= 200; j++) {
			SetPixel(hdc, i, j, new_color);
		}
	}*/
}
void ScanLineSeedFill(HDC hdc, int x, int y, COLORREF oldcolor, COLORREF newcolor) {
	/*P p;
	p.x = x, p.y = y;
	s.push(p);
	while (s.size()) {
		P cur = s.top();
		s.pop();
		
	}*/
	//左右延伸填充颜色,(x,y)是本行扫描线的种子
	int xr = x + 1, xl = x - 1;
	while (GetPixel(hdc, xl, y) == oldcolor) {
		SetPixel(hdc, xl, y, newcolor);
		xl--;
	}
	while (GetPixel(hdc, xr, y) == oldcolor) {
		SetPixel(hdc, xr, y, newcolor);
		xr++;
	}
	xl++, xr--;
	//确定相邻两行的种子点
	int nx;
	bool flag = false;
	for (nx = xl; x <= xr; x++) {
		if (GetPixel(hdc, nx, y + 1) == oldcolor) {
			flag = true;
			break;
		}
	}
	if (flag) {
		while (GetPixel(hdc, nx, y + 1) == oldcolor) {
			nx++;
		}
		nx--;
		ScanLineSeedFill(hdc, nx, y + 1, oldcolor, newcolor);
	}
	flag = false;
	for (nx = xl; nx <= xr; nx++) {
		if (GetPixel(hdc, nx, y - 1) == oldcolor) {
			flag = true;
			break;
		}
	}
	if (flag) {
		while (GetPixel(hdc, nx, y - 1) == oldcolor) {
			nx++;
		}
		nx--;
		ScanLineSeedFill(hdc, nx, y - 1, oldcolor, newcolor);
	}
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//声明窗口过程函数

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	//Windows程序入口函数,功能类似C语言的main()函数
	static TCHAR szAppName[] = TEXT("Graph");    //窗口类标题
	HWND            hwnd;
	MSG              msg;
	WNDCLASS   wndclass;               //定义一个WNDCLASS型结构
	UINT              nWight = 300;    //窗口宽度
	UINT              nHeight = 400;    //窗口高度

	wndclass.style = CS_HREDRAW | CS_VREDRAW;                //窗口大小发生变化时自动刷新
	wndclass.lpfnWndProc = WndProc;                                //设置窗口过程(fnp)
	wndclass.cbClsExtra = 0;                                    //预留空间，字节为单位
	wndclass.cbWndExtra = 0;                                    //预留空间，字节为单位
	wndclass.hInstance = hInstance;                            //指想程序的实列句柄
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);        //设置窗口图标样式
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);            //设置鼠标样式
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);    //设置窗口背景颜色
	wndclass.lpszMenuName = NULL;                                    //指定窗口菜单项NULL为空
	wndclass.lpszClassName = szAppName;                            //窗口类名称，这里指想字符串数组

	if (!RegisterClass(&wndclass))        //注册窗口类
	{
		MessageBox(NULL, TEXT("程序运行失败!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName,                  // RegisterClass注册的窗口类名
		TEXT("Graph"), // 窗口标题
		WS_OVERLAPPEDWINDOW,        // 窗口样式   *
		CW_USEDEFAULT,                 // int x 窗口打开的X坐标
		CW_USEDEFAULT,                 // int y 窗口打开的Y坐标
		nWight,                     // int nWidth----窗口宽
		nHeight,                     // int nHeight---窗口高
		NULL,                       // parent window handle
		NULL,                       // window menu handle
		hInstance,                  // program instance handle
		NULL);                     // creation parameters

	ShowWindow(hwnd, iCmdShow);        //在显示器上显示窗口
	UpdateWindow(hwnd);                //绘制窗口

	while (GetMessage(&msg, NULL, 0, 0))
	{
		//消息循环
		TranslateMessage(&msg);    //将击键消息转换为字符消息
		DispatchMessage(&msg);    //将消息回传给Windows，由Windows转发给适当的窗口过程
	   //这里的窗口过程为WndProc()函数
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//窗口过程函数
	HDC          hdc;
	PAINTSTRUCT ps;
	RECT         rect;

	switch (message)
	{
	case WM_PAINT:
		//InvalidateRect(hwnd,NULL,TRUE);

		hdc = BeginPaint(hwnd, &ps);    //创建设备描述表句柄

		GetClientRect(hwnd, &rect);

		//阴影填充
		 //DDA(hdc,100,100,200,100);
		 //DDA(hdc,100,100,100,200);
		 //DDA(hdc,100,200,200,200);
		 //DDA(hdc,200,200,200,100);
		 ////ColorFill(hdc,150,150,RGB(255,255,255),RGB(0,0,255));
		 //ScanLineSeedFill(hdc, 150, 150, RGB(255, 255, 255), RGB(0, 0, 255));
		Bresenham(hdc, 100, 100, 200, 300);
		//Bresenham(hdc, 200, 300, 300, 100);
		Bresenham(hdc, 100, 100, 300, 100);
		Bresenham(hdc, 200, 300, 300, 100);
		//DDA(hdc,20,400,400,20);
		//RoundArc(hdc,200,200,100);
		//EllipseArc(hdc,200,200,150,100);

		EndPaint(hwnd, &ps);    //释放设备描述表句柄
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam); //处理所有窗口过程不予处理的消息
}