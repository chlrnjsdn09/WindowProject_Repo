// DoubleBuffer.cpp : 더블버퍼링 싱글톤 클래스의 구현입니다.
//

#include "pch.h"
#include "DoubleBuffer.h"

DoubleBuffer::DoubleBuffer()
    : m_hWnd(nullptr)
    , m_memDC(nullptr)
    , m_bitmap(nullptr)
    , m_oldBitmap(nullptr)
    , m_width(0)
    , m_height(0)
{
}

DoubleBuffer::~DoubleBuffer()
{
    Shutdown();
}

DoubleBuffer& DoubleBuffer::GetInstance()
{
    static DoubleBuffer instance;
    return instance;
}

bool DoubleBuffer::Init(HWND hWnd)
{
    if (m_memDC != nullptr && m_hWnd == hWnd)
    {
        return true;
    }

    Shutdown();

    m_hWnd = hWnd;

    RECT rc = {};
    GetClientRect(m_hWnd, &rc);
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;
    if (width <= 0) width = 800;
    if (height <= 0) height = 600;

    return CreateBuffer(width, height);
}

void DoubleBuffer::Shutdown()
{
    DestroyBuffer();
    m_hWnd = nullptr;
}

bool DoubleBuffer::Resize(int width, int height)
{
    if (width <= 0 || height <= 0)
    {
        return false;
    }
    if (width == m_width && height == m_height && m_memDC != nullptr)
    {
        return true;
    }
    return CreateBuffer(width, height);
}

HDC DoubleBuffer::GetMemDC() const
{
    return m_memDC;
}

int DoubleBuffer::GetWidth() const
{
    return m_width;
}

int DoubleBuffer::GetHeight() const
{
    return m_height;
}

void DoubleBuffer::Clear(COLORREF color)
{
    if (m_memDC == nullptr)
    {
        return;
    }
    HBRUSH hBrush = CreateSolidBrush(color);
    RECT rc = { 0, 0, m_width, m_height };
    FillRect(m_memDC, &rc, hBrush);
    DeleteObject(hBrush);
}

void DoubleBuffer::Present(HDC hdc)
{
    if (hdc == nullptr || m_memDC == nullptr)
    {
        return;
    }
    BitBlt(hdc, 0, 0, m_width, m_height, m_memDC, 0, 0, SRCCOPY);
}

bool DoubleBuffer::CreateBuffer(int width, int height)
{
    DestroyBuffer();

    HDC hdc = GetDC(m_hWnd);
    if (hdc == nullptr)
    {
        return false;
    }

    m_memDC = CreateCompatibleDC(hdc);
    if (m_memDC == nullptr)
    {
        ReleaseDC(m_hWnd, hdc);
        return false;
    }

    m_bitmap = CreateCompatibleBitmap(hdc, width, height);
    ReleaseDC(m_hWnd, hdc);

    if (m_bitmap == nullptr)
    {
        DeleteDC(m_memDC);
        m_memDC = nullptr;
        return false;
    }

    m_oldBitmap = (HBITMAP)SelectObject(m_memDC, m_bitmap);
    m_width = width;
    m_height = height;

    Clear(RGB(255, 255, 255));
    return true;
}

void DoubleBuffer::DestroyBuffer()
{
    if (m_memDC != nullptr)
    {
        if (m_oldBitmap != nullptr)
        {
            SelectObject(m_memDC, m_oldBitmap);
            m_oldBitmap = nullptr;
        }
        DeleteDC(m_memDC);
        m_memDC = nullptr;
    }
    if (m_bitmap != nullptr)
    {
        DeleteObject(m_bitmap);
        m_bitmap = nullptr;
    }
    m_width = 0;
    m_height = 0;
}
