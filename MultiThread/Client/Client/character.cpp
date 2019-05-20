#include "character.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "MapManager.h"
#include <random>
#include "BubbleManager.h"
character::character()
{
	HEngine::ResourceManager::GetInstance()->SetAnimation("./RES/Down.bmp", 8, FALSE);
	HEngine::ResourceManager::GetInstance()->SetAnimation("./RES/Left.bmp", 8, FALSE);
	HEngine::ResourceManager::GetInstance()->SetAnimation("./RES/Right.bmp", 8, FALSE);
	HEngine::ResourceManager::GetInstance()->SetAnimation("./RES/Up.bmp", 8, FALSE);
	m_bitPlayer[DOWN] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Down.bmp");
	m_bitPlayer[UP] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Up.bmp");
	m_bitPlayer[RIGHT] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Right.bmp");
	m_bitPlayer[LEFT] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Left.bmp");
	m_bitPlayer[DIEBUBBLE] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Characbub1.bmp");
	m_bitPlayer[DIE] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Die.bmp");

	m_State = DOWN;
}


character::~character()
{
}

void character::Draw()
{
	if(m_State != END)
		m_bitPlayer[m_State]->Draw(m_DrawPt.x, m_DrawPt.y);
}

void character::PowerUp()
{
	m_BubblePower++;
}

int character::GetPower()
{
	return m_BubblePower;
}

int character::GetSpeed()
{
	return m_Speed;
}

HEngine::POINTF character::GetDrawPT()
{
	return m_DrawPt;
}

HEngine::POINT character::GetPt()
{
	return PlayerPt;
}

HEngine::POINT character::GetBackPt()
{
	return BackPt;
}

void character::SetState(DIRECT State)
{
	m_State = State;
	m_Speed = 300;
}
int character::GetState()
{
	return m_State;
}
void character::Init()
{
	m_fTime = 0.0f;
	m_bMove = true;
	m_Speed = 300;
	m_State = DOWN;
	m_BubblePower = 1;

	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_UP);
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_DOWN);
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_RIGHT);
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_LEFT);
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_SPACE);

}

