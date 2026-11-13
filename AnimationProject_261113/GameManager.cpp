// GameManager.cpp : 게임 루프 상태와 라이프사이클을 담당하는 싱글톤 클래스의 구현입니다.
//

#include "pch.h"
#include "GameManager.h"

#include <string.h> // wcsrchr/wcscat_s (경로 조립용)

//
//  비트맵 원본 데이터입니다.
//
//  원본에서는 파일 전역 배열이었으나, 헤더를 깨끗하게 유지하기 위해
//  익명 네임스페이스 안에 두어 이 번역단위(.cpp)에서만 보이게 했습니다.
//
namespace
{
    //이미지 자체인데, 1bit이미지 //흑백
    //BYTE bitImage[] = {
    //	0xC3, 0xFF, 
    //	0xBD, 0xFF,
    //	0x66, 0xFF,
    //	0x5A, 0xFF,
    //	0x5A, 0xFF,
    //	0x66, 0xFF,
    //	0xBD, 0xFF,
    //	0xC3, 0xFF,
    //};

    BYTE bitImage[][16] = {
        {
        0xE7, 0xFF,
        0xC3, 0xFF,
        0x81, 0xFF,
        0x24, 0xFF,
        0x00, 0xFF,
        0xC3, 0xFF,
        0x81, 0xFF,
        0x5A, 0xFF,
        },
        {
        0xE7, 0xFF,
        0xC3, 0xFF,
        0x81, 0xFF,
        0x12, 0xFF,
        0x00, 0xFF,
        0xC3, 0xFF,
        0x81, 0xFF,
        0xAD, 0xFF,
        }
    };
}

//
//  함수: GameManager::GetInstance()
//
//  용도: 유일한 GameManager 인스턴스를 반환합니다.
//        함수 내 static 변수를 사용하므로 첫 호출 시 생성되고,
//        프로그램 종료 시 자동으로 소멸합니다. (Meyers Singleton)
//
GameManager& GameManager::GetInstance()
{
    static GameManager instance;
    return instance;
}

void GameManager::Awake()
{
}

//
//  함수: ResolveImagePath() (파일 전역 헬퍼)
//
//  용도: 이미지 파일 경로를 찾습니다. 1) 실행 파일 폴더  2) 현재 작업 폴더
//        (Visual Studio에서 실행하면 작업 폴더가 프로젝트 폴더가 됩니다.)
//
static void ResolveImagePath(const wchar_t* fileName, wchar_t* out, size_t count)
{
    wcsncpy_s(out, count, fileName, _TRUNCATE); //기본: 현재 작업 폴더

    wchar_t exe[MAX_PATH] = L"";
    if (GetModuleFileNameW(nullptr, exe, MAX_PATH) > 0)
    {
        wchar_t* slash = wcsrchr(exe, L'\\');
        if (slash != nullptr)
        {
            *(slash + 1) = L'\0';
            wcscat_s(exe, MAX_PATH, fileName);
            if (GetFileAttributesW(exe) != INVALID_FILE_ATTRIBUTES)
            {
                wcsncpy_s(out, count, exe, _TRUNCATE); //실행 폴더를 우선합니다.
            }
        }
    }
}

void GameManager::Start(HWND hWnd)
{
    m_hWnd = hWnd; // 게임이 붙을 메인 윈도우 핸들만 기억해 둡니다.

    RECT rc = {};
    GetClientRect(m_hWnd, &rc);
    m_width = rc.right - rc.left;
    m_height = rc.bottom - rc.top;
    if (m_width <= 0)  m_width  = GetSystemMetrics(SM_CXSCREEN);
    if (m_height <= 0) m_height = GetSystemMetrics(SM_CYSCREEN);

    m_ScreenDC = GetDC(m_hWnd); //화면DC 선언

    //이미지를 모으는 작업
    m_MemoryDC = CreateCompatibleDC(m_ScreenDC); //메모리 DC 생성
    m_hBitmap = CreateCompatibleBitmap(m_ScreenDC, m_width, m_height); //컬러 캐릭터 출력용
    SelectObject(m_MemoryDC, m_hBitmap); //DC와 비트맵 연결 //포인터와 데이터 연결

    //배경이미지 생성 
    m_BkImageDC = CreateCompatibleDC(m_ScreenDC); //흰색 배경이미지 DC 생성
    m_hBkImage = CreateCompatibleBitmap(m_ScreenDC, m_width, m_height); //컬러 배경
    SelectObject(m_BkImageDC, m_hBkImage);//DC와 비트맵 연결 //포인터와 데이터 연결

    //실제이미지 생성
    m_ImageDC[0] = CreateCompatibleDC(m_ScreenDC); //이미지 DC 생성
    m_hImage[0] = CreateBitmap(8, 8, 1, 1, bitImage[0]); //1bit 흑백이미지 1면 너비 8 높이 8을 bitImage 데이터를 가지고 생성
    SelectObject(m_ImageDC[0], m_hImage[0]);//DC와 비트맵 연결 //포인터와 데이터 연결
    m_ImageDC[1] = CreateCompatibleDC(m_ScreenDC); //이미지 DC 생성
    m_hImage[1] = CreateBitmap(8, 8, 1, 1, bitImage[1]); //1bit 흑백이미지 1면 너비 8 높이 8을 bitImage 데이터를 가지고 생성
    SelectObject(m_ImageDC[1], m_hImage[1]);//DC와 비트맵 연결 //포인터와 데이터 연결

    // 걷기 캐릭터 이동 범위/위치를 데스크톱 크기에 맞게 설정합니다.
    m_blueBoy.SetRange(100, m_width - 100);
    // 작업표시줄 위(작업 영역 바닥)를 걷도록 합니다.
    RECT work = {};
    int groundY = m_height - 20;
    if (SystemParametersInfoW(SPI_GETWORKAREA, 0, &work, 0))
    {
        groundY = work.bottom;
    }
    m_blueBoy.SetPos(m_width / 2, groundY);

    //걷기 캐릭터 로드 (실행 폴더 → 현재 폴더 순으로 찾습니다.)
    wchar_t charPath[MAX_PATH] = L"";
    ResolveImagePath(L"BlueBoy.bmp", charPath, MAX_PATH);
    m_blueBoy.Load(charPath);
}

