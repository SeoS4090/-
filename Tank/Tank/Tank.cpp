#include "Tank.h"
#include "ResManager.h"
#include "BitMap.h"
#include <random>

void Tank::Init(INFO_BITMAP id, int x, int y)
{
	m_id = id;
	m_FirstPosition.x = x;
	m_FirstPosition.y = y;
	m_Position.x = x;
	m_Position.y = y;
	m_PrevPosition = m_Position;

	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();

	m_AIDirect = 0;
	m_Die = false;
	m_bBarrier = false;
	if (m_id >= TANK_DOWN_00 && m_id <= TANK_UP_01)
		m_iLife = PLAYER_LIFE;
	else
		m_iLife = 1;

	m_fDuringTime = 0;
	m_bRespone = true;
}

void Tank::Update()
{
	m_dwCurTime = GetTickCount();
	m_fElapseTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = m_dwCurTime;
	m_fDuringTime += m_fElapseTime;

	if (m_bRespone && m_fDuringTime > 1)
	{
		m_bRespone = false;
		m_fDuringTime = 0;
	}

	if (m_bBarrier && m_fDuringTime > 1.5)
		m_bBarrier = false;
}

void Tank::TankMove(vector <INFO*> vecMap)
{
	if (m_Die || m_bRespone)
		return;

	auto iter = vecMap.begin();
	for (; iter != vecMap.end(); iter++)
	{
		RECT rt, rtbuf;
		rt.left = (*iter)->bitmap_Point.x;
		rt.top = (*iter)->bitmap_Point.y;
		rt.right = rt.left + BITMAP_XSIZE;
		rt.bottom = rt.top + BITMAP_YSIZE;
		if ((*iter)->bitmap_id == BLOCK01 || (*iter)->bitmap_id == BLOCK09)
			rt.bottom = rt.bottom - (BITMAP_YSIZE / 2);
		if ((*iter)->bitmap_id == BLOCK02 || (*iter)->bitmap_id == BLOCK10)
			rt.right = rt.right - (BITMAP_XSIZE / 2);
		if ((*iter)->bitmap_id == BLOCK03 || (*iter)->bitmap_id == BLOCK11)
			rt.top = rt.top + (BITMAP_YSIZE / 2);
		if ((*iter)->bitmap_id == BLOCK04 || (*iter)->bitmap_id == BLOCK12)
			rt.left = rt.left + (BITMAP_XSIZE / 2);

		if (!(*iter)->bitmap_Fade && IntersectRect(&rtbuf, &rt, &GetRect()) != 0)
		{
			m_Position = m_PrevPosition;
			break;
		}
	}

	if (iter == vecMap.end())
	{
		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			if (m_id == TANK_LEFT_01)
				m_id = TANK_LEFT_00;
			else
				m_id = TANK_LEFT_01;

			m_PrevPosition.x = m_Position.x;
			if (m_Position.x > 10)
				m_Position.x -= 100 * m_fElapseTime;

			return;
		}
		if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			if (m_id == TANK_RIGHT_01)
				m_id = TANK_RIGHT_00;
			else
				m_id = TANK_RIGHT_01;

			m_PrevPosition.x = m_Position.x;
			if (m_Position.x < MAP_WIDTH + 10 - BITMAP_XSIZE)
				m_Position.x += 140 * m_fElapseTime;

			return;
		}
		if (GetKeyState(VK_UP) & 0x8000)
		{
			if (m_id == TANK_UP_01)
				m_id = TANK_UP_00;
			else
				m_id = TANK_UP_01;

			m_PrevPosition.y = m_Position.y;
			if (m_Position.y > 10)
				m_Position.y -= 120 * m_fElapseTime;

			return;
		}
		if (GetKeyState(VK_DOWN) & 0x8000)
		{
			if (m_id == TANK_DOWN_01)
				m_id = TANK_DOWN_00;
			else
				m_id = TANK_DOWN_01;
			m_PrevPosition.y = m_Position.y;
			if (m_Position.y < MAP_HEIGHT - BITMAP_YSIZE)
				m_Position.y += 140 * m_fElapseTime;

			return;
		}

	}
}

void Tank::Draw(HDC hdc)
{
	if(m_bRespone)
		ResManager::GetInstance()->GetImg(RESPONE00 + (int)(m_fDuringTime*10)%2)->Draw(hdc, m_Position.x, m_Position.y);
	else if (!m_Die)
		ResManager::GetInstance()->GetImg(m_id)->Draw(hdc, m_Position.x, m_Position.y);

	else if (m_fDuringTime > 0.3)
	{
		m_Position = m_FirstPosition;
		return;
	}

	else if (m_fDuringTime > 0.2)
	{
		ResManager::GetInstance()->GetImg(EXPLOSION_04)->Draw(hdc, m_Position.x + BITMAP_XSIZE / 2 - 35, m_Position.y + BITMAP_YSIZE / 2 - 27);
	}
	else if (m_fDuringTime > 0.1)
	{
		ResManager::GetInstance()->GetImg(EXPLOSION_03)->Draw(hdc, m_Position.x + BITMAP_XSIZE / 2 - 34, m_Position.y + BITMAP_YSIZE / 2 - 25);
	}
	else if (m_fDuringTime > 0)
	{
		ResManager::GetInstance()->GetImg(EXPLOSION_02)->Draw(hdc, m_Position.x, m_Position.y);
	}
	if (m_bBarrier && (int)(m_fDuringTime * 100) % 2 == 0 && !m_bRespone)
	{
		ResManager::GetInstance()->GetImg(SHIELD_00)->Draw(hdc, m_Position.x, m_Position.y);
	}
	else if (m_bBarrier && !m_bRespone)
	{
		ResManager::GetInstance()->GetImg(SHIELD_01)->Draw(hdc, m_Position.x, m_Position.y);
	}
}

