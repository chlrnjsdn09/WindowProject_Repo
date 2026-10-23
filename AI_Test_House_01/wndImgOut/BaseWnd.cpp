// BaseWnd.cpp : BaseWnd 기본 클래스의 구현입니다.
//

#include "pch.h"
#include "framework.h"
#include "Resource.h"
#include "BaseWnd.h"

//
//  함수: BaseWnd::RegisterWindowClass(HINSTANCE, LPCWSTR)
//
//  용도: 파생 클래스가 지정한 이름으로 창 클래스를 등록합니다.
//        메시지 프로시저로는 정적 썽크 StaticWndProc를 사용합니다.
//
BOOL BaseWnd::RegisterWindowClass(HINSTANCE hInstance, LPCWSTR className)
{
    WNDCLASSEXW wcex = {};

    wcex.cbSize        = sizeof(WNDCLASSEXW);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = StaticWndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_WNDIMGOUT));
    wcex.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName  = MAKEINTRESOURCEW(IDC_WNDIMGOUT);
    wcex.lpszClassName = className;
    wcex.hIconSm       = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_SMALL));

    return RegisterClassExW(&wcex) != 0;
}

//
//  함수: BaseWnd::Create(HINSTANCE, ...)
//
//  용도: 창을 생성합니다. CREATESTRUCT의 lpCreateParams로 this를 전달해
//        WM_NCCREATE 시점에 GWLP_USERDATA에 연결합니다.
//
BOOL BaseWnd::Create(HINSTANCE hInstance, LPCWSTR className, LPCWSTR windowName, DWORD style,
                     int x, int y, int width, int height, HMENU hMenu)
{
    m_hInstance = hInstance;

    m_hWnd = CreateWindowW(className, windowName, style,
                           x, y, width, height,
                           nullptr, hMenu, hInstance, this);

    return m_hWnd != nullptr;
}

void BaseWnd::Show(int nCmdShow)
{
    if (m_hWnd != nullptr)
    {
        ShowWindow(m_hWnd, nCmdShow);
    }
}

void BaseWnd::Update()
{
    if (m_hWnd != nullptr)
    {
        UpdateWindow(m_hWnd);
    }
}

//
//  함수: BaseWnd::HandleMessage(UINT, WPARAM, LPARAM)
//
//  용도: 메시지를 가상 핸들러로 라우팅하고, 처리하지 않은 메시지는
//        DefWindowProc에 전달합니다.
//
LRESULT BaseWnd::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        return OnCreate(reinterpret_cast<LPCREATESTRUCT>(lParam));
    case WM_COMMAND:
        return OnCommand(wParam, lParam);
    case WM_PAINT:
        return OnPaint();
    case WM_SIZE:
        return OnSize(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
    case WM_ERASEBKGND:
        return OnEraseBkgnd(reinterpret_cast<HDC>(wParam));
    case WM_DESTROY:
        return OnDestroy();
    default:
        break;
    }
    return DefWindowProcW(m_hWnd, message, wParam, lParam);
}

//
//  함수: BaseWnd::StaticWndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 모든 창이 공유하는 정적 메시지 프로시저입니다.
//        GWLP_USERDATA에서 this를 찾아 인스턴스의 핸들러로 전달합니다.
//
LRESULT CALLBACK BaseWnd::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // 최초 메시지(WM_NCCREATE)에서는 CreateWindow에 전달된 this로 연결합니다.
    if (message == WM_NCCREATE)
    {
        auto* cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
        auto* pThis = static_cast<BaseWnd*>(cs->lpCreateParams);
        if (pThis != nullptr)
        {
            pThis->m_hWnd = hWnd;
            SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        }
    }

    BaseWnd* pThis = FromHandle(hWnd);
    if (pThis != nullptr)
    {
        pThis->m_hWnd = hWnd;
        const LRESULT result = pThis->HandleMessage(message, wParam, lParam);

        // 창이 파괴된 뒤에는 연결을 해제해 dangling 참조를 막습니다.
        if (message == WM_NCDESTROY)
        {
            SetWindowLongPtrW(hWnd, GWLP_USERDATA, 0);
            pThis->m_hWnd = nullptr;
        }
        return result;
    }

    return DefWindowProcW(hWnd, message, wParam, lParam);
}

BaseWnd* BaseWnd::FromHandle(HWND hWnd)
{
    return reinterpret_cast<BaseWnd*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
}
