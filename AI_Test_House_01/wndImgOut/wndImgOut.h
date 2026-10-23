#pragma once

#include "Resource.h"
#include "MainProc.h"

#ifndef MAX_LOADSTRING
#define MAX_LOADSTRING 100
#endif

//
//  클래스: Application
//
//  용도: 애플리케이션입니다. MainWnd를 상속받아(is-a) 주 창의
//        메시지 처리를 그대로 이어받고, 여기에 전역 문자열 초기화,
//        창 클래스 등록, 주 창 생성, 메시지 루프를 추가합니다.
//
//  구조 (wndWorkSpaceOOP2 계승):
//        BaseWnd ──is-a── MainWnd ──is-a── Application
//        Application이 MainWnd를 상속하므로 별도 창 멤버 없이
//        자신이 곧 주 창이며, 창 등록/생성/메시지 루프를 담당합니다.
//
//  추가 자원 (AI_Test_Project_005):
//        그리기는 DoubleBuffer(더블버퍼)로 감싸고 House 클래스로 출력합니다.
//
class Application : public MainWnd
{
public:
    Application() = default;
    ~Application() override = default;

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    // 문자열을 읽고 창 클래스를 등록한 뒤 주 창을 만듭니다.
    BOOL Initialize(HINSTANCE hInstance, int nCmdShow);

    // 가속기 키를 적용하는 메시지 루프를 실행하고 종료 코드를 반환합니다.
    int Run();

private:
    HACCEL m_hAccelTable = nullptr;
    WCHAR  m_szTitle[MAX_LOADSTRING] = {};
    WCHAR  m_szWindowClass[MAX_LOADSTRING] = {};
};
