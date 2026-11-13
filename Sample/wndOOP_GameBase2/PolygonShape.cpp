// PolygonShape.cpp : 다각형 클래스의 구현입니다.
//

#include "pch.h"
#include "PolygonShape.h"

PolygonShape::PolygonShape(COLORREF color)
    : Shape(color), m_count(0)
{
}

void PolygonShape::Clear()
{
    m_count = 0;
}

bool PolygonShape::AddPoint(int x, int y)
{
    if (m_count < 0 || m_count >= MAX_POINTS)
    {
        return false;
    }
    m_pts[m_count].x = x;
    m_pts[m_count].y = y;
    m_count++;
    return true;
}

int PolygonShape::GetCount() const
{
    return m_count;
}

POINT PolygonShape::GetPoint(int index) const
{
    POINT empty = { 0, 0 };
    if (index < 0 || index >= m_count)
    {
        return empty;
    }
    return m_pts[index];
}

void PolygonShape::Draw(HDC hdc) const
{
    // 점이 2개 이하면 그리지 않습니다.
    if (m_count <= 2)
    {
        return;
    }

    HPEN hPen = CreatePen(PS_SOLID, 2, m_color);
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);
    HGDIOBJ hOldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Polygon(hdc, const_cast<POINT*>(m_pts), m_count);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
}
