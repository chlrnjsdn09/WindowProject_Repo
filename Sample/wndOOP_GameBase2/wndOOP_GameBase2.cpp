// wndOOP_GameBase2.cpp : 애플리케이션 진입점입니다.
//

#include "pch.h"
#include "framework.h"
#include "wndOOP_GameBase2.h"
#include "GameWnd.h"
#include "GameManager.h"

//
//  함수: wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
//
//  용도: 애플리케이션 진입점입니다. GameWnd 객체 하나를 만들어
//        초기화하고 게임 메시지 루프를 돌립니다.
//
//  구조 변화 (wndOOP_GameBase 대비):
//        원본: 전역변수(인스턴스/문자열/루프상태) + MyRegisterClass/InitInstance 전역함수
//        현재: GameWnd가 이 모두를 멤버로 소유하고, 진입점은 Initialize/Run 두 줄뿐
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    GameManager::GetInstance().Awake();

    GameWnd gameWnd;
    if (!gameWnd.Initialize(hInstance, nCmdShow))
    {
        return FALSE;
    }

    return gameWnd.Run();
}
