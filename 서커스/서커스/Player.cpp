#include "Player.h"
#include "ResManager.h"
#include "BitMap.h"
#include "resource.h"
#include <math.h>
enum PLAYER_STATE
{
	PLAYER_WAIT,
	PLAYER_RUN,
	PLAYER_RUN2,
	PLAYER_WIN,
	PLAYER_WIN2,
	PLAYER_DIE,
	PLAYER_RJUMP,
	PLAYER_LJUMP,
	PLAYER_UJUMP
};
RECT Player::GetRect()
{
	RECT rt;
	rt.left = m_Position.x - m_Move + 10;
	rt.top = m_Position.y;
	rt.right = rt.left + 46;
	rt.bottom = rt.top + 63;

	return rt;
}
Player::Player()
{
}


Player::~Player()
{
}
float Radian(int theta)
{
	float radian;
	radian = 3.141f / 180 * theta;
	return radian;
}


void Player::Init()
{
	Turn_Count = 0;
	m_Position.x = 50;
	m_Position.y = 100 + 183 + 67 - 63;
	m_State = PLAYER_WAIT;
	m_Move = 0;
	m_JumpPoint.x = m_Position.x;
	m_JumpPoint.y = m_Position.y + 50;
	m_Radian = 20;
	m_bjump = false;
	m_jumpDirection = PLAYER_UJUMP;
}

void Player::Draw(HDC hdc)
{
	if (m_State == PLAYER_WAIT)
		ResManager::GetInstance()->GetImg(PLAYER0)->Draw(hdc, m_Position.x - m_Move, m_Position.y);
	else if (m_State == PLAYER_RUN)
		ResManager::GetInstance()->GetImg(PLAYER1)->Draw(hdc, m_Position.x - m_Move, m_Position.y);
	else if (m_State == PLAYER_RUN2)
		ResManager::GetInstance()->GetImg(PLAYER2)->Draw(hdc, m_Position.x - m_Move, m_Position.y);
	else if (m_State == PLAYER_WIN)
		ResManager::GetInstance()->GetImg(WIN)->Draw(hdc, m_Position.x - m_Move, m_Position.y);
	else if (m_State == PLAYER_WIN2)
		ResManager::GetInstance()->GetImg(WIN2)->Draw(hdc, m_Position.x - m_Move, m_Position.y);
	else if (m_State == PLAYER_DIE)
		ResManager::GetInstance()->GetImg(DIE)->Draw(hdc, m_Position.x - m_Move, m_Position.y);
}

void Player::SetPosition(int move)
{
	m_Move = move;
	m_Position.x = move + 50;
	m_Position.y = 100 + 183 + 67 - 63;
	m_JumpPoint.x = m_Position.x;
	m_JumpPoint.y = m_Position.y + 50;
	m_Radian = 20;
	m_State = PLAYER_WAIT;
}

void Player::Move(int key)
{
	if (m_bjump)
	{
		return;
	}

	if (m_Position.x > LENGTH * 66 - 76)
	{
		m_Position.x--;
		m_Position.y = 100 + 64 + 183 - 50 - 60;
	}

	if (key == VK_RIGHT)
	{
		m_Position.x +=2;
		if ((LENGTH - 16) * 66 +30 > m_Move)
			m_Move += 2;
		if (Turn_Count > 3)
		{
			if (m_State == PLAYER_RUN)
				m_State = PLAYER_RUN2;
			else if (m_State == PLAYER_RUN2)
				m_State = PLAYER_WAIT;
			else if (m_State == PLAYER_WAIT)
				m_State = PLAYER_RUN;
			Turn_Count = 0;
		}
		Turn_Count++;
		m_JumpPoint.x = -cosf(Radian(180)) * 100 + m_Position.x;
		m_jumpDirection = PLAYER_RJUMP;
	}

	if (key == VK_LEFT)
	{
		if (m_Move > 0)
		{
			m_Position.x--;
			if ((LENGTH - 16) * 66 + 30 > m_Move)
				m_Move--;
			else if(m_Position.x - m_Move == 50)
				m_Move--;
		}
		if (Turn_Count > 6)
		{
			if (m_State == PLAYER_RUN2)
				m_State = PLAYER_WAIT;
			if (m_State == PLAYER_RUN)
				m_State = PLAYER_WAIT;
			else if (m_State == PLAYER_WAIT)
				m_State = PLAYER_RUN;
			Turn_Count = 0;
		}
		Turn_Count++;
		m_JumpPoint.x = m_Position.x + cosf(Radian(180)) * 100;
		m_jumpDirection = PLAYER_LJUMP;
	}


	if (key == VK_UP)
	{
		m_bjump = true;
	}

	if (key == 0)
	{
		m_jumpDirection = PLAYER_UJUMP;
	}
}

