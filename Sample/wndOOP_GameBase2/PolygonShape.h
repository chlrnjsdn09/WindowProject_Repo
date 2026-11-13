#pragma once

#include "Shape.h"

// 다각형 클래스입니다. (vector 없이 고정 배열 사용)
class PolygonShape : public Shape
{
public:
    static const int MAX_POINTS = 16;

private:
    POINT m_pts[MAX_POINTS];
    int m_count;

public:
    PolygonShape(COLORREF color = RGB(0, 0, 0));

    void Clear();
    bool AddPoint(int x, int y);
    int GetCount() const;
    POINT GetPoint(int index) const;

    virtual void Draw(HDC hdc) const override;
};
