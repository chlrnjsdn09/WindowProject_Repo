#pragma once

class Line
{
private:
	Dot p1;
	Dot p2;

public:
	Line();
	Line(Dot d1, Dot d2);
	~Line();

public:
	void SetPoints(Dot d1, Dot d2) { p1 = d1; p2 = d2; }

	void HLine();
	void VLine();

	void Render();
};