void Player::Jump()
{
	if (m_State == PLAYER_DIE)
	{
		m_bjump = false;
		return;
	}

	//위로 점프
	if (m_jumpDirection == PLAYER_UJUMP)
	{
		if (m_Radian < 160)
		{
			//플레이어 모양 바꿈
			m_State = PLAYER_RUN2;
			//위치 이동 위로만
			m_Position.y = -sinf(Radian(m_Radian)) * 150 + m_JumpPoint.y;
			//각도 올리기
			
			m_Radian++;
			m_Radian++;
		}
		else
		{
			//플레이어 모양
			m_State = PLAYER_WAIT;
			//각도 초기화
			m_Radian = 20;
			m_bjump = false;
			m_JumpPoint.x = -cosf(Radian(180)) * 100 + m_Position.x;

		}
	}
	//우측 점프
	if (m_jumpDirection == PLAYER_RJUMP)
	{
		if (m_Radian < 160)
		{
			//플레이어 모양 바꿈
			m_State = PLAYER_RUN2;
			//위치 가로
			m_Position.x = -cosf(Radian(m_Radian)) * 100 + m_JumpPoint.x;
			if ((LENGTH - 16) * 66 +30 > m_Move)
				m_Move = -cosf(Radian(m_Radian)) * 100 + m_JumpPoint.x - 50;
			//위치 이동 위
			m_Position.y = -sinf(Radian(m_Radian)) * 150 + m_JumpPoint.y;
			//각도 올리기
			m_Radian++;
			m_Radian++;
		}
		else
		{
			//플레이어 모양
			m_State = PLAYER_WAIT;
			//각도 초기화
			m_Radian = 20;
			m_bjump = false;
			m_JumpPoint.x = -cosf(Radian(180)) * 100 + m_Position.x - m_Move;
			m_jumpDirection = PLAYER_UJUMP;
		}
	}

	//좌측 점프
	if (m_jumpDirection == PLAYER_LJUMP)
	{
		if (m_Radian < 160)
		{
			//플레이어 모양 바꿈
			m_State = PLAYER_RUN2;
			//위치 가로

			if (m_Move > 0)
			{
				m_Position.x = cosf(Radian(m_Radian)) * 100 + m_JumpPoint.x;
				if ((LENGTH - 16) * 66 > m_Move)
					m_Move = cosf(Radian(m_Radian)) * 100 + m_JumpPoint.x - 50;
				if (m_Position.x - m_Move <= 50)
					m_Move = cosf(Radian(m_Radian)) * 100 + m_JumpPoint.x - 50;

			}
			//위치 이동 위
			m_Position.y = -sinf(Radian(m_Radian)) * 150 + m_JumpPoint.y;
			//각도 올리기
			m_Radian++;
		}
		else
		{
			//플레이어 모양
			m_State = PLAYER_WAIT;
			//각도 초기화
			m_Radian = 20;
			m_bjump = false;
			m_JumpPoint.x = -cosf(Radian(180)) * 100 + m_Position.x - m_Move;
			m_jumpDirection = PLAYER_UJUMP;
		}
	}
}

void Player::Die()
{
	m_State = PLAYER_DIE;
	m_bjump = false;
}

void Player::Win()
{
	m_bjump = false;

	if (m_Position.x > LENGTH * 66 - 76)
		m_Position.x--;

	if (Turn_Count > 6)
	{
		if (m_State == PLAYER_WIN2)
			m_State = PLAYER_WIN;
		else
			m_State = PLAYER_WIN2;
		Turn_Count = 0;
	}
	Turn_Count++;
}
