#include "Fire.h"
#include "ResManager.h"
#include "BitMap.h"
#include "Mecro.h"
#include "Player.h"
#include <random>
void Fire::Init()
{
	m_bTurn = true;
	m_iTurn_Count = 0;
	m_iMove = 0;
	m_bWin = false;
	m_bCash = false;
	m_bDie = false;
	m_Cashturn = 0;
	srand(GetTickCount());

	int m_irandom = rand();

	for (int i = 0; i < FIRE_COUNT; i++)
	{
		m_irandom = rand();
		arr[i].m_bPrev = false;
		arr[i].m_Fire1 = new BitMap();
		arr[i].m_Fire2 = new BitMap();
		if ((m_irandom % 3 == 0 || m_irandom % 3 == 1) && i < FIRE_COUNT-2)
		{
			arr[i].m_Fire1 = ResManager::GetInstance()->GetImg(ENEMY_1B);
			arr[i].m_Fire2 = ResManager::GetInstance()->GetImg(ENEMY_B);
			arr[i].m_iMove.x = i * 300 + 300;
			arr[i].m_iMove.y = 100 + 64;
			arr[i].m_InitPosition = arr[i].m_iMove.x;

			arr[i + 1].m_Fire1 = ResManager::GetInstance()->GetImg(ENEMY_1F);
			arr[i + 1].m_Fire2 = ResManager::GetInstance()->GetImg(ENEMY_F);
			arr[i + 1].m_iMove.x = arr[i].m_iMove.x +26;
			arr[i + 1].m_iMove.y = 100 + 64;
			arr[i + 1].m_InitPosition = arr[i + 1].m_iMove.x;

			
			if (m_irandom % 3 == 1 && i < FIRE_COUNT-3)
			{
				arr[i + 2].m_Fire1 = ResManager::GetInstance()->GetImg(CASH);
				arr[i + 2].m_Fire2 = ResManager::GetInstance()->GetImg(CASH);
				arr[i + 2].m_iMove.x = arr[i].m_iMove.x + 15;
				arr[i + 2].m_iMove.y = 100 + 64 + 35;
				arr[i + 2].m_InitPosition = arr[i + 2].m_iMove.x;
				i += 2;
			}
			else
				i++;
			
		}
		
		else 
		{

			if (i * 300 + 300 > (LENGTH * 66) / (LENGTH / 10)*i && i * 300 + 300 < (LENGTH * 66) / (LENGTH / 10)*i + 172 / 2)
				i++;

			arr[i].m_Fire1 = ResManager::GetInstance()->GetImg(FRONT);
			arr[i].m_Fire2 = ResManager::GetInstance()->GetImg(FRONT2);
			arr[i].m_iMove.x = i * 300 + 300;
			arr[i].m_iMove.y = 100 + 64 + 183 - 50;
			arr[i].m_InitPosition = arr[i].m_iMove.x;
		}
	}

	arr[FIRE_COUNT - 2].m_Fire1 = ResManager::GetInstance()->GetImg(FRONT);
	arr[FIRE_COUNT - 2].m_Fire2 = ResManager::GetInstance()->GetImg(FRONT2);
	arr[FIRE_COUNT - 2].m_iMove.x = LENGTH * 66 - 76 - 50 -5;
	arr[FIRE_COUNT - 2].m_iMove.y = 100 + 64 + 183 - 50;
	arr[FIRE_COUNT - 2].m_InitPosition = arr[FIRE_COUNT - 2].m_iMove.x;
	arr[FIRE_COUNT - 1].m_Fire1 = ResManager::GetInstance()->GetImg(END);
	arr[FIRE_COUNT - 1].m_Fire2 = ResManager::GetInstance()->GetImg(END);
	arr[FIRE_COUNT - 1].m_iMove.x = LENGTH * 66 - 76;
	arr[FIRE_COUNT - 1].m_iMove.y = 100 + 64 + 183 - 50;
	arr[FIRE_COUNT - 1].m_InitPosition = arr[FIRE_COUNT - 1].m_iMove.x;
}



void Fire::SetPosition(int x)
{
	m_iMove = x;
	m_bDie = false;
	
	for (int i = 0; i < FIRE_COUNT; i++)
	{
		
			arr[i].m_iMove.x = arr[i].m_InitPosition;
	}
}

