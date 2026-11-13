// Triangle.cpp : 삼각형 클래스의 구현입니다.
//

#include "pch.h"
#include "Triangle.h"

Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, COLORREF color)
    : Shape(color)
{
    SetPoints(x1, y1, x2, y2, x3, y3);
}

Triangle::Triangle(POINT p1, POINT p2, POINT p3, COLORREF color)
    : Shape(color)
{
    m_pts[0] = p1;
    m_pts[1] = p2;
    m_pts[2] = p3;
}

void Triangle::SetPoints(int x1, int y1, int x2, int y2, int x3, int y3)
{
    m_pts[0].x = x1; m_pts[0].y = y1;
    m_pts[1].x = x2; m_pts[1].y = y2;
    m_pts[2].x = x3; m_pts[2].y = y3;
}

POINT Triangle::GetPoint(int index) const
{
    POINT empty = { 0, 0 };
    if (index < 0 || index >= 3)
    {
        return empty;
    }
    return m_pts[index];
}

void Triangle::Draw(HDC hdc) const
{
    HPEN hPen = CreatePen(PS_SOLID, 2, m_color);
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);
    HGDIOBJ hOldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Polygon(hdc, const_cast<POINT*>(m_pts), 3);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
}
