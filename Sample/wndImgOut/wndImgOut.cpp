// wndImgOut.cpp : 애플리케이션(Application)과 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "wndImgOut.h"

//
//  함수: Application::Initialize(HINSTANCE, int)
//
//  용도: 전역 문자열을 초기화하고 창 클래스를 등록한 뒤 주 창을 만듭니다.
//        Application이 MainWnd(→BaseWnd)를 상속하므로 별도 창 객체 없이
//        자신(this)이 곧 생성되는 주 창입니다.
//
BOOL Application::Initialize(HINSTANCE hInstance, int nCmdShow)
{
    // 전역(멤버) 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WNDIMGOUT, m_szWindowClass, MAX_LOADSTRING);

    // 애플리케이션 초기화를 수행합니다:
    if (!RegisterWindowClass(hInstance, m_szWindowClass))
    {
        return FALSE;
    }

    if (!Create(hInstance, m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr))
    {
        return FALSE;
    }

    Show(nCmdShow);
    Update();

    m_hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDC_WNDIMGOUT));
    return TRUE;
}

//
//  함수: Application::Run()
//
//  용도: 가속기 키를 적용하는 기본 메시지 루프를 실행합니다.
//
int Application::Run()
{
    MSG msg = {};

    // 기본 메시지 루프입니다:
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        if (!TranslateAcceleratorW(msg.hwnd, m_hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}

//
//  함수: wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
//
//  용도: 애플리케이션 진입점입니다. Application 객체 하나를 만들어
//        초기화하고 메시지 루프를 돌립니다.
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Application app;

    if (!app.Initialize(hInstance, nCmdShow))
    {
        return FALSE;
    }

    return app.Run();
}
