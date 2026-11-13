#pragma once

//
//  클래스: BaseWnd
//
//  용도: Win32 창 하나를 캡슐화하는 기본(기반) 클래스입니다.
//        리소스 ID(아이콘/메뉴)는 Resource.h의 프로젝트 상수를 사용합니다.
//        파생 클래스는 가상 핸들러(OnCommand, OnPaint 등)를 재정의해
//        각 창에 맞는 동작을 구현합니다.
//
//  파생 관계:
//        BaseWnd  <-  MainWnd  <-  Application
//        (애플리케이션과 주 창이 하나의 객체로 이어지는 is-a 계층)
//
//  메시지 전달 방식:
//        WndProc는 정적 썽크(StaticWndProc)로 동작하며, 창의
//        GWLP_USERDATA에 저장된 this 포인터를 찾아 해당 인스턴스의
//        가상 핸들러로 메시지를 전달합니다. 따라서 전역 변수 없이도
//        여러 창 인스턴스를 각각 처리할 수 있습니다.
//
class BaseWnd
{
public:
    BaseWnd() = default;
    virtual ~BaseWnd() = default;

    BaseWnd(const BaseWnd&) = delete;
    BaseWnd& operator=(const BaseWnd&) = delete;

    // 창 클래스를 등록합니다.
    BOOL RegisterWindowClass(HINSTANCE hInstance, LPCWSTR className);

    // 창을 생성합니다. 내부에서 this를 GWLP_USERDATA에 연결합니다.
    BOOL Create(HINSTANCE hInstance, LPCWSTR className, LPCWSTR windowName, DWORD style,
                int x, int y, int width, int height, HMENU hMenu = nullptr);

    // 창을 표시하고 갱신합니다.
    void Show(int nCmdShow);
    void Update();

    HWND      GetHwnd()     const { return m_hWnd; }
    HINSTANCE GetInstance() const { return m_hInstance; }

    // 창 클래스에 등록할 프로시저입니다. (Application에서 사용)
    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
    // ---- 파생 클래스가 재정의하는 가상 핸들러입니다. ----
    virtual LRESULT OnCreate(LPCREATESTRUCT /*lpCreateStruct*/) { return 0; }
    virtual LRESULT OnCommand(WPARAM /*wParam*/, LPARAM /*lParam*/) { return 0; }
    virtual LRESULT OnPaint() { return 0; }
    virtual LRESULT OnSize(UINT /*fwSizeType*/, int /*cx*/, int /*cy*/) { return 0; }
    virtual LRESULT OnEraseBkgnd(HDC /*hdc*/) { return 0; }
    virtual LRESULT OnDestroy() { return 0; }

    // 가상 핸들러로 메시지를 라우팅합니다. (파생 클래스에서 메시지를 확장할 때 사용)
    virtual LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);

private:
    static BaseWnd* FromHandle(HWND hWnd);

    HWND      m_hWnd = nullptr;
    HINSTANCE m_hInstance = nullptr;
};
