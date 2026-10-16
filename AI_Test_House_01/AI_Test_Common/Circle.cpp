#include "pch.h"
#include "Circle.h"

Circle::Circle() : radius(0)
{
}

Circle::Circle(Dot c, int r) : center(c), radius(r)
{
}

Circle::~Circle()
{
}

// Midpoint 원 알고리즘 (8-way 대칭, Line::Render와 같은 SetPixelV 방식)
void Circle::Render()
{
	HDC hdc = center.GetHDC();
	COLORREF color = center.GetColor();
	int cx = center.GetCoord().x;
	int cy = center.GetCoord().y;

	if (radius <= 0)
	{
		SetPixelV(hdc, cx, cy, color);
		return;
	}

	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y)
	{
		SetPixelV(hdc, cx + x, cy + y, color);
		SetPixelV(hdc, cx + y, cy + x, color);
		SetPixelV(hdc, cx - y, cy + x, color);
		SetPixelV(hdc, cx - x, cy + y, color);
		SetPixelV(hdc, cx - x, cy - y, color);
		SetPixelV(hdc, cx - y, cy - x, color);
		SetPixelV(hdc, cx + y, cy - x, color);
		SetPixelV(hdc, cx + x, cy - y, color);

		y++;
		if (err <= 0)
		{
			err += 2 * y + 1;
		}
		if (err > 0)
		{
			x--;
			err -= 2 * x + 1;
		}
	}
}
