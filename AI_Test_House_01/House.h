#pragma once

// 집 한 채(몸통+지붕+굴뚝+문+창문+길)와 울타리를 그린다.
// Dot / Line / Box / Triangle / Shape 클래스만 사용한다.
void DrawHouse(HDC hdc);

// xStart ~ xEnd 구간에 뾰족 말뚝 울타리를 그린다.
// (말뚝 몸통 = Box, 말뚝 꼭지 = Triangle, 가로대 = Line)
void DrawFence(HDC hdc, int xStart, int xEnd, COLORREF color);
