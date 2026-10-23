// Shape.cpp : Shape 베이스 클래스의 구현입니다.
//

#include "pch.h"
#include "Shape.h"

Shape::Shape(COLORREF color)
    : m_color(color)
{
}

Shape::~Shape()
{
}

void Shape::SetColor(COLORREF color)
{
    m_color = color;
}

COLORREF Shape::GetColor() const
{
    return m_color;
}
