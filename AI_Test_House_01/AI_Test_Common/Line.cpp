#include "pch.h"
#include <algorithm>

Line::Line()
{
}

Line::Line(Dot d1, Dot d2) : p1(d1), p2(d2)
{
}

Line::~Line()
{
}

// 수정됨: 시작점/끝점 순서와 관계없이 수평선을 그린다.
// (기존 코드는 sx > ex 인 경우 아무것도 그리지 않았음)
void Line::HLine()
{
	HDC hdc = p1.GetHDC();
	COLORREF color = p1.GetColor();
	int y = p1.GetCoord().y;

	int sx = p1.GetCoord().x;
	int ex = p2.GetCoord().x;

	int xStart = (std::min)(sx, ex);
	int xEnd = (std::max)(sx, ex);

	for (int x = xStart; x <= xEnd; x++)
	{
		Dot dot(hdc, x, y, color);
		dot.Render();
	}
}

// 수정됨: 시작점/끝점 순서와 관계없이 수직선을 그린다.
// (기존 코드는 루프 조건이 뒤집혀 위->아래 순서에서 아무것도 그리지 않았음)
void Line::VLine()
{
	HDC hdc = p1.GetHDC();
	COLORREF color = p1.GetColor();
	int x = p1.GetCoord().x;

	int sy = p1.GetCoord().y;
	int ey = p2.GetCoord().y;

	int yStart = (std::min)(sy, ey);
	int yEnd = (std::max)(sy, ey);

	for (int y = yStart; y <= yEnd; y++)
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
