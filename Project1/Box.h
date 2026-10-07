#pragma once

class Box
{
private:
	Dot p1;
	Dot p2;
	Dot p3;
	Dot p4;

public:
	Box();
	Box(Dot lt, Dot rt, Dot rb, Dot lb);
	~Box();

public:
	void SetPoints(Dot lt, Dot rt, Dot rb, Dot lb) { p1 = lt; p2 = rt; p1 = rb; p2 = lb; }

	void Draw();

};