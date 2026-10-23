#pragma once

#include "Shape.h"

//
//  클래스: House
//
//  용도: AI_Test_Project_005의 집 그리기 로직(DrawHouse)을 캡슐화합니다.
//        도형(Shape 파생 클래스)들을 만들고, 베이스 포인터 고정 배열로
//        묶어 가상 Draw()를 호출하는 다형성을 그대로 유지합니다.
//
class House
{
public:
    House() = default;
    ~House() = default;

    House(const House&) = delete;
    House& operator=(const House&) = delete;

    // 제목 텍스트와 집 전체(본체·지붕·굴뚝·문·창문·창살·지면)를 그립니다.
    void Draw(HDC hdc) const;
};
