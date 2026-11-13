#pragma once

//
//  클래스: AboutDialog
//
//  용도: 정보 대화 상자를 캡슐화합니다.
//        DialogBox는 함수 포인터를 요구하므로 정적 멤버 프로시저를 사용합니다.
//
class AboutDialog
{
public:
    // 정보 대화 상자를 모달로 표시하고 닫힘 결과를 반환합니다.
    static INT_PTR Show(HINSTANCE hInstance, HWND hOwner);

private:
    static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};
