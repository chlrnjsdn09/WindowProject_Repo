#include "pch.h"
#include "Triangle.h"

Triangle::Triangle()
{
}

Triangle::Triangle(Dot d1, Dot d2, Dot d3)
{
	SetPoints(d1, d2, d3);
}

Triangle::~Triangle()
{
}

void Triangle::SetPoints(Dot d1, Dot d2, Dot d3)
{
	vecDots.clear();
	vecDots.push_back(d1);
	vecDots.push_back(d2);
	vecDots.push_back(d3);
}
