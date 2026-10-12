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
