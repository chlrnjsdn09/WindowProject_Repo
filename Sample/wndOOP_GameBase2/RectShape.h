#pragma once

#include "Shape.h"

// 사각형 클래스입니다. (left, top, right, bottom 보관)
class RectShape : public Shape
{
private:
    int m_left;
    int m_top;
    int m_right;
    int m_bottom;

public:
    RectShape(int left = 0, int top = 0, int right = 0, int bottom = 0, COLORREF color = RGB(0, 0, 0));

    void SetRect(int left, int top, int right, int bottom);

    virtual void Draw(HDC hdc) const override;
};