void Tank::AIUpdate(vector <INFO*> vecMap)
{
	if (m_Die || m_bRespone)
		return;

	srand(GetTickCount());

	if (rand() % 100 == 0)
		m_AIDirect = rand() % 4;

	auto iter = vecMap.begin();
	for (; iter != vecMap.end(); iter++)
	{
		RECT rt, rtbuf;
		rt.left = (*iter)->bitmap_Point.x;
		rt.top = (*iter)->bitmap_Point.y;
		rt.right = rt.left + BITMAP_XSIZE;
		rt.bottom = rt.top + BITMAP_YSIZE;
		if ((*iter)->bitmap_id == BLOCK01 || (*iter)->bitmap_id == BLOCK09)
			rt.bottom = rt.bottom - (BITMAP_YSIZE / 2);
		if ((*iter)->bitmap_id == BLOCK02 || (*iter)->bitmap_id == BLOCK10)
			rt.right = rt.right - (BITMAP_XSIZE / 2);
		if ((*iter)->bitmap_id == BLOCK03 || (*iter)->bitmap_id == BLOCK11)
			rt.top = rt.top + (BITMAP_YSIZE / 2);
		if ((*iter)->bitmap_id == BLOCK04 || (*iter)->bitmap_id == BLOCK12)
			rt.left = rt.left + (BITMAP_XSIZE / 2);

		if (!(*iter)->bitmap_Fade && IntersectRect(&rtbuf, &rt, &GetRect()) != 0)
		{
			m_Position = m_PrevPosition;
			m_AIDirect = rand() & 4;
			break;
		}
	}

	if (iter != vecMap.end())
	{
		m_AIDirect = rand() % 4;
	}

	if (iter == vecMap.end())
	{
		if (m_AIDirect == 0)
		{
			if (m_id == E_LEFT_01)
				m_id = E_LEFT_00;
			else
				m_id = E_LEFT_01;

			m_PrevPosition.x = m_Position.x;
			if (m_Position.x > 10 && m_Position.x - 100 * m_fElapseTime  > 10)
				m_Position.x -= 100 * m_fElapseTime;
			else
				m_AIDirect = rand() % 4;


			return;
		}
		if (m_AIDirect == 1)
		{
			if (m_id == E_RIGHT_01)
				m_id = E_RIGHT_00;
			else
				m_id = E_RIGHT_01;

			m_PrevPosition.x = m_Position.x;
			if (m_Position.x < MAP_WIDTH + 10 - BITMAP_XSIZE && m_Position.x + 140 * m_fElapseTime  < MAP_WIDTH + 10 - BITMAP_XSIZE)
				m_Position.x += 140 * m_fElapseTime;
			else
				m_AIDirect = rand() % 4;

			return;
		}
		if (m_AIDirect == 2)
		{
			if (m_id == E_UP_01)
				m_id = E_UP_00;
			else
				m_id = E_UP_01;

			m_PrevPosition.y = m_Position.y;
			if (m_Position.y > 10 && m_Position.y - 120 * m_fElapseTime  > 10)
				m_Position.y -= 120 * m_fElapseTime;
			else
				m_AIDirect = rand() % 4;

			return;
		}
		if (m_AIDirect == 3)
		{
			if (m_id == E_DOWN_01)
				m_id = E_DOWN_00;
			else
				m_id = E_DOWN_01;
			m_PrevPosition.y = m_Position.y;
			if (m_Position.y < MAP_HEIGHT + 10 - BITMAP_YSIZE&& m_Position.y + 140 * m_fElapseTime  < MAP_HEIGHT - BITMAP_YSIZE)
				m_Position.y += 140 * m_fElapseTime;
			else
				m_AIDirect = rand() % 4;

			return;
		}

	}
}

bool Tank::Resurrection()
{

	if (m_fDuringTime > 0.5)
	{
		m_Position.x = m_FirstPosition.x;
		m_Position.y = m_FirstPosition.y;
		m_PrevPosition = m_Position;

		m_dwLastTime = GetTickCount();
		m_dwCurTime = GetTickCount();

		m_Die = false;
		m_fDuringTime = 0;
		m_bBarrier = true;
		return true;
	}
	return false;
}

void Tank::ChangeDirect()
{
	while (true) 
	{
		srand(GetTickCount());
		int direct = rand() % 4;
		if (direct != m_AIDirect)
		{
			m_AIDirect = direct;
			break;
		}
	}
}

Tank::Tank()
{
}


Tank::~Tank()
{
}
