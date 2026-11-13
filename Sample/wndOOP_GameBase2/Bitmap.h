#pragma once

#include "framework.h"

//
//  클래스: Bitmap
//
//  용도: BMP 이미지를 관리·출력하는 싱글톤 이미지 매니저입니다. (순수 GDI 기반)
//        이름(key)으로 여러 이미지를 등록해두고, 필요할 때 key로 찾아 출력합니다.
//        흰색 배경은 TransparentBlt 컬러키로 투명 처리됩니다.
//
//  싱글턴:
//        이미지는 프로젝트 전체에서 하나의 저장소로 관리하는 것이 편하므로
//        GameManager/DoubleBuffer와 동일한 GetInstance() 싱글톤 패턴을 사용합니다.
//
//  저장 구조(기초학습용):
//        std::map 등 STL을 쓰지 않고, 고정 배열 + 선형검색으로 관리합니다.
//        (이전 도형 클래스들이 고정 배열을 쓰던 스타일과 동일)
//        이미지 수가 MAX_IMAGES를 넘으면 Load()는 실패(false)를 반환합니다.
//
//  사용 방식:
//        Bitmap::GetInstance().Load(L"Sheep", L"Sheep.bmp");
//        Bitmap::GetInstance().Draw(hdc, L"Sheep", 100, 100);
//
class Bitmap
{
public:
    static const int MAX_IMAGES = 32;   // 등록 가능한 최대 이미지 수
    static const int MAX_KEY    = 64;   // 이름(key) 최대 길이(널 포함)

    // 전역 인스턴스를 반환합니다. (Meyers Singleton)
    static Bitmap& GetInstance();

    Bitmap(const Bitmap&) = delete;
    Bitmap& operator=(const Bitmap&) = delete;

    // key 이름으로 이미지를 파일에서 불러와 등록합니다. 성공 시 true.
    bool Load(const wchar_t* key, const wchar_t* path);
    // key에 해당하는 이미지를 제거합니다.
    void Unload(const wchar_t* key);
    // 등록된 모든 이미지를 제거합니다.
    void UnloadAll();

    // key로 이미지를 찾았는지 여부를 반환합니다.
    bool IsLoaded(const wchar_t* key) const;

    // key 이미지의 원본 크기를 반환합니다. (찾지 못하면 0)
    int GetWidth(const wchar_t* key) const;
    int GetHeight(const wchar_t* key) const;

    // key 이미지의 투명 처리 컬러키를 설정합니다.
    void SetTransparentColor(const wchar_t* key, COLORREF color);

    // key 이미지를 원본 크기로 출력합니다.
    void Draw(HDC hdc, const wchar_t* key, int destX, int destY) const;
    // key 이미지를 지정 크기로 출력합니다. (전체 이미지)
    void Draw(HDC hdc, const wchar_t* key, int destX, int destY, int destW, int destH) const;
    // key 이미지의 일부(srcRect)만 잘라서 출력합니다. flip이 true면 좌우 반전합니다.
    void DrawFrame(HDC hdc, const wchar_t* key, int destX, int destY, int destW, int destH,
        int srcX, int srcY, int srcW, int srcH, bool flip) const;

private:
    Bitmap() = default;               // 싱글톤: 외부에서 생성 불가
    ~Bitmap();                        // 등록된 이미지 전체 해제

    // 등록된 이미지 한 개의 정보입니다.
    struct Entry
    {
        bool     used;                // 이 슬롯을 사용 중인지 여부
        wchar_t  key[MAX_KEY];        // 이미지 이름
        HBITMAP  hBitmap;             // 원본 비트맵
        HBITMAP  hFlipped;            // 좌우 반전 사본
        int      width;
        int      height;
        COLORREF transColor;          // 투명 처리 컬러키
    };

    Entry m_entries[MAX_IMAGES];       // 이미지 저장소 (고정 배열)

    // key를 가진 슬롯 인덱스를 선형검색으로 찾습니다. 못 찾으면 -1.
    int Find(const wchar_t* key) const;
    // 사용하지 않는 빈 슬롯 인덱스를 찾습니다. 못 찾으면 -1.
    int FindEmpty() const;
    // 슬롯의 반전 사본(hFlipped)을 만듭니다.
    bool CreateFlipped(Entry& e);
    // 슬롯의 비트맵 핸들들과 키를 비웁니다.
    void ClearEntry(Entry& e);
};
