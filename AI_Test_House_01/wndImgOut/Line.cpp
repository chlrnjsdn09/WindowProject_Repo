// Line.cpp : 선 클래스의 구현입니다.
//

#include "pch.h"
#include "Line.h"

Line::Line(int x1, int y1, int x2, int y2, COLORREF color)
    : Shape(color)
{
    m_a.x = x1;
    m_a.y = y1;
    m_b.x = x2;
    m_b.y = y2;
}

Line::Line(POINT a, POINT b, COLORREF color)
    : Shape(color), m_a(a), m_b(b)
{
}

void Line::SetPoints(int x1, int y1, int x2, int y2)
{
    m_a.x = x1;
    m_a.y = y1;
    m_b.x = x2;
    m_b.y = y2;
}

POINT Line::GetA() const
{
    return m_a;
}

POINT Line::GetB() const
{
    return m_b;
}

void Line::Draw(HDC hdc) const
{
    HPEN hPen = CreatePen(PS_SOLID, 2, m_color);
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);

    MoveToEx(hdc, m_a.x, m_a.y, nullptr);
    LineTo(hdc, m_b.x, m_b.y);

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}
