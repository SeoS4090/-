#include "Bullet.h"
#include "ResManager.h"
#include "BitMap.h"


void Bullet::Init(int x, int y, bool enemy)
{
	m_Position.x = x;
	m_Position.y = y;

	m_bDie = false;
	m_bEnemy = enemy;
}

void Bullet::Update()
{
	
	if (m_bDie)
		return;

	if(!m_bEnemy)
		m_Position.y -= 5;
	else
		m_Position.y += 5;

	if (m_Position.y < 0 || m_Position.y > GAME_HEIGHT)
	{
		m_bDie = true;

	}
}

void Bullet::Draw(HDC hdc)
{
	if (!m_bDie)
	{
		if (!m_bEnemy)
			ResManager::GetInstance()->GetImg(INFO_BULLET2)->Draw(hdc, m_Position.x, m_Position.y);
		else
			ResManager::GetInstance()->GetImg(INFO_BULLET)->Draw(hdc, m_Position.x, m_Position.y);
	}
	else
	{
		if (!m_bEnemy)
			ResManager::GetInstance()->GetImg(INFO_BULLET2_B)->Draw(hdc, m_Position.x, m_Position.y);
		else
			ResManager::GetInstance()->GetImg(INFO_BULLET_B)->Draw(hdc, m_Position.x, m_Position.y);
	}
}

RECT Bullet::GetRect()
{

	RECT rt;
	rt.left = m_Position.x;
	rt.top = m_Position.y;
	rt.right = rt.left + 5;
	rt.bottom = rt.top +10;

	return rt;
}

bool Bullet::GetDie()
{
	return m_bDie;
}

bool Bullet::CheckCollider(RECT rt)
{
	
	if (IntersectRect(&SaveRt, &GetRect(), &rt) > 0)
		return true;

	return false;
}

Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}
