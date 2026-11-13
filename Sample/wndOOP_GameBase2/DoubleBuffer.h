#pragma once

#include "framework.h"

// 더블버퍼링용 싱글톤 클래스입니다.
// 메모리 DC + 호환 비트맵을 보관하고 WM_PAINT에서 BitBlt로 화면에 출력합니다.
class DoubleBuffer
{
private:
    HWND m_hWnd;
    HDC m_memDC;
    HBITMAP m_bitmap;
    HBITMAP m_oldBitmap;
    int m_width;
    int m_height;

    DoubleBuffer();
    ~DoubleBuffer();

    DoubleBuffer(const DoubleBuffer&) = delete;
    DoubleBuffer& operator=(const DoubleBuffer&) = delete;

    bool CreateBuffer(int width, int height);
    void DestroyBuffer();

public:
    static DoubleBuffer& GetInstance();

    bool Init(HWND hWnd);
    void Shutdown();
    bool Resize(int width, int height);

    HDC GetMemDC() const;
    int GetWidth() const;
    int GetHeight() const;

    void Clear(COLORREF color = RGB(255, 255, 255));
    void Present(HDC hdc);
};
