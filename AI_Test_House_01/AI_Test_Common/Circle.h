#pragma once
#include "Dot.h"

// 원 (중심 Dot + 반지름, Midpoint/Bresenham 외곽선)
class Circle
{
private:
	Dot center;
	int radius;

public:
	Circle();
	Circle(Dot c, int r);
	~Circle();

public:
	void Render();

public:
	inline void SetCenter(Dot c) { center = c; }
	inline Dot GetCenter() { return center; }

	inline void SetRadius(int r) { radius = r; }
	inline int GetRadius() { return radius; }
};
