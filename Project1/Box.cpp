#include "pch.h"
#include "Box.h"

Box::Box()
{
}

Box::Box(Dot lt, Dot rt, Dot rb, Dot lb) : p1(lt), p2(rt), p3(rb), p4(lb)
{
}

Box::~Box()
{
}

void Box::Draw()
{
	Line(p1, p2).HLine();
	Line(p3, p2).VLine();
	Line(p4, p3).HLine();
	Line(p4, p1).VLine();
}