void GameManager::Update()
{
    m_blueBoy.Update(); //걷기 캐릭터 갱신 (내부에서 애니메이션/이동 속도를 조절합니다)

    // 흰색 배경으로 초기화 (흰색은 윈도우 컬러키로 투명 처리됩니다)
    PatBlt(m_MemoryDC, 0, 0, m_width, m_height, WHITENESS);

    //===================================================================
    //FPS 및 프레임 표시
    m_nCount++;
    wchar_t strTemp[128] = L"";
    wsprintf(strTemp, L"FPS = %06d, frame = %06d", m_nFPS, m_nCount);
    TextOut(m_MemoryDC, 0, 0, strTemp, (int)wcslen(strTemp));
    //===================================================================

    m_blueBoy.Draw(m_MemoryDC); //걷기 캐릭터 출력

    BitBlt(m_ScreenDC, 0, 0, m_width, m_height, m_MemoryDC, 0, 0, SRCCOPY); //메모리DC 모아진 이미지를 화면에 출력
}

void GameManager::Release()
{
    m_blueBoy.Unload(); //걷기 캐릭터 이미지 해제 (Bitmap 싱글턴에서 제거)

    DeleteObject(m_hImage[0]); m_hImage[0] = nullptr; //실제 이미지 데이터 지우기
    if (m_ImageDC[0] != nullptr) { DeleteDC(m_ImageDC[0]); m_ImageDC[0] = nullptr; } //실제 이미지 DC(포인터) 지우기 (CreateCompatibleDC로 생성)
    DeleteObject(m_hImage[1]); m_hImage[1] = nullptr; //실제 이미지 데이터 지우기
    if (m_ImageDC[1] != nullptr) { DeleteDC(m_ImageDC[1]); m_ImageDC[1] = nullptr; } //실제 이미지 DC(포인터) 지우기 (CreateCompatibleDC로 생성)

    DeleteObject(m_hBkImage); m_hBkImage = nullptr;  //배경 이미지 데이터 지우기
    if (m_BkImageDC != nullptr) { DeleteDC(m_BkImageDC); m_BkImageDC = nullptr; } //배경 이미지 DC(포인터) 지우기 (CreateCompatibleDC로 생성)

    DeleteObject(m_hBitmap); m_hBitmap = nullptr; //모으는 DC 데이터 지우기
    if (m_MemoryDC != nullptr) { DeleteDC(m_MemoryDC); m_MemoryDC = nullptr; } //모으는 DC(포인터) 지우기 (CreateCompatibleDC로 생성)

    if (m_ScreenDC != nullptr) { ReleaseDC(m_hWnd, m_ScreenDC); m_ScreenDC = nullptr; } //화면 DC 지우기 //메인 DC지우기 (GetDC로 획득)
}

//
//  함수: GameManager::OnFpsTimer()
//
//  용도: 1초마다 호출되어 그동안 그린 프레임 수(m_nCount)를 초당 프레임에 기록합니다.
//
void GameManager::OnFpsTimer()
{
    m_nFPS = m_nCount;
    m_nCount = 0;
}

//
//  함수: GameManager::ToggleAnimation()
//
//  용도: 애니메이션 프레임 인덱스를 0 <-> 1로 토글합니다.
//
void GameManager::ToggleAnimation()
{
    (m_nAniOn == 0) ? m_nAniOn = 1 : m_nAniOn = 0;
}
