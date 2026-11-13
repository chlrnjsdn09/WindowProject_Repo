#pragma once

#include "Shape.h"

// 점 클래스입니다.
class Dot : public Shape
{
private:
    int m_x;
    int m_y;

public:
    Dot(int x = 0, int y = 0, COLORREF color = RGB(0, 0, 0));

    void SetPos(int x, int y);
    int GetX() const;
    int GetY() const;

    virtual void Draw(HDC hdc) const override;
};
