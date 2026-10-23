// House.cpp : 집 그리기(House) 클래스의 구현입니다.
//              로직은 AI_Test_Project_005의 DrawHouse에서 가져왔습니다.
//

#include "pch.h"
#include "framework.h"
#include "Shape.h"
#include "Dot.h"
#include "Line.h"
#include "Triangle.h"
#include "RectShape.h"
#include "House.h"

//
//  함수: House::Draw(HDC)
//
//  용도: 제목 문자열과 집 전체를 hdc에 그립니다.
//        도형은 스택에 만들고 Shape* 고정 배열로 다형성을 보여줍니다.
//
void House::Draw(HDC hdc) const
{
    if (hdc == nullptr)
    {
        return;
    }

    const wchar_t* title = L"wndImgOut - 집 그리기";
    TextOutW(hdc, 20, 20, title, lstrlenW(title));

    // 집 본체와 지붕, 굴뚝
    RectShape body(250, 280, 550, 500, RGB(180, 120, 60));
    Triangle roof(220, 280, 580, 280, 400, 150, RGB(200, 50, 50));
    RectShape chimney(500, 150, 540, 230, RGB(100, 100, 100));

    // 문과 문고리
    RectShape door(370, 400, 430, 500, RGB(120, 70, 30));
    Dot knob(420, 450, RGB(0, 0, 0));

    // 창문 2개와 창살
    RectShape win1(280, 330, 340, 390, RGB(0, 0, 0));
    RectShape win2(460, 330, 520, 390, RGB(0, 0, 0));
    Line win1V(310, 330, 310, 390, RGB(0, 0, 0));
    Line win1H(280, 360, 340, 360, RGB(0, 0, 0));
    Line win2V(490, 330, 490, 390, RGB(0, 0, 0));
    Line win2H(460, 360, 520, 360, RGB(0, 0, 0));

    // 지면
    Line ground(100, 500, 700, 500, RGB(0, 128, 0));

    // 베이스 포인터 고정 배열로 다형성을 보여줍니다.
    Shape* shapes[16];
    int count = 0;
    shapes[count++] = &body;
    shapes[count++] = &roof;
    shapes[count++] = &chimney;
    shapes[count++] = &door;
    shapes[count++] = &knob;
    shapes[count++] = &win1;
    shapes[count++] = &win2;
    shapes[count++] = &win1V;
    shapes[count++] = &win1H;
    shapes[count++] = &win2V;
    shapes[count++] = &win2H;
    shapes[count++] = &ground;

    for (int i = 0; i < count; i++)
    {
        shapes[i]->Draw(hdc);
    }
}
