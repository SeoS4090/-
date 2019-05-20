#include "Enemy.h"
#include "ResManager.h"
#include "BitMap.h"
#include <random>
void Enemy::Init(IMG_RES info_enemy, int x, int y)
{
	m_Position.x = x;
	m_Position.y = y;
	m_enemy = info_enemy;
	m_Die = false;
	m_DieCount = 0;
	m_Direction = false;
	m_bMove;
	m_Score = 0;
	m_Life = 3;
	if (info_enemy == INFO_ENEMY1)
		m_Score = 10;
	else if (info_enemy == INFO_ENEMY2)
		m_Score = 20;
	else if (info_enemy == INFO_ENEMY3)
		m_Score = 30;
	else if (info_enemy == INFO_UFO)
	{
		srand(GetTickCount());
		m_Score = rand()%100;
	}
}

void Enemy::ReverseDirection(bool direction)
{
	m_Direction = direction;
	m_Position.y += 10;
}

void Enemy::Draw(HDC hdc)
{
	if (!m_Die)
	{
		if (m_enemy == INFO_UFO)
			ResManager::GetInstance()->GetImg(m_enemy)->Draw(hdc, m_Position.x, m_Position.y);
		else if (m_bMove && m_enemy != INFO_PLAYER)
			ResManager::GetInstance()->GetImg(m_enemy + 1)->Draw(hdc, m_Position.x, m_Position.y);
		else
			ResManager::GetInstance()->GetImg(m_enemy)->Draw(hdc, m_Position.x, m_Position.y);
	}
	else if (m_DieCount >= 0 && m_DieCount < 30)
	{
		ResManager::GetInstance()->GetImg(8 + m_DieCount / 10)->Draw(hdc, m_Position.x, m_Position.y);
		m_DieCount++;
	}

}


void Enemy::Update()
{
	if (m_Direction)
		m_Position.x += 16;
	else
		m_Position.x -= 16;

	m_bMove = !m_bMove;
}

void Enemy::Move(int key)
{
	if (key == VK_RIGHT && m_Position.x < GAME_WIDTH - BITMAP_WIDTH - 16)
		m_Position.x += 5;
	else if (key == VK_LEFT&& m_Position.x > 5)
		m_Position.x -= 5;

}

RECT Enemy::GetRect()
{
	RECT rt;
	rt.left = m_Position.x;
	rt.top = m_Position.y;
	rt.right = rt.left + 32;
	rt.bottom = rt.top + 32;
	return rt;
}

void Enemy::SetDie()
{
	m_Die = true;
}

void Enemy::Live()
{
	m_DieCount = 0;
	m_Die = false;
	m_Life--;
	m_Position.x = GAME_WIDTH / 2 - BITMAP_WIDTH / 2;
}
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}
