#pragma once

#include "BlueBoy.h"

//
//  클래스: GameManager
//
//  용도: 게임 루프의 전체 상태(더블버퍼링 DC, 비트맵, FPS, 애니메이션)와
//        라이프사이클(Awake/Start/Update/Release)을 하나로 캡슐화합니다.
//
//  싱글턴:
//        게임은 전역적으로 하나뿐이므로 싱글턴 패턴을 사용합니다.
//        외부에서는 전역 변수 없이 GameManager::GetInstance()로 접근합니다.
//        (DoubleBuffer, Bitmap도 동일한 GetInstance() 싱글턴 패턴을 사용)
//
//  변환 포인트(wndOOP_GameBase 대비):
//        전역 인스턴스 g_game          →  GameManager::GetInstance()
//        extern GameManager g_game;    →  (제거) private 생성자로 외부 생성 차단
//
//  사용 방식:
//        GameManager::GetInstance().Start(hWnd);
//        GameManager::GetInstance().Update();
//
class GameManager
{
public:
    // 전역 인스턴스를 반환합니다. (함수 내 static 변수로 프로그램 종료 시 자동 소멸)
    static GameManager& GetInstance();

    // 복사를 막아 "상태를 가진 단 하나의 게임"임을 표현합니다.
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    // 게임 시작 전 준비 단계입니다. (현재 비어 있음)
    void Awake();

    // 윈도우 핸들로 게임 자원(DC/비트맵)을 생성합니다.
    void Start(HWND hWnd);

    // 매 프레임 호출되어 화면을 갱신합니다.
    void Update();

    // 게임 자원(DC/비트맵)을 해제합니다.
    void Release();

    // 1초 타이머에서 초당 프레임을 기록합니다.
    void OnFpsTimer();

    // 애니메이션 프레임을 전환합니다.
    void ToggleAnimation();

private:
    // 싱글턴이므로 생성자/소멸자를 외부에서 호출할 수 없게 막습니다.
    GameManager() = default;
    ~GameManager() = default;

    HWND m_hWnd = nullptr;              // 게임이 붙어 있는 메인 윈도우 핸들

    HDC     m_ScreenDC  = nullptr;      // 화면DC //메인DC
    HDC     m_MemoryDC  = nullptr;      // 메모리에서 이미지를 모으는 DC
    HBITMAP m_hBitmap   = nullptr;      // 해당DC의 이미지 자체
    HDC     m_BkImageDC = nullptr;      // 메모리DC //이미지 연결 DC
    HBITMAP m_hBkImage  = nullptr;      // 흰 이미지 배경 깔기용
    HDC     m_ImageDC[2] = { nullptr, nullptr }; // 메모리DC //실제 구현된 이미지DC
    HBITMAP m_hImage[2]  = { nullptr, nullptr }; // 출력 이미지 자체

    BlueBoy m_blueBoy;                  // 좌우로 왔다갔다하는 걷기 캐릭터

    int m_nFPS   = 0;                   // 게임루프 초당 프레임 검출값
    int m_nCount = 0;                   // 게임루프 프레임 카운터
    int m_nAniOn = 0;                   // 애니메이션 프레임 선택 인덱스(0/1)
};
