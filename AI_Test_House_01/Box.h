#pragma once
class Box
{
private:
	union
	{
		struct
		{
			Line p1;
			Line p2;
			Line p3;
			Line p4;
		};

		Line p[4];
	};

public:
	Box();
	Box(Dot lt, Dot rt, Dot rb, Dot lb);
	~Box();

public:
	void Render();
};
