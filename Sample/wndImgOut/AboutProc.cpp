// AboutProc.cpp : 정보 대화 상자(AboutDialog)의 구현입니다.
//

#include "pch.h"
#include "framework.h"
#include "wndImgOut.h"
#include "AboutProc.h"

//
//  함수: AboutDialog::Show(HINSTANCE, HWND)
//
//  용도: 정보 대화 상자를 모달로 표시합니다.
//
INT_PTR AboutDialog::Show(HINSTANCE hInstance, HWND hOwner)
{
    return DialogBoxW(hInstance, MAKEINTRESOURCEW(IDD_ABOUTBOX), hOwner, DlgProc);
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK AboutDialog::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
