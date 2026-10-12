#include "pch.h"
#include "Box.h"

Box::Box()
{
}

Box::Box(Dot lt, Dot rt, Dot rb, Dot lb)
	:p1(lt, rt), p2(rt, rb), p3(rb, lb), p4(lb, lt)
{
}

Box::~Box()
{
}

void Box::Render()
{
	//위, 오, 아래, 왼
	p1.HLine();
	p2.VLine();
	p3.HLine();
	p4.VLine();
}
