#pragma once

#include "framework.h"

//
//  클래스: BlueBoy
//
//  용도: BlueBoy 시트의 걷기 애니메이션으로 창 안을 좌우로
//        왔다갔다 하는 캐릭터입니다. (현재는 걷기 동작만 사용)
//
//  시트 정보:
//        원본 BlueBoy.png(1027x1531)를 마젠타 배경의 BlueBoy.bmp로 변환해 사용합니다.
//        걷기 동작은 두번째 행(y=456~816)이며 5프레임입니다.
//        걷기 프레임은 오른쪽을 바라보고 있어서, 왼쪽으로 이동할 때 flip으로 반전합니다.
//        (픽셀 실측 결과 머리카락 덩어리는 왼쪽, 얼굴은 오른쪽에 있어 우향임을 확인)
//
//  이미지 관리:
//        이미지는 Bitmap 싱글턴에 "BlueBoy" 키로 등록해 함께 관리합니다.
//        마젠타(255,0,255)는 캐릭터에 없는 색이라 투명 컬러키로 사용합니다.
//
//  타이밍:
//        게임 루프(PeekMessage)는 매우 빠르게 돌므로 Update() 안에서
//        GetTickCount()로 간격을 확인합니다. 애니메이션(초당 5프레임)과
//        이동은 간격을 따로 두어, 프레임 전환이 느려도 걸음은 부드럽게 이동합니다.
//
//  사용 방식:
//        BlueBoy boy;
//        boy.Load(L"BlueBoy.bmp");
//        boy.Update();   // 게임 루프에서
//        boy.Draw(hdc);  // 합성 DC에
//
class BlueBoy
{
public:
    static const int MAX_FRAMES = 5;   // 걷기 프레임 수

    BlueBoy(int x = 320, int y = 400, int minX = 70, int maxX = 570);

    // Bitmap 싱글턴에 걷기 이미지를 등록하고 투명키를 설정합니다. 성공 시 true.
    bool Load(const wchar_t* path);
    bool IsLoaded() const;
    void Unload();

    void SetRange(int minX, int maxX);
    void SetPos(int x, int y);
    int GetX() const;
    int GetDir() const;

    // 프레임 전환 + 좌우 이동 + 방향(반전) 결정. (게임 루프에서 호출)
    void Update();
    // 현재 프레임을 발·중심 기준으로 그립니다.
    void Draw(HDC hdc) const;

private:
    int m_frame;
    DWORD m_lastAni;       // 마지막으로 애니메이션 프레임을 바꾼 시각
    DWORD m_aniInterval;   // 애니메이션 간격(ms) — 초당 5프레임(200ms)
    DWORD m_lastMove;      // 마지막으로 이동한 시각
    DWORD m_moveInterval;  // 이동 간격(ms) — 애니메이션과 분리해 부드러운 이동

    int m_x;        // 기준점 (발 중심 X)
    int m_y;        // 기준점 (지면 높이)
    int m_dir;      // 1: 오른쪽, -1: 왼쪽
    int m_speed;
    int m_minX;
    int m_maxX;
    int m_destH;    // 화면 출력 높이 (폭은 프레임 비율로 결정)
};