bool character::Update()
{
	if (m_State == DIE || m_State == END)
		return false;
	
	if (m_State == DIEBUBBLE)
	{
		m_fTime += HEngine::TimeManager::GetInstance()->GetElipseTime();
		if (m_fTime >= 1.0f)
			m_State = DIE;
		return false;
	}


	/*부드럽게 이동*/
	if (m_DrawPt.x > PlayerPt.x * 40 + 20)
	{
		m_DrawPt.x -= HEngine::TimeManager::GetInstance()->GetElipseTime() * m_Speed;
		if (m_DrawPt.x <= PlayerPt.x * 40 + 20)
		{
			m_DrawPt.x = PlayerPt.x * 40 + 20;
			m_bMove = true;
		}
	}
	if (m_DrawPt.x < PlayerPt.x * 40 + 20)
	{
		m_DrawPt.x += HEngine::TimeManager::GetInstance()->GetElipseTime() * m_Speed;
		if (m_DrawPt.x >= PlayerPt.x * 40 + 20)
		{
			m_DrawPt.x = PlayerPt.x * 40 + 20;
			m_bMove = true;
		}
	}
	if (m_DrawPt.y > PlayerPt.y * 40)
	{
		m_DrawPt.y -= HEngine::TimeManager::GetInstance()->GetElipseTime() * m_Speed;
		if (m_DrawPt.y <= PlayerPt.y * 40)
		{
			m_DrawPt.y = PlayerPt.y * 40;
			m_bMove = true;
		}
	}
	if (m_DrawPt.y < PlayerPt.y * 40)
	{

		m_DrawPt.y += HEngine::TimeManager::GetInstance()->GetElipseTime() * m_Speed;
		if (m_DrawPt.y >= PlayerPt.y * 40)
		{
			m_DrawPt.y = PlayerPt.y * 40;
			m_bMove = true;
		}
	}


	/*방향 이동 완료시 다음 방향 이동 가능*/
	if (!m_bMove)
		return false;

	BackPt = PlayerPt;

	/*방향 이동*/
	if (HEngine::InputManager::GetInstance()->isKeyPress(VK_UP))
	{
		if (m_State != DIEBUBBLE)
			m_State = UP;
		if (PlayerPt.y <= 0)
			return false;

		HEngine::POINT pt = { PlayerPt.x, PlayerPt.y - 1 };
		if (MapManager::GetInstance()->GetType(pt) == EMPTY)
		{
			MapManager::GetInstance()->MovePlayer(BackPt, pt);
			PlayerPt.y--;
		}
		else if (MapManager::GetInstance()->GetType(pt) >= SPEED &&MapManager::GetInstance()->GetType(pt) <= BUBBLE_COUNT)
		{
			if (MapManager::GetInstance()->GetType(pt) == SPEED)
			{
				m_Speed += ITEM_SPEED;
				if (m_Speed >= MAX_SPEED)
					m_Speed = MAX_SPEED;
			}
			else if (MapManager::GetInstance()->GetType(pt) == POWER)
				m_BubblePower++;

			else if (MapManager::GetInstance()->GetType(pt) == BUBBLE_COUNT)
				BubbleManager::GetInstance()->PlusBubbleCount();

			MapManager::GetInstance()->MovePlayer(BackPt, pt);
			PlayerPt.y--;
		}
		else
			return false;
		m_bMove = false;



		return true;
	}
	if (HEngine::InputManager::GetInstance()->isKeyPress(VK_DOWN))
	{
		if (m_State != DIEBUBBLE)
			m_State = DOWN;
		if (PlayerPt.y >= 12)
			return false;

		HEngine::POINT pt = { PlayerPt.x, PlayerPt.y + 1 };
		if (MapManager::GetInstance()->GetType(pt) == EMPTY)
		{
			MapManager::GetInstance()->MovePlayer(BackPt, pt);
			PlayerPt.y++;
		}
		else if (MapManager::GetInstance()->GetType(pt) >= SPEED &&MapManager::GetInstance()->GetType(pt) <= BUBBLE_COUNT)
		{
			if (MapManager::GetInstance()->GetType(pt) == SPEED)
			{
				m_Speed += ITEM_SPEED;
				if (m_Speed >= MAX_SPEED)
					m_Speed = MAX_SPEED;
			}
			else if (MapManager::GetInstance()->GetType(pt) == POWER)	
				m_BubblePower++;

			else if (MapManager::GetInstance()->GetType(pt) == BUBBLE_COUNT)
				BubbleManager::GetInstance()->PlusBubbleCount();

			MapManager::GetInstance()->MovePlayer(BackPt, pt);
			PlayerPt.y++;
		}
		else
			return false;
		m_bMove = false;


		return true;
	}
	if (HEngine::InputManager::GetInstance()->isKeyPress(VK_RIGHT))
	{
		if (m_State != DIEBUBBLE)
			m_State = RIGHT;

		if (PlayerPt.x >= 14)
			return false;


		HEngine::POINT pt = { PlayerPt.x + 1, PlayerPt.y };
		if (MapManager::GetInstance()->GetType(pt) == EMPTY)
		{
			MapManager::GetInstance()->MovePlayer(BackPt, pt);
			PlayerPt.x++;
		}
		else if (MapManager::GetInstance()->GetType(pt) >= SPEED &&MapManager::GetInstance()->GetType(pt) <= BUBBLE_COUNT)
		{
			if (MapManager::GetInstance()->GetType(pt) == SPEED)
			{
				m_Speed += ITEM_SPEED;
				if (m_Speed >= MAX_SPEED)
					m_Speed = MAX_SPEED;
			}
			else if (MapManager::GetInstance()->GetType(pt) == POWER)
				m_BubblePower++;

			else if (MapManager::GetInstance()->GetType(pt) == BUBBLE_COUNT)
				BubbleManager::GetInstance()->PlusBubbleCount();

			MapManager::GetInstance()->MovePlayer(BackPt, pt);
			PlayerPt.x++;
		}
		else
			return false;
		m_bMove = false;

		return true;
	}
	if (HEngine::InputManager::GetInstance()->isKeyPress(VK_LEFT))
	{
		if (m_State != DIEBUBBLE)
			m_State = LEFT;

		if (PlayerPt.x <= 0)
			return false;

		HEngine::POINT pt = { PlayerPt.x - 1, PlayerPt.y };
		if (MapManager::GetInstance()->GetType(pt) == EMPTY)
		{
			MapManager::GetInstance()->MovePlayer(BackPt, pt);
			PlayerPt.x--;
		}
		else if (MapManager::GetInstance()->GetType(pt) >= SPEED &&MapManager::GetInstance()->GetType(pt) <= BUBBLE_COUNT)
		{
			if (MapManager::GetInstance()->GetType(pt) == SPEED)
			{
				m_Speed += ITEM_SPEED;
				if (m_Speed >= MAX_SPEED)
					m_Speed = MAX_SPEED;
			}
			else if (MapManager::GetInstance()->GetType(pt) == POWER)
				m_BubblePower++;

			else if (MapManager::GetInstance()->GetType(pt) == BUBBLE_COUNT)
				BubbleManager::GetInstance()->PlusBubbleCount();

			MapManager::GetInstance()->MovePlayer(BackPt, pt);
			PlayerPt.x--;
		}
		else
			return false;
		m_bMove = false;
		return true;
	}

	return false;
}

void character::SetPt(HEngine::POINT pt)
{
	PlayerPt.x = pt.x;
	PlayerPt.y = pt.y;
	m_DrawPt.x = PlayerPt.x * 40 + 10;
	m_DrawPt.y = PlayerPt.y * 40;
}