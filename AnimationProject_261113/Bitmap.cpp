// Bitmap.cpp : 싱글톤 이미지 매니저 Bitmap 클래스의 구현입니다. (순수 GDI 기반)
//

#include "pch.h"
#include "Bitmap.h"

#pragma comment(lib, "Msimg32.lib")

//
//  함수: Bitmap::GetInstance()
//
//  용도: 유일한 Bitmap(이미지 매니저) 인스턴스를 반환합니다.
//
Bitmap& Bitmap::GetInstance()
{
    static Bitmap instance;
    return instance;
}

Bitmap::~Bitmap()
{
    UnloadAll();
}

//
//  함수: Bitmap::Find(const wchar_t*)
//
//  용도: key와 일치하는 슬롯을 선형검색으로 찾습니다. 못 찾으면 -1.
//
int Bitmap::Find(const wchar_t* key) const
{
    if (key == nullptr)
    {
        return -1;
    }
    for (int i = 0; i < MAX_IMAGES; i++)
    {
        if (m_entries[i].used && wcscmp(m_entries[i].key, key) == 0)
        {
            return i;
        }
    }
    return -1;
}

//
//  함수: Bitmap::FindEmpty()
//
//  용도: 사용하지 않는 빈 슬롯 인덱스를 찾습니다. 못 찾으면 -1.
//
int Bitmap::FindEmpty() const
{
    for (int i = 0; i < MAX_IMAGES; i++)
    {
        if (!m_entries[i].used)
        {
            return i;
        }
    }
    return -1;
}

//
//  함수: Bitmap::ClearEntry(Entry&)
//
//  용도: 슬롯의 비트맵 핸들(원본/반전본)을 해제하고 키를 비웁니다.
//
void Bitmap::ClearEntry(Entry& e)
{
    if (e.hBitmap != nullptr)
    {
        DeleteObject(e.hBitmap);
        e.hBitmap = nullptr;
    }
    if (e.hFlipped != nullptr)
    {
        DeleteObject(e.hFlipped);
        e.hFlipped = nullptr;
    }
    e.key[0] = L'\0';
    e.width = 0;
    e.height = 0;
    e.used = false;
}

