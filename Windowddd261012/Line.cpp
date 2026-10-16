#include "pch.h"
#include "Line.h"

Line::Line()
{
}

Line::Line(Dot d1, Dot d2) : p1(d1), p2(d2)
{
}

Line::~Line()
{
}

void Line::HLine()
{
	HDC hdc = p1.GetHDC();
	COLORREF color = p1.GetColor();
	int y = p1.GetCoord().y;

	int sx = p1.GetCoord().x;
	int ex = p2.GetCoord().x;

	for (int x = sx; x < ex; x++)
	{
		Dot dot(hdc, x, y, color);
		dot.Render();
	}
}

void Line::VLine()
{
	HDC hdc = p1.GetHDC();
	COLORREF color = p1.GetColor();
	int x = p1.GetCoord().x;

	int sy = p1.GetCoord().y;
	int ey = p2.GetCoord().y;

	for (int y = ey; y < sy; y++)
	{
		Dot dot(hdc, x, y, color);
		dot.Render();
	}
}

void Line::Render()
{
	HDC hdc = p1.GetHDC();
	COLORREF color = p1.GetColor();

	// 두 점 좌표
	int x1 = p1.GetCoord().x, y1 = p1.GetCoord().y;
	int x2 = p2.GetCoord().x, y2 = p2.GetCoord().y;

	// x,y 증가량
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	
	// 방향 (x가 양수면 오른쪽, 음수면 왼쪽)
	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;
	
	// 
	int err = dx - dy;

	while (true)
	{
		SetPixelV(hdc, x1, y1, color);
		if (x1 == x2 && y1 == y2) break;
		int e2 = err << 1; // (err * 2)
		if (e2 > -dy) { err -= dy; x1 += sx; }
		if (e2 < dx) { err += dx; y1 += sy; }
	}
}