// GameWnd.cpp : 게임용 주 창(GameWnd)의 구현입니다.
//                진입점 초기화(Initialize), 게임루프(Run), 메시지 핸들러를 담당합니다.
//

#include "pch.h"
#include "framework.h"
#include "GameWnd.h"
#include "AboutProc.h"
#include "GameManager.h"

//
//  함수: GameWnd::Initialize(HINSTANCE, int)
//
//  용도: 전역(멤버) 문자열을 초기화하고 창 클래스를 등록한 뒤 주 창을 만듭니다.
//        가속기 테이블도 미리 불러옵니다.
//
BOOL GameWnd::Initialize(HINSTANCE hInstance, int nCmdShow)
{
    // 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WNDOOPGAMEBASE2, m_szWindowClass, MAX_LOADSTRING);

    // 창 클래스를 등록합니다. (BaseWnd가 StaticWndProc으로 등록)
    if (!RegisterWindowClass(hInstance, m_szWindowClass))
    {
        return FALSE;
    }

    // 주 창을 만듭니다. (내부에서 this를 GWLP_USERDATA에 연결)
    if (!Create(hInstance, m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr))
    {
        return FALSE;
    }

    Show(nCmdShow);
    Update();

    m_hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDC_WNDOOPGAMEBASE2));
    return TRUE;
}

//
//  함수: GameWnd::Run()
//
//  용도: PeekMessage 기반 게임 메시지 루프를 실행합니다. (원본 GameMsgLoop와 동일)
//        메시지가 없고 창이 활성 상태일 때마다 GameManager::Update()를 호출해
//        화면을 갱신합니다.
//
int GameWnd::Run()
{
    MSG msg = {};

    while (m_bIsLoopExit)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE))
        {
            if (GetMessage(&msg, nullptr, 0, 0))
            {
                if (!TranslateAcceleratorW(msg.hwnd, m_hAccelTable, &msg))
                {
                    TranslateMessage(&msg);
                    DispatchMessageW(&msg);
                }
                else return (int)msg.wParam;
            }
        }
        else if (m_bIsActive)
        {
            GameManager::GetInstance().Update();
        }
        else WaitMessage();
    }
    return (int)msg.wParam;
}

//
//  함수: GameWnd::OnCreate(LPCREATESTRUCT)
//
//  용도: 창이 만들어질 때 게임 자원을 준비하고 타이머를 켭니다.
//        (원본 MainProc::OnCreate와 동일한 일을 수행)
//
LRESULT GameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    UNREFERENCED_PARAMETER(lpCreateStruct);

    GameManager::GetInstance().Start(GetHwnd()); // 윈도우 핸들과 함께 게임 자원을 생성합니다.
    SetTimer(GetHwnd(), 0, 1000, NULL);  // FPS 측정용 (1초)
    SetTimer(GetHwnd(), 1, 300, NULL);   // 애니메이션 전환용 (0.3초)
    m_bIsActive = TRUE;
    return 0;
}

//
//  함수: GameWnd::OnCommand(WPARAM, LPARAM)
//
//  용도: 메뉴/가속기 명령을 처리합니다.
//
LRESULT GameWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
    const int wmId = LOWORD(wParam);

    switch (wmId)
    {
    case IDM_ABOUT:
        DialogBoxW(GetInstance(), MAKEINTRESOURCEW(IDD_ABOUTBOX), GetHwnd(), About);
        break;
    case IDM_EXIT:
        DestroyWindow(GetHwnd());
        break;
    default:
        return DefWindowProcW(GetHwnd(), WM_COMMAND, wParam, lParam);
    }
    return 0;
}

//
//  함수: GameWnd::OnPaint()
//
//  용도: PeekMessage 게임루프가 주 그리기를 담당하므로, 여기서는
//        윈도우가 요구할 때의 최소 동작(BeginPaint/EndPaint)만 수행합니다.
//
LRESULT GameWnd::OnPaint()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(GetHwnd(), &ps);
    UNREFERENCED_PARAMETER(hdc);
    EndPaint(GetHwnd(), &ps);
    return 0;
}

//
//  함수: GameWnd::OnTimer(WPARAM, LPARAM)
//
//  용도: 타이머 이벤트를 GameManager에 전달합니다.
//        case 0: FPS 측정, case 1: 애니메이션 프레임 전환
//
LRESULT GameWnd::OnTimer(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (wParam)
    {
    case 0:
        GameManager::GetInstance().OnFpsTimer();
        break;
    case 1:
        GameManager::GetInstance().ToggleAnimation();
        break;
    }
    return 0;
}

//
//  함수: GameWnd::OnDestroy()
//
//  용도: 창이 닫힐 때 게임 자원을 해제하고 타이머를 끈 뒤 애플리케이션을 종료합니다.
//
LRESULT GameWnd::OnDestroy()
{
    GameManager::GetInstance().Release();
    m_bIsActive = FALSE;
    m_bIsLoopExit = FALSE;
    KillTimer(GetHwnd(), 1);
    KillTimer(GetHwnd(), 0);
    PostQuitMessage(0);
    return 0;
}
