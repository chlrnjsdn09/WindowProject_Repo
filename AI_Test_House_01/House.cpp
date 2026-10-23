#include "pch.h"
#include "House.h"

// 기준선: 집 바닥 = 585, 지면 = 585
static const int BASE_Y = 585;

void DrawFence(HDC hdc, int xStart, int xEnd, COLORREF color)
{
    const int railY1 = 520;   // 위 가로대
    const int railY2 = 552;   // 아래 가로대
    const int picketTop = 505;    // 말뚝 몸통 위
    const int picketBottom = BASE_Y; // 말뚝 몸통 아래(지면)
    const int picketW = 10;   // 말뚝 너비
    const int gap = 8;        // 말뚝 사이 간격

    // 1) 가로대 2줄 (Line)
    Line rail1(Dot(hdc, xStart, railY1, color), Dot(hdc, xEnd, railY1, color));
    rail1.Render();
    Line rail2(Dot(hdc, xStart, railY2, color), Dot(hdc, xEnd, railY2, color));
    rail2.Render();

    // 2) 말뚝 반복 (몸통 Box + 꼭지 Triangle)
    for (int x = xStart; x + picketW <= xEnd; x += picketW + gap)
    {
        Box post(
            Dot(hdc, x, picketTop, color),
            Dot(hdc, x + picketW, picketTop, color),
            Dot(hdc, x + picketW, picketBottom, color),
            Dot(hdc, x, picketBottom, color));
        post.Render();

        Triangle cap(
            Dot(hdc, x, picketTop, color),
            Dot(hdc, x + picketW, picketTop, color),
            Dot(hdc, x + picketW / 2, picketTop - 10, color));
        cap.Render();
    }
}

void DrawHouse(HDC hdc)
{
    const COLORREF black = RGB(0, 0, 0);
    const COLORREF red = RGB(255, 0, 0);
    const COLORREF blue = RGB(0, 0, 255);
    const COLORREF green = RGB(0, 128, 0);
    const COLORREF gray = RGB(128, 128, 128);
    const COLORREF brown = RGB(139, 69, 19);
    const COLORREF darkGreen = RGB(0, 100, 0);

    // 1) 지면 (Line)
    Line ground(Dot(hdc, 40, BASE_Y, darkGreen), Dot(hdc, 660, BASE_Y, darkGreen));
    ground.Render();

    // 2) 집 몸통 (Box)
    Box body(
        Dot(hdc, 200, 300, black),
        Dot(hdc, 500, 300, black),
        Dot(hdc, 500, BASE_Y, black),
        Dot(hdc, 200, BASE_Y, black));
    body.Render();

    // 3) 지붕 (Triangle)
    Triangle roof(
        Dot(hdc, 170, 300, red),
        Dot(hdc, 350, 150, red),
        Dot(hdc, 530, 300, red));
    roof.Render();

    // 4) 굴뚝 (Box)
    Box chimney(
        Dot(hdc, 430, 165, gray),
        Dot(hdc, 460, 165, gray),
        Dot(hdc, 460, 245, gray),
        Dot(hdc, 430, 245, gray));
    chimney.Render();

    // 5) 문 (Box) + 문고리 (Dot)
    Box door(
        Dot(hdc, 320, 430, blue),
        Dot(hdc, 380, 430, blue),
        Dot(hdc, 380, BASE_Y, blue),
        Dot(hdc, 320, BASE_Y, blue));
    door.Render();
    Dot knob(hdc, 370, 510, blue);
    knob.Render();

    // 6) 왼쪽 창문 (Box) + 십자창 (Line 2개)
    Box winL(
        Dot(hdc, 230, 370, green),
        Dot(hdc, 290, 370, green),
        Dot(hdc, 290, 430, green),
        Dot(hdc, 230, 430, green));
    winL.Render();
    Line winLCrossH(Dot(hdc, 230, 400, green), Dot(hdc, 290, 400, green));
    winLCrossH.Render();
    Line winLCrossV(Dot(hdc, 260, 370, green), Dot(hdc, 260, 430, green));
    winLCrossV.Render();

    // 7) 오른쪽 창문 (Box) + 십자창 (Line 2개)
    Box winR(
        Dot(hdc, 410, 370, green),
        Dot(hdc, 470, 370, green),
        Dot(hdc, 470, 430, green),
        Dot(hdc, 410, 430, green));
    winR.Render();
    Line winRCrossH(Dot(hdc, 410, 400, green), Dot(hdc, 470, 400, green));
    winRCrossH.Render();
    Line winRCrossV(Dot(hdc, 440, 370, green), Dot(hdc, 440, 430, green));
    winRCrossV.Render();

    // 8) 현관 앞 길 (Shape 사다리꼴)
    Shape path;
    path.AddDot(Dot(hdc, 310, BASE_Y, brown));
    path.AddDot(Dot(hdc, 390, BASE_Y, brown));
    path.AddDot(Dot(hdc, 420, 620, brown));
    path.AddDot(Dot(hdc, 280, 620, brown));
    path.Render();

    // 9) 울타리: 좌우 2구간 (Box + Triangle + Line 조합)
    DrawFence(hdc, 55, 185, brown);
    DrawFence(hdc, 515, 645, brown);
}
