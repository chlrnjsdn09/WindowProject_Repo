// BlueBoy.cpp : 걷기 애니메이션 캐릭터의 구현입니다.
//

#include "pch.h"
#include "BlueBoy.h"
#include "Bitmap.h"

namespace
{
    // Bitmap 싱글턴에 등록할 이미지 이름입니다.
    const wchar_t* const BB_KEY = L"BlueBoy";

    // 걷기 행(두번째 행, y=456~816)에서 실측한 프레임 좌표입니다.
    // 시작x = 24, 226, 417, 627, 813  (프레임3↔4 경계 x≈812 확인)
    // 프레임 폭이 제각각이라 출력 시 높이 기준으로 비율을 맞춥니다.
    struct Frame
    {
        int x;
        int y;
        int w;
        int h;
    };

    // (수정) 이미지를 바로잡은 뒤 걷기 행은 y=387~669(높이 283)입니다.
    // 이전 값(y=456, h=361)은 아래 행(y>=716)의 머리까지 포함해 애니메이션이 깨졌습니다.
    const Frame WALK_FRAMES[BlueBoy::MAX_FRAMES] = {
        {  95, 385, 152, 286 },   // 프레임0 (캐릭터 x=99..242)
        { 280, 385, 152, 286 },   // 프레임1 (x=284..427)
        { 455, 385, 164, 286 },   // 프레임2 (x=459..614)
        { 647, 385, 160, 286 },   // 프레임3 (x=651..802)
        { 828, 385, 166, 286 },   // 프레임4 (x=832..989)
    };
}

BlueBoy::BlueBoy(int x, int y, int minX, int maxX)
    : m_frame(0)
    , m_lastAni(0)
    , m_aniInterval(200)        // 애니메이션: 초당 5프레임 (200ms)
    , m_lastMove(0)
    , m_moveInterval(33)        // 이동: 약 30fps (애니메이션과 분리해 부드럽게)
    , m_x(x), m_y(y), m_dir(-1), m_speed(2)
    , m_minX(minX), m_maxX(maxX)
    , m_destH(170)              // 시트 프레임이 361px 이므로 절반 이하로 줄입니다.
{
}

//
//  함수: BlueBoy::Load()
//
//  용도: Bitmap 싱글턴에 걷기 이미지를 등록하고, 마젠타를 투명 컬러키로 지정합니다.
//
bool BlueBoy::Load(const wchar_t* path)
{
    if (!Bitmap::GetInstance().Load(BB_KEY, path))
    {
        return false;
    }

    // 캐릭터에 존재하지 않는 마젠타를 투명 처리 컬러키로 사용합니다.
    Bitmap::GetInstance().SetTransparentColor(BB_KEY, RGB(255, 0, 255));
    return true;
}

bool BlueBoy::IsLoaded() const
{
    return Bitmap::GetInstance().IsLoaded(BB_KEY);
}

void BlueBoy::Unload()
{
    Bitmap::GetInstance().Unload(BB_KEY);
}

void BlueBoy::SetRange(int minX, int maxX)
{
    m_minX = minX;
    m_maxX = maxX;
}

void BlueBoy::SetPos(int x, int y)
{
    m_x = x;
    m_y = y;
}

int BlueBoy::GetX() const
{
    return m_x;
}

int BlueBoy::GetDir() const
{
    return m_dir;
}

//
//  함수: BlueBoy::Update()
//
//  용도: 애니메이션과 이동의 간격을 따로 계산합니다.
//        - 애니메이션: 초당 5프레임(200ms)으로 걷기 프레임을 계속 반복합니다.
//        - 이동: 더 짧은 간격(33ms)으로 계산해 프레임이 느려도 걸음은 부드럽게 움직입니다.
//        간격이 지나면 좌우 경계에 닿는 방향으로 이동하고 방향을 반전합니다.
//
void BlueBoy::Update()
{
    if (!IsLoaded())
    {
        return;
    }

    DWORD now = GetTickCount();

    // 애니메이션: 초당 5프레임(200ms)으로 걷기 프레임을 반복 전환합니다.
    if (now - m_lastAni >= m_aniInterval)
    {
        m_lastAni = now;
        m_frame = (m_frame + 1) % MAX_FRAMES;
    }

    // 이동: 애니메이션과 간격을 따로 두어 부드럽게 움직입니다.
    if (now - m_lastMove < m_moveInterval)
    {
        return;
    }
    m_lastMove = now;

    // 좌우 왕복 이동
    m_x += m_dir * m_speed;
    if (m_x >= m_maxX)
    {
        m_x = m_maxX;
        m_dir = -1;   // 오른쪽 끝 → 왼쪽으로
    }
    else if (m_x <= m_minX)
    {
        m_x = m_minX;
        m_dir = 1;    // 왼쪽 끝 → 오른쪽으로
    }
}

//
//  함수: BlueBoy::Draw()
//
//  용도: 현재 걷기 프레임을 발·중심 기준으로 합성 DC에 그립니다.
//        시트의 걷기 프레임은 오른쪽을 보고 있어서 왼쪽으로 이동할 때 뒤집습니다.
//
void BlueBoy::Draw(HDC hdc) const
{
    if (!IsLoaded())
    {
        return;
    }

    const Frame& f = WALK_FRAMES[m_frame];

    // 프레임 폭이 제각각이라 높이를 기준으로 비율을 맞춥니다.
    int destW = f.w * m_destH / f.h;
    int destX = m_x - destW / 2;    // 발 중심 기준으로 가로 정렬
    int destY = m_y - m_destH;      // 발 밑 기준으로 세로 정렬

    // 시트가 오른쪽을 보고 있으므로 왼쪽으로 갈 때 뒤집어야
    // 캐릭터가 항상 이동하는 방향을 바라봅니다.
    bool flip = (m_dir < 0);

    Bitmap::GetInstance().DrawFrame(hdc, BB_KEY, destX, destY, destW, m_destH,
        f.x, f.y, f.w, f.h, flip);
}
