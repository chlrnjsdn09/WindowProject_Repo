#pragma once

#include "Shape.h"

// 삼각형 클래스입니다. (고정 배열 3개 사용)
class Triangle : public Shape
{
private:
    POINT m_pts[3];

public:
    Triangle(int x1, int y1, int x2, int y2, int x3, int y3, COLORREF color = RGB(0, 0, 0));
    Triangle(POINT p1, POINT p2, POINT p3, COLORREF color = RGB(0, 0, 0));

    void SetPoints(int x1, int y1, int x2, int y2, int x3, int y3);
    POINT GetPoint(int index) const;

    virtual void Draw(HDC hdc) const override;
};
