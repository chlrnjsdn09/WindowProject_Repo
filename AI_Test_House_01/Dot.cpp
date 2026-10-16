#include "pch.h"
#include "Dot.h"

Dot::Dot()
{

}

Dot::Dot(HDC dc, int x, int y, COLORREF clr)
	: hdc(dc), ptCoord(POINT(x, y)), color(clr)
{
}


Dot::~Dot()
{

}

void Dot::Render()
{
	SetPixel(this->hdc, this->ptCoord.x, this->ptCoord.y, this->color);
}
