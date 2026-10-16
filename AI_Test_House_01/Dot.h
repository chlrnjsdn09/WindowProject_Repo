#pragma once
class Dot
{
private:
	HDC hdc;
	POINT ptCoord;
	COLORREF color;

public:
	Dot();
	Dot(HDC dc, int x, int y, COLORREF clr);
	~Dot();

public:
	void Render();

public:
	inline void SetHDC(HDC dc) { hdc = dc; }
	inline HDC GetHDC() { return hdc; }

	inline void SetCoord(POINT pt) { ptCoord = pt; }
	inline POINT GetCoord() { return ptCoord; }

	inline void SetColor(COLORREF clr) { color = clr; }
	inline COLORREF GetColor() { return color; }
};
