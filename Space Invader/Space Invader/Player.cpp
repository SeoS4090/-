#include "Player.h"
#include "resource.h"
#include "ResManager.h"
#include "BitMap.h"
void Player::Init()
{
	m_Position.x = (GAME_WIDTH) / 2 - 16;
	m_Position.y = GAME_HEIGHT - BITMAP_HEIGHT - 50;
	m_PlayerCenter = m_Position.x + 16;
	m_Die = false;
	m_Count = 0;
	m_Life = 3;
}

void Player::Live()
{
	m_Position.x = (GAME_WIDTH) / 2 - 16;
	m_Position.y = GAME_HEIGHT - BITMAP_HEIGHT - 50;
	m_PlayerCenter = m_Position.x + 16;
	m_Die = false;
	m_Count = 0;

}

void Player::Die()
{
	m_Die = true;
	m_Life--;
}

void Player::Draw(HDC hdc)
{
	if(!m_Die)
		ResManager::GetInstance()->GetImg(INFO_PLAYER)->Draw(hdc, m_Position.x, m_Position.y);
	else if (m_Count >= 0 && m_Count < 30)
	{
		ResManager::GetInstance()->GetImg(8 + m_Count / 10)->Draw(hdc, m_Position.x, m_Position.y);
		m_Count++;
	}

	for (int i = 0; i < m_Life; i++)
	{
		ResManager::GetInstance()->GetImg(INFO_PLAYER)->Draw(hdc, i * 50 , GAME_HEIGHT- 50);
	}
}

void Player::Move(int key)
{
	if (key == VK_RIGHT && m_Position.x < GAME_WIDTH - BITMAP_WIDTH - 16)
		m_Position.x += 5;
	else if (key == VK_LEFT&& m_Position.x > 5)
		m_Position.x -= 5;

	if (key == VK_SPACE)
	{
		m_PlayerCenter = m_Position.x + 16;
	}
}

RECT Player::GetRect()
{
	RECT rt;
	rt.left = m_Position.x;
	rt.top = m_Position.y;
	rt.right = rt.left + 32;
	rt.bottom = rt.top + 32;
	return rt;
}

int Player::GetPlayerCenter()
{
	return m_PlayerCenter;
}

void Player::Release()
{
}

Player::Player()
{
}


Player::~Player()
{
}
