#pragma once

#include "framework.h"

// 모든 도형의 베이스 클래스입니다.
// vector를 사용하지 않고, 고정 배열 + 일반 배열로 화면에 보여줍니다.
class Shape
{
protected:
    COLORREF m_color;

public:
    Shape(COLORREF color = RGB(0, 0, 0));
    virtual ~Shape();

    void SetColor(COLORREF color);
    COLORREF GetColor() const;

    virtual void Draw(HDC hdc) const = 0;
};
