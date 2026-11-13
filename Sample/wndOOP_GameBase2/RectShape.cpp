// RectShape.cpp : 사각형 클래스의 구현입니다.
//

#include "pch.h"
#include "RectShape.h"

RectShape::RectShape(int left, int top, int right, int bottom, COLORREF color)
    : Shape(color), m_left(left), m_top(top), m_right(right), m_bottom(bottom)
{
}

void RectShape::SetRect(int left, int top, int right, int bottom)
{
    m_left = left;
    m_top = top;
    m_right = right;
    m_bottom = bottom;
}

void RectShape::Draw(HDC hdc) const
{
    HPEN hPen = CreatePen(PS_SOLID, 2, m_color);
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);
    HGDIOBJ hOldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Rectangle(hdc, m_left, m_top, m_right, m_bottom);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
}
