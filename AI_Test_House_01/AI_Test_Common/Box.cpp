#include "pch.h"

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

// 수정됨: 방향에 의존하는 HLine/VLine 대신
// 양방향으로 동작하는 Line::Render()로 4변을 그린다.
// (기존 코드는 우측/하단 변이 그려지지 않았음)
void Box::Render()
{
	//위, 오, 아래, 왼
	p1.Render();
	p2.Render();
	p3.Render();
	p4.Render();
}
