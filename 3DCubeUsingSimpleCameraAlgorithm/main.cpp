#include<Windows.h>
#include<vector>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpstr, int iCmd)
{
	const wchar_t* className = L"3D Object";
	WNDCLASS wndclass = {};
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpfnWndProc = WndProc;
	wndclass.hCursor = LoadCursor(hInstance, nullptr);
	wndclass.hIcon = LoadIcon(hInstance, nullptr);
	wndclass.lpszClassName = className;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	RegisterClass(&wndclass);

	HWND hwnd = CreateWindow(className, className, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

	UpdateWindow(hwnd);
	ShowWindow(hwnd, iCmd);
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return msg.wParam;
}

class Vec3
{
	public:
	double x, y, z;

	Vec3(double x, double y, double z) : x{ x }, y{ y }, z{ z } { };
};

POINT PointCalculator(Vec3 point, Vec3 camera, int ScalingFactor)
{
	return POINT{ static_cast<int>(((point.x - camera.x) / (point.z - camera.z)) * ScalingFactor), static_cast<int>(((point.y - camera.y) / (point.z - camera.z)) * ScalingFactor) };
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static std::vector<Vec3> points = { {1, 1, 1}, {-1, 1, 1}, { 1, -1, 1 } , {-1, -1, 1}, {1, 1, -1}, {-1, 1, -1}, {1, -1, -1}, {-1, -1, -1 } };
	static Vec3 camera = { 2, 3, 5 };
	POINT p1, p2, p3, p4, p5, p6, p7, p8;
	PAINTSTRUCT ps;
	HDC hdc;
	static int scalingfactor = 50;
	RECT rect;
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wparam)
		{
		case 'A':
			camera.x -= 1;
			GetClientRect(hwnd, &rect);
			InvalidateRect(hwnd, &rect, TRUE);
			break;
		case 'D':
			camera.x += 1;
			GetClientRect(hwnd, &rect);
			InvalidateRect(hwnd, &rect, TRUE);
			break;
		case 'W':
			camera.y -= 1;
			GetClientRect(hwnd, &rect);
			InvalidateRect(hwnd, &rect, TRUE);
			break;
		case 'S':
			camera.y += 1;
			GetClientRect(hwnd, &rect);
			InvalidateRect(hwnd, &rect, TRUE);
			break;
		case VK_LEFT:
			scalingfactor -= 1;
			GetClientRect(hwnd, &rect);
			InvalidateRect(hwnd, &rect, TRUE);
			break;
		case VK_RIGHT:
			scalingfactor += 1;
			GetClientRect(hwnd, &rect);
			InvalidateRect(hwnd, &rect, TRUE);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		p1 = PointCalculator(points[0], camera, scalingfactor);
		p2 = PointCalculator(points[1], camera, scalingfactor);
		p3 = PointCalculator(points[2], camera, scalingfactor);
		p4 = PointCalculator(points[3], camera, scalingfactor);
		p5 = PointCalculator(points[4], camera, scalingfactor);
		p6 = PointCalculator(points[5], camera, scalingfactor);
		p7 = PointCalculator(points[6], camera, scalingfactor);
		p8 = PointCalculator(points[7], camera, scalingfactor);
		MoveToEx(hdc, p1.x, p1.y, nullptr);
		LineTo(hdc, p2.x, p2.y);
		LineTo(hdc, p4.x, p4.y);
		LineTo(hdc, p3.x, p3.y);
		LineTo(hdc, p1.x, p1.y);
		LineTo(hdc, p5.x, p5.y);
		LineTo(hdc, p6.x, p6.y);
		LineTo(hdc, p2.x, p2.y);
		MoveToEx(hdc, p6.x, p6.y, nullptr);
		LineTo(hdc, p8.x, p8.y);
		LineTo(hdc, p4.x, p4.y);
		MoveToEx(hdc, p8.x, p8.y, nullptr);
		LineTo(hdc, p7.x, p7.y);
		LineTo(hdc, p3.x, p3.y);
		MoveToEx(hdc, p7.x, p7.y, nullptr);
		LineTo(hdc, p5.x, p5.y);
		EndPaint(hwnd, &ps);
		return 0;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcW(hwnd, msg, wparam, lparam);
}