//
//  함수: Bitmap::Load(const wchar_t*, const wchar_t*)
//
//  용도: key 이름으로 이미지를 파일에서 불러와 빈 슬롯에 등록합니다.
//
bool Bitmap::Load(const wchar_t* key, const wchar_t* path)
{
    if (key == nullptr || path == nullptr)
    {
        return false;
    }

    // 같은 key가 이미 있으면 먼저 교체합니다.
    int slot = Find(key);
    if (slot < 0)
    {
        slot = FindEmpty();
        if (slot < 0)
        {
            return false; // 슬롯 부족
        }
    }
    else
    {
        ClearEntry(m_entries[slot]);
    }

    HBITMAP hBmp = (HBITMAP)LoadImageW(nullptr, path, IMAGE_BITMAP,
        0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    if (hBmp == nullptr)
    {
        m_entries[slot].used = false;
        return false;
    }

    BITMAP bm = {};
    GetObject(hBmp, sizeof(bm), &bm);

    Entry& e = m_entries[slot];
    e.used = true;
    wcsncpy_s(e.key, key, _TRUNCATE);
    e.hBitmap = hBmp;
    e.hFlipped = nullptr;
    e.width = bm.bmWidth;
    e.height = bm.bmHeight;
    e.transColor = RGB(255, 255, 255); // 기본 흰색 투명

    // 좌우 반전 출력용으로 미리 뒤집은 복사본을 만듭니다.
    if (!CreateFlipped(e))
    {
        ClearEntry(e);
        return false;
    }

    return true;
}

void Bitmap::Unload(const wchar_t* key)
{
    int slot = Find(key);
    if (slot >= 0)
    {
        ClearEntry(m_entries[slot]);
    }
}

void Bitmap::UnloadAll()
{
    for (int i = 0; i < MAX_IMAGES; i++)
    {
        if (m_entries[i].used)
        {
            ClearEntry(m_entries[i]);
        }
    }
}

bool Bitmap::IsLoaded(const wchar_t* key) const
{
    return Find(key) >= 0;
}

int Bitmap::GetWidth(const wchar_t* key) const
{
    int slot = Find(key);
    return (slot >= 0) ? m_entries[slot].width : 0;
}

int Bitmap::GetHeight(const wchar_t* key) const
{
    int slot = Find(key);
    return (slot >= 0) ? m_entries[slot].height : 0;
}

void Bitmap::SetTransparentColor(const wchar_t* key, COLORREF color)
{
    int slot = Find(key);
    if (slot >= 0)
    {
        m_entries[slot].transColor = color;
    }
}

//
//  함수: Bitmap::CreateFlipped(Entry&)
//
//  용도: 슬롯의 원본 비트맵을 좌우 반전한 복사본(hFlipped)을 만듭니다.
//
bool Bitmap::CreateFlipped(Entry& e)
{
    if (e.hBitmap == nullptr || e.width <= 0 || e.height <= 0)
    {
        return false;
    }

    HDC hdcScreen = GetDC(nullptr);
    if (hdcScreen == nullptr)
    {
        return false;
    }

    HDC hdcSrc = CreateCompatibleDC(hdcScreen);
    HDC hdcDst = CreateCompatibleDC(hdcScreen);
    HBITMAP hFlip = CreateCompatibleBitmap(hdcScreen, e.width, e.height);

    bool ok = false;
    if (hdcSrc != nullptr && hdcDst != nullptr && hFlip != nullptr)
    {
        HGDIOBJ hOldSrc = SelectObject(hdcSrc, e.hBitmap);
        HGDIOBJ hOldDst = SelectObject(hdcDst, hFlip);

        // 음수 너비로 복사하면 좌우가 뒤집힙니다.
        ok = StretchBlt(hdcDst, e.width, 0, -e.width, e.height,
            hdcSrc, 0, 0, e.width, e.height, SRCCOPY) != FALSE;

        SelectObject(hdcSrc, hOldSrc);
        SelectObject(hdcDst, hOldDst);
    }

    if (hdcSrc != nullptr) DeleteDC(hdcSrc);
    if (hdcDst != nullptr) DeleteDC(hdcDst);
    ReleaseDC(nullptr, hdcScreen);

    if (!ok)
    {
        if (hFlip != nullptr) DeleteObject(hFlip);
        return false;
    }

    e.hFlipped = hFlip;
    return true;
}

void Bitmap::Draw(HDC hdc, const wchar_t* key, int destX, int destY) const
{
    int slot = Find(key);
    if (slot < 0)
    {
        return;
    }
    Draw(hdc, key, destX, destY, m_entries[slot].width, m_entries[slot].height);
}

void Bitmap::Draw(HDC hdc, const wchar_t* key, int destX, int destY, int destW, int destH) const
{
    int slot = Find(key);
    if (slot < 0 || hdc == nullptr)
    {
        return;
    }
    if (destW <= 0 || destH <= 0)
    {
        return;
    }

    const Entry& e = m_entries[slot];

    HDC hdcMem = CreateCompatibleDC(hdc);
    if (hdcMem == nullptr)
    {
        return;
    }
    HGDIOBJ hOld = SelectObject(hdcMem, e.hBitmap);

    TransparentBlt(hdc, destX, destY, destW, destH,
        hdcMem, 0, 0, e.width, e.height, e.transColor);

    SelectObject(hdcMem, hOld);
    DeleteDC(hdcMem);
}

void Bitmap::DrawFrame(HDC hdc, const wchar_t* key, int destX, int destY, int destW, int destH,
    int srcX, int srcY, int srcW, int srcH, bool flip) const
{
    int slot = Find(key);
    if (slot < 0 || hdc == nullptr)
    {
        return;
    }
    if (destW <= 0 || destH <= 0 || srcW <= 0 || srcH <= 0)
    {
        return;
    }

    const Entry& e = m_entries[slot];

    // 뒤집힌 복사본은 좌우가 바뀌어 있으므로 원본 좌표도 뒤집어서 읽습니다.
    HBITMAP hSrc = flip ? e.hFlipped : e.hBitmap;
    int readX = flip ? (e.width - srcX - srcW) : srcX;

    HDC hdcMem = CreateCompatibleDC(hdc);
    if (hdcMem == nullptr)
    {
        return;
    }
    HGDIOBJ hOld = SelectObject(hdcMem, hSrc);

    TransparentBlt(hdc, destX, destY, destW, destH,
        hdcMem, readX, srcY, srcW, srcH, e.transColor);

    SelectObject(hdcMem, hOld);
    DeleteDC(hdcMem);
}
