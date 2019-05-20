#include "Bullet.h"
#include "ResManager.h"
#include "BitMap.h"

void Bullet::Init(int x, int y, INFO_BITMAP Direct)
{
	m_Position.x = x;
	m_Position.y = y;
 	m_iDirect = Direct;

	if (Direct == TANK_UP_00 || Direct == TANK_UP_01 || Direct == E_UP_00 || Direct == E_UP_01)
		m_iDirect = MISSILE_00;
	if (Direct == TANK_DOWN_00 || Direct == TANK_DOWN_01 || Direct == E_DOWN_00 || Direct == E_DOWN_01)
		m_iDirect = MISSILE_02;
	if (Direct == TANK_LEFT_00 || Direct == TANK_LEFT_01 || Direct == E_LEFT_00 || Direct == E_LEFT_01)
		m_iDirect = MISSILE_01;
	if (Direct == TANK_RIGHT_00 || Direct == TANK_RIGHT_01 || Direct == E_RIGHT_00 || Direct == E_RIGHT_01)
		m_iDirect = MISSILE_03;

	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();

	m_Fade = false;
	m_ExplosionCount = 0;
}
void Bullet::Update()
{
	
	if (m_Position.x >= MAP_WIDTH || m_Position.x <= 10|| m_Position.y >= MAP_HEIGHT || m_Position.y <= 10)
		SetFade();


	m_dwCurTime = GetTickCount();
	m_fElapseTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = m_dwCurTime;

	if (m_Fade)
	{
		m_ExplosionCount += m_fElapseTime;
		return;
	}

	if (m_iDirect == MISSILE_00)
		m_Position.y -= 300 * m_fElapseTime;
	if (m_iDirect == MISSILE_02)
		m_Position.y += 300 * m_fElapseTime;
	if (m_iDirect == MISSILE_01)
		m_Position.x -= 300 * m_fElapseTime;
	if (m_iDirect == MISSILE_03)
		m_Position.x += 300 * m_fElapseTime;
}
void Bullet::Draw(HDC hdc)
{
	if (!m_Fade)
		ResManager::GetInstance()->GetImg(m_iDirect)->Draw(hdc, m_Position.x, m_Position.y);
	else if (m_ExplosionCount > 0.3)
		return;
	else if (m_ExplosionCount > 0.2)
	{
		ResManager::GetInstance()->GetImg(EXPLOSION_02)->Draw(hdc, m_Position.x, m_Position.y);
	}
	else if (m_ExplosionCount > 0.1)
	{
		ResManager::GetInstance()->GetImg(EXPLOSION_01)->Draw(hdc, m_Position.x, m_Position.y);
	}
 	else if (m_ExplosionCount > 0)
	{
 		ResManager::GetInstance()->GetImg(EXPLOSION_00)->Draw(hdc, m_Position.x, m_Position.y);
	}
}

void Bullet::SetFade()	
{
	m_Fade = true;
	m_Position.x -= 33 / 2;
	m_Position.y -= 25 / 2;
}

bool Bullet::CheckCollider(RECT rect)
{
	RECT m_rect;
	m_rect.left = m_Position.x;
	m_rect.top = m_Position.y;
	m_rect.right = m_rect.left + 10;
	m_rect.bottom = m_rect.top + 10;
	RECT recbuf;
	if (IntersectRect(&recbuf, &rect, &m_rect) != 0)
	{
		SetFade();
		
		
		
	
		return true;
	}
	return false;
}

Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}
