#pragma once

#include "BaseWnd.h"

#ifndef MAX_LOADSTRING
#define MAX_LOADSTRING 100
#endif

//
//  클래스: GameWnd
//
//  용도: 게임용 주 창입니다. BaseWnd를 상속받아(is-a) 창 캡슐화와
//        메시지 라우팅을 이어받고, 여기에 게임 초기화(Start/Release),
//        타이머(FPS/애니메이션), 진입점·게임루프(Run)를 추가합니다.
//
//  구조 (wndOOP_GameBase 대비):
//        원본: 전역함수 WndProc + OnCreate/OnCommand/OnTimer/OnDestroy + 전역 루프
//        현재: BaseWnd 가상 핸들러를 GameWnd에서 재정의하고, 루프는 Run()에 넣음
//        GameManager는 전역 인스턴스 g_game 대신 싱글턴 GetInstance()로 접근
//
//  게임루프:
//        PeekMessage 기반 게임루프를 Run()에서 돌립니다. (원본 GameMsgLoop와 동일)
//        놀 때(메시지 없음 + 활성)마다 GameManager::Update()를 호출합니다.
//
class GameWnd : public BaseWnd
{
public:
    GameWnd() = default;
    ~GameWnd() override = default;

    // 전역 문자열을 읽고 창 클래스를 등록한 뒤 주 창을 만듭니다.
    BOOL Initialize(HINSTANCE hInstance, int nCmdShow);

    // PeekMessage 기반 게임 메시지 루프를 실행하고 종료 코드를 반환합니다.
    int Run();

protected:
    // 우클릭 시 "종료하기" 컨텍스트 메뉴를 띄웁니다.
    LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) override;

    // 창이 만들어질 때 게임 자원을 준비하고 타이머를 켭니다.
    LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct) override;

    // 메뉴 명령(끝내기/정보)을 처리합니다.
    LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;

    // 주 창을 그립니다. (PeekMessage 루프가 주 그리기를 담당하므로 최소 동작만)
    LRESULT OnPaint() override;

    // 타이머: FPS 측정과 애니메이션 전환을 GameManager에 전달합니다.
    LRESULT OnTimer(WPARAM wParam, LPARAM lParam) override;

    // 창이 닫힐 때 게임 자원을 해제하고 애플리케이션을 종료합니다.
    LRESULT OnDestroy() override;

private:
    HACCEL m_hAccelTable = nullptr;
    WCHAR  m_szTitle[MAX_LOADSTRING] = {};
    WCHAR  m_szWindowClass[MAX_LOADSTRING] = {};

    BOOL   m_bIsLoopExit = TRUE;    // 게임루프 종료 플래그
    BOOL   m_bIsActive   = FALSE;   // 게임루프 활성화 상태
};
