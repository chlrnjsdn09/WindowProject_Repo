#pragma once
#include "Shape.h"

// 삼각형 (Polygon의 특수한 경우: 꼭짓점 3개)
class Triangle : public Shape
{
public:
	Triangle();
	Triangle(Dot d1, Dot d2, Dot d3);
	~Triangle();

public:
	void SetPoints(Dot d1, Dot d2, Dot d3);
};