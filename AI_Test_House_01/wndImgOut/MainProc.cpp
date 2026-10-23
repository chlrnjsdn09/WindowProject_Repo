// MainProc.cpp : 주 창(MainWnd)의 메시지 처리 구현입니다.
//                집 그리기와 더블버퍼링은 AI_Test_Project_005에서 가져왔습니다.
//

#include "pch.h"
#include "framework.h"
#include "wndImgOut.h"
#include "MainProc.h"
#include "AboutProc.h"
#include "DoubleBuffer.h"
#include "House.h"

// 집 객체입니다. (House 클래스가 005의 DrawHouse 로직을 담당합니다)
static House g_house;

//
//  함수: MainWnd::OnCreate(LPCREATESTRUCT)
//
//  용도: 주 창 생성 시 더블버퍼를 초기화합니다.
//
LRESULT MainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    UNREFERENCED_PARAMETER(lpCreateStruct);
    DoubleBuffer::GetInstance().Init(GetHwnd());
    return 0;
}

//
//  함수: MainWnd::OnCommand(WPARAM, LPARAM)
//
//  용도: 메뉴/액셀러레이터 명령을 처리합니다.
//        처리하지 않은 명령은 DefWindowProc으로 되돌려 보냅니다.
//
LRESULT MainWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
    const int wmId = LOWORD(wParam);

    // 메뉴 선택을 구문 분석합니다:
    switch (wmId)
    {
    case IDM_ABOUT:
        AboutDialog::Show(GetInstance(), GetHwnd());
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
//  함수: MainWnd::OnPaint()
//
//  용도: 더블버퍼 메모리 DC에 집을 그리고 화면으로 한 번에 출력합니다.
//        더블버퍼를 만들 수 없으면 hdc에 직접 그립니다.
//
LRESULT MainWnd::OnPaint()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(GetHwnd(), &ps);

    DoubleBuffer& buffer = DoubleBuffer::GetInstance();
    if (buffer.GetMemDC() == nullptr)
    {
        buffer.Init(GetHwnd());
    }

    HDC memDC = buffer.GetMemDC();
    if (memDC != nullptr)
    {
        buffer.Clear(RGB(255, 255, 255));
        g_house.Draw(memDC);
        buffer.Present(hdc);
    }
    else
    {
        // 더블버퍼 실패 시 직접 그립니다.
        g_house.Draw(hdc);
    }

    EndPaint(GetHwnd(), &ps);
    return 0;
}

//
//  함수: MainWnd::OnSize(UINT, int, int)
//
//  용도: 창 크기가 바뀌면 더블버퍼 크기를 다시 만듭니다.
//
LRESULT MainWnd::OnSize(UINT fwSizeType, int cx, int cy)
{
    UNREFERENCED_PARAMETER(fwSizeType);
    DoubleBuffer::GetInstance().Resize(cx, cy);
    return 0;
}

//
//  함수: MainWnd::OnEraseBkgnd(HDC)
//
//  용도: 배경을 미리 지우지 않아 깜빡임을 방지합니다.
//
LRESULT MainWnd::OnEraseBkgnd(HDC hdc)
{
    UNREFERENCED_PARAMETER(hdc);
    return 1;
}

//
//  함수: MainWnd::OnDestroy()
//
//  용도: 더블버퍼를 해제하고 창이 닫힐 때 메시지 루프를 종료합니다.
//
LRESULT MainWnd::OnDestroy()
{
    DoubleBuffer::GetInstance().Shutdown();
    PostQuitMessage(0);
    return 0;
}
