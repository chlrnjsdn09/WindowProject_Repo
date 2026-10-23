#include "pch.h"
#include <algorithm>

Shape::Shape()
{
}

Shape::Shape(const std::vector<Dot>& dots) : vecDots(dots)
{
}

Shape::~Shape()
{
}

// 꼭짓점을 순서대로 이어서 윤곽선을 그림
void Shape::Render()
{
	size_t n = vecDots.size();
	if (n < 2) return;

	for (size_t i = 0; i < n; i++)
	{
		// 마지막 점은 첫 번째 점과 연결 (도형 닫기)
		Line line(vecDots[i], vecDots[(i + 1) % n]);
		line.Render();
	}
}
