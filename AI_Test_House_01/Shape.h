#pragma once
#include <vector>

// 다각형 (꼭짓점 N개를 순서대로 Line으로 이어서 그림)
class Shape
{
protected:
	std::vector<Dot> vecDots;

public:
	Shape();
	Shape(const std::vector<Dot>& dots);
	virtual ~Shape();

public:
	void AddDot(Dot d) { vecDots.push_back(d); }
	void SetDots(const std::vector<Dot>& dots) { vecDots = dots; }
	const std::vector<Dot>& GetDots() const { return vecDots; }
	void Clear() { vecDots.clear(); }

	virtual void Render();
};
