#pragma once

#include "Shape.h"

// 선 클래스입니다.
class Line : public Shape
{
private:
    POINT m_a;
    POINT m_b;

public:
    Line(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0, COLORREF color = RGB(0, 0, 0));
    Line(POINT a, POINT b, COLORREF color = RGB(0, 0, 0));

    void SetPoints(int x1, int y1, int x2, int y2);
    POINT GetA() const;
    POINT GetB() const;

    virtual void Draw(HDC hdc) const override;
};
