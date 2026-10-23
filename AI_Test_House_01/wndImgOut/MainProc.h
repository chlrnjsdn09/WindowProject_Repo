#pragma once

#include "BaseWnd.h"

//
//  클래스: MainWnd
//
//  용도: 주 창입니다. BaseWnd 파생 클래스로서
//        메뉴 명령 / 페인트 / 크기 변경 / 종료 메시지를 처리합니다.
//        그리기는 더블버퍼(DoubleBuffer)로 감싼 뒤 House 클래스로 출력합니다.
//
class MainWnd : public BaseWnd
{
public:
    MainWnd() = default;
    ~MainWnd() override = default;

protected:
    // 더블버퍼를 초기화합니다.
    LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct) override;

    // 파일 > 끝내기, 도움말 > 정보 메뉴를 처리합니다.
    LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;

    // 더블버퍼에 집을 그리고 화면으로 출력합니다.
    LRESULT OnPaint() override;

    // 더블버퍼 크기를 창 크기에 맞춥니다.
    LRESULT OnSize(UINT fwSizeType, int cx, int cy) override;

    // 배경 지우기를 막아 깜빡임을 방지합니다.
    LRESULT OnEraseBkgnd(HDC hdc) override;

    // 더블버퍼를 해제하고 애플리케이션 종료를 게시합니다.
    LRESULT OnDestroy() override;
};