void Fire::Draw(HDC hdc, Player *player)
{
	for (int i = 0; i < FIRE_COUNT; i++)
	{
		if (arr[i].m_iMove.x - m_iMove > -50)
		{
			if (arr[i].m_iMove.x - m_iMove > 150)
				arr[i].m_bPrev = false;

			if (!arr[i].m_bPrev && arr[i].m_Fire1 == ResManager::GetInstance()->GetImg(ENEMY_1B))
			{
				if (m_bTurn)
				{
					arr[i].m_Fire1->Draw(hdc, arr[i].m_iMove.x - m_iMove, arr[i].m_iMove.y);
				}
				else
				{
					arr[i].m_Fire2->Draw(hdc, arr[i].m_iMove.x - m_iMove, arr[i].m_iMove.y);
				}

			}
		}
		else
		{
			arr[i].m_bPrev = true;
		}
	}

	player->Draw(hdc);

	for (int i = 0; i < FIRE_COUNT; i++)
	{
		if (arr[i].m_iMove.x - m_iMove > -50)
		{
			if (arr[i].m_iMove.x - m_iMove > 150)
				arr[i].m_bPrev = false;
			if (!arr[i].m_bPrev && arr[i].m_Fire1 != ResManager::GetInstance()->GetImg(ENEMY_1B))
			{
				if (m_bTurn)
				{
					arr[i].m_Fire1->Draw(hdc, arr[i].m_iMove.x - m_iMove, arr[i].m_iMove.y);
				}
				else 
				{
					arr[i].m_Fire2->Draw(hdc, arr[i].m_iMove.x - m_iMove, arr[i].m_iMove.y);
				}
				
			}
		}
		else
		{
			arr[i].m_bPrev = true;
		}
	}

	if (m_iTurn_Count > 10)
	{
		if (m_bTurn)
			m_bTurn = false;
		else
			m_bTurn = true;
		m_iTurn_Count = 0;
	}
	else
		m_iTurn_Count++;
}

void Fire::SetMove(int move)
{
	if (move < LENGTH * 66)
		m_iMove = move;
}

void Fire::Update()
{
	if (m_bDie)
		return;


	for (int i = 0; i < FIRE_COUNT; i++)
	{
		if (arr[i].m_Fire1 == ResManager::GetInstance()->GetImg(ENEMY))
			arr[i].m_iMove.x--;
		else if (arr[i].m_Fire1 == ResManager::GetInstance()->GetImg(CASH))
			arr[i].m_iMove.x--;
		else if (arr[i].m_Fire1 == ResManager::GetInstance()->GetImg(ENEMY_1F))
			arr[i].m_iMove.x--;
		if (arr[i].m_Fire1 == ResManager::GetInstance()->GetImg(ENEMY_1B))
			arr[i].m_iMove.x--;

		if (arr[i].m_Fire1 == ResManager::GetInstance()->GetImg(CASHSCORE))
			m_Cashturn++;
	}


	for (int i = 0; i < FIRE_COUNT; i++)
	{
		if (m_Cashturn > 100)
		{
			if (arr[i].m_Fire1 == ResManager::GetInstance()->GetImg(CASHSCORE))
			{
				for (int j = i; j < FIRE_COUNT - 1; j++)
				{
					arr[i] = arr[j + 1];
				}
				m_Cashturn = 0;
			}

		}
	}
}

void Fire::SetBonus()
{
	m_bCash = false;
}

bool Fire::Collider(RECT rt)
{
	RECT m_rt;
	RECT Testrt;
	for (int i = 0; i < FIRE_COUNT; i++)
	{
		if (arr[i].m_Fire1 == ResManager::GetInstance()->GetImg(FRONT))
		{
			m_rt.left = arr[i].m_iMove.x - m_iMove;
			m_rt.top = arr[i].m_iMove.y;
			m_rt.right = m_rt.left + 50;
			m_rt.bottom = m_rt.top + 50;
			if (IntersectRect(&Testrt, &rt, &m_rt) != 0)
			{
				if (arr[i].m_bPrev)
					return false;
				m_bWin = false;
				return true;
			}
		}

 		else if (arr[i].m_Fire1 == ResManager::GetInstance()->GetImg(ENEMY_1B))
		{
			m_rt.left = arr[i].m_iMove.x - m_iMove + 11;
			m_rt.top = arr[i].m_iMove.y + 116;
			m_rt.right = m_rt.left + 41 - 11;
			m_rt.bottom = m_rt.top + 130 - 116;
			if (IntersectRect(&Testrt, &rt, &m_rt) != 0)
			{
				if (arr[i].m_bPrev)
					return false;
				if (arr[i].m_iMove.x - m_iMove > 1024 - 200)
					return false;

				m_bWin = false;
				return true;
			}
		}

		else if (arr[i].m_Fire1 == ResManager::GetInstance()->GetImg(CASH))
		{
			m_rt.left = arr[i].m_iMove.x - m_iMove;
			m_rt.top = arr[i].m_iMove.y;
			m_rt.right = m_rt.left + 25;
			m_rt.bottom = m_rt.top + 26;
			if (IntersectRect(&Testrt, &rt, &m_rt) != 0)
			{
				m_bCash = true;
				m_Cashturn = 0;
				arr[i].m_Fire1 = ResManager::GetInstance()->GetImg(CASHSCORE);
				arr[i].m_Fire2 = ResManager::GetInstance()->GetImg(CASHSCORE);

				return true;
			}
		}

		else if (arr[i].m_Fire1 == ResManager::GetInstance()->GetImg(END))
		{
			m_rt.left = arr[i].m_iMove.x - m_iMove;
			m_rt.top = arr[i].m_iMove.y;
			m_rt.right = m_rt.left + 76;
			m_rt.bottom = m_rt.top + 49;

			if (IntersectRect(&Testrt, &rt, &m_rt) != 0)
			{
				m_bWin = true;
				return true;
			}
		}
	}
	return false;
}

Fire::Fire()
{
}


Fire::~Fire()
{
	for (int i = 0; i < FIRE_COUNT; i++)
	{
		SAFE_DELETE(arr[i].m_Fire1);
		SAFE_DELETE(arr[i].m_Fire2);
	}
}
