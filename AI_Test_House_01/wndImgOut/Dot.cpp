// Dot.cpp : 점 클래스의 구현입니다.
//

#include "pch.h"
#include "Dot.h"

Dot::Dot(int x, int y, COLORREF color)
    : Shape(color), m_x(x), m_y(y)
{
}

void Dot::SetPos(int x, int y)
{
    m_x = x;
    m_y = y;
}

int Dot::GetX() const
{
    return m_x;
}

int Dot::GetY() const
{
    return m_y;
}

void Dot::Draw(HDC hdc) const
{
    // 작은 원으로 점을 표현합니다. (반지름 4)
    HPEN hPen = CreatePen(PS_SOLID, 1, m_color);
    HBRUSH hBrush = CreateSolidBrush(m_color);
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);
    HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);

    Ellipse(hdc, m_x - 4, m_y - 4, m_x + 4, m_y + 4);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}
