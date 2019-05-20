#include "GameManager.h"
#include "Mecro.h"
#include "ResManager.h"
#include "BitMap.h"
#include "resource.h"
#include "BackGround.h"
#include "Barrier.h"
#include "Bullet.h"
#include "Enemy.h"
#include <random>

GameManager* GameManager::p_This = NULL;

void GameManager::Init(HDC hdc, HINSTANCE g_hInst)
{
	BackDC = CreateCompatibleDC(hdc);
	m_hBitmap = CreateCompatibleBitmap(hdc, GAME_WIDTH, GAME_HEIGHT);
	m_Oldbitmap = (HBITMAP)SelectObject(BackDC, m_hBitmap);

	ResManager::GetInstance()->Init(hdc, g_hInst);
	

	m_Player = new Enemy();
	m_Player->Init(INFO_PLAYER, (GAME_WIDTH) / 2 - BITMAP_WIDTH/2, GAME_HEIGHT - BITMAP_HEIGHT - 50);

	for (int i = 0; i < NUM_ENEMY; i++)
	{
		m_arrEnemy[i] = new Enemy();
		if (i < 10)
			m_arrEnemy[i]->Init(INFO_ENEMY3, i % 10 * 50 + 75, i / 10 * 50 + 50 + BACKGROUND_HEIGHT);
		else if (i < 20)
			m_arrEnemy[i]->Init(INFO_ENEMY2, i % 10 * 50 + 75, i / 10 * 50 + 50 + BACKGROUND_HEIGHT);
		else
			m_arrEnemy[i]->Init(INFO_ENEMY1, i % 10 * 50 + 75, i / 10 * 50 + 50 + BACKGROUND_HEIGHT);
	}
	
	m_iEnemy_Count = 0;
	m_Ufo = new Enemy();
	m_Ufo->Init(INFO_UFO, -200, BACKGROUND_HEIGHT);
	m_Ufo->SetDie();

	m_BackGround = new BackGround();
	m_BackGround->Init();

	for (int i = 0; i < NUM_BARRIER; i++)
	{
		m_Barrier[i] = new Barrier();
		m_Barrier[i]->Init(i);
	}
	m_state = START;

	m_DrawCount = 0;
	m_iBullet_Count = 50;
	m_Direction = false;
}

void GameManager::Draw(HDC hdc)
{
	
	
	if (m_state == START)
	{
		ResManager::GetInstance()->GetImg(INFO_START)->Draw(BackDC, 0, 0);

	}
	if (m_state == GAME || m_state == WAIT)
	{
		m_BackGround->Draw(BackDC);
		
		

		for(int i = 0 ; i < NUM_BARRIER ; i++)
		m_Barrier[i]->Draw(BackDC);

		for (auto iter = m_vecBullet.begin(); iter != m_vecBullet.end(); iter++)
		{
			(*iter)->Draw(BackDC);
		}

		m_Player->Draw(BackDC);
		for (int i = 0; i < m_Player->GetLife(); i++)
		{
			ResManager::GetInstance()->GetImg(INFO_PLAYER)->Draw(BackDC, BITMAP_WIDTH * i, GAME_HEIGHT - BITMAP_HEIGHT * 2);
		}


		m_Ufo->Draw(BackDC);

		for (int i = 0; i < NUM_ENEMY; i++)
		{
			m_arrEnemy[i]->Draw(BackDC);
		}
		if (m_state == WAIT)
			m_state = END;

		
	}

	if (m_state == END)
	{
		m_DrawCount++;
		if (m_DrawCount < 50)
			m_Player->Draw(BackDC);
		else if (m_Player->GetLife() >= 0)
		{
			m_Player->Live();
			m_state = GAME;
			m_DrawCount = 0;
		}
		else
		{
			m_DrawCount = 0;
			ResManager::GetInstance()->GetImg(INFO_STRINGEND)->Draw(BackDC, GAME_WIDTH / 2, GAME_HEIGHT / 2);
		}
	}
	BitBlt(hdc, 0, 0, GAME_WIDTH, GAME_HEIGHT, BackDC, 0, 0, SRCCOPY);
}

void GameManager::Update(HWND hWnd)
{


	if (m_state != GAME)
		return;
	

	if( (m_arrEnemy[m_iEnemy_Count]->Get_XPosition() < WALL || m_arrEnemy[m_iEnemy_Count]->Get_XPosition() > GAME_WIDTH - BITMAP_WIDTH - WALL) && !m_arrEnemy[m_iEnemy_Count]->GetDie())
	{
		m_Direction = !m_arrEnemy[m_iEnemy_Count]->GetDirection();

		if(m_arrEnemy[m_iEnemy_Count]->Get_XPosition() > GAME_WIDTH - BITMAP_WIDTH - WALL)
			m_arrEnemy[m_iEnemy_Count]->Update();

		for (int i = 0; i < NUM_ENEMY; i++)
		{
			m_arrEnemy[i]->ReverseDirection(m_Direction);
			if ((m_arrEnemy[m_iEnemy_Count]->Get_XPosition() < WALL || m_arrEnemy[m_iEnemy_Count]->Get_XPosition() > GAME_WIDTH - BITMAP_WIDTH - WALL))
				m_arrEnemy[i]->Update();
			m_arrEnemy[i]->Update();
		}
		
	}
	else
		m_arrEnemy[m_iEnemy_Count]->Update();
	m_iEnemy_Count++;
	if (m_iEnemy_Count >= NUM_ENEMY)
		m_iEnemy_Count = 0;


	srand(GetTickCount());
	if (rand() % 1000 == 0 && m_Ufo->GetDie())
	{
		m_Ufo->Live();
		m_Ufo->Move(-m_Ufo->Get_XPosition());
	}
	if (rand() % 100 == 0 && !m_arrEnemy[m_iEnemy_Count]->GetDie())
	{
		Bullet *bullet = new Bullet();
		bullet->Init(m_arrEnemy[m_iEnemy_Count]->Get_XPosition(), m_arrEnemy[m_iEnemy_Count]->Get_YPosition(), true);
		m_vecBullet.push_back(bullet);
	}

	if (m_Ufo->Get_XPosition() > 650)
		m_Ufo->SetDie();
	else
		m_Ufo->Move(5);

	
	for (auto iter = m_vecBullet.begin(); iter != m_vecBullet.end(); iter++)
	{
		(*iter)->Update();
		//총알 적 검사
		for (int i = 0; i < NUM_ENEMY; i++)
		{
			if ((*iter)->CheckCollider(m_arrEnemy[i]->GetRect()) && !m_arrEnemy[i]->GetDie() && !(*iter)->GetDie() && !(*iter)->GetEnemyBullet())
			{
				m_arrEnemy[i]->SetDie();

				//점수
				
				m_BackGround->SetScore(m_arrEnemy[i]->GetScore());

				(*iter)->Die();
				break;
			}
		}
		//총알 UFO 검사
		if ((*iter)->CheckCollider(m_Ufo->GetRect()) && !m_Ufo->GetDie() && !(*iter)->GetEnemyBullet())
		{
			m_Ufo->SetDie();
			m_BackGround->SetScore(rand() % 1000);
		}

		//총알 플레이어 검사
		if ((*iter)->CheckCollider(m_Player->GetRect()) && !(*iter)->GetDie() && (*iter)->GetEnemyBullet())
		{
			(*iter)->Die();
			m_Player->SetDie();
			m_state = WAIT;
		}
		

		//총알 베리어 검사

		for (int i = 0; i < NUM_BARRIER; i++)
		{
			if ((*iter)->CheckCollider(m_Barrier[i]->GetRect()))
			{
				for (auto deadbullet = m_vecBullet.begin(); deadbullet != m_vecBullet.end(); deadbullet++)
				{
					if ((*iter)->CheckCollider((*deadbullet)->GetRect()))
					{
						(*iter)->Update();
					}
				}

				if ((*iter)->GetRect().top >= GAME_HEIGHT - BITMAP_HEIGHT - 34 - 100 && (*iter)->GetRect().top <= GAME_HEIGHT - BITMAP_HEIGHT - 100)
					(*iter)->Die();
			}
		}
		
	}
	m_iBullet_Count++;	

	//게임 끝 검사
	int i;
     	for (i = 0; i < NUM_ENEMY; i++)
	{
		if (!m_arrEnemy[i]->GetDie())
			break;
	}
	if (i == NUM_ENEMY)
	{
		m_state = WAIT;
	}
}



void GameManager::Move(int key)
{
	if (key == VK_RETURN &&  m_state == END)
	{
		ReStart();
		m_state = START;
		return;
	}

	if(m_state == START && key == VK_SPACE)
	{
		m_state = GAME;
		return;
	}


	if (m_state == GAME)
	{
		m_Player->Move(key);
		if (key == VK_SPACE && m_iBullet_Count > BULLET_INTERVAL)
		{
			Bullet *bullet = new Bullet();
			bullet->Init((m_Player->GetRect().left + m_Player->GetRect().right )/ 2, m_Player->GetRect().top - 10, false);
			m_vecBullet.push_back(bullet);
			m_iBullet_Count = 0;
		}
	}
}

void GameManager::ReStart()
{
	for (auto iter = m_vecBullet.begin(); iter != m_vecBullet.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecBullet.clear();

	m_Player->Init(INFO_PLAYER, (GAME_WIDTH) / 2 - 16,GAME_HEIGHT - BITMAP_HEIGHT - 50);

	for (int i = 0; i < NUM_ENEMY; i++)
	{
		if (i < 10)
			m_arrEnemy[i]->Init(INFO_ENEMY3, i % 10 * 50 + 75, i / 10 * 50 + 50 + BACKGROUND_HEIGHT);
		else if (i < 20)
			m_arrEnemy[i]->Init(INFO_ENEMY2, i % 10 * 50 + 75, i / 10 * 50 + 50 + BACKGROUND_HEIGHT);
		else
			m_arrEnemy[i]->Init(INFO_ENEMY1, i % 10 * 50 + 75, i / 10 * 50 + 50 + BACKGROUND_HEIGHT);
	}
	m_iEnemy_Count = 0;
	m_Ufo->Init(INFO_UFO, -200, BACKGROUND_HEIGHT);
	m_Ufo->SetDie();

	m_BackGround->Init();

	for (int i = 0; i < 3; i++)
		m_Barrier[i]->Init(i);

	m_iBullet_Count = 50;
}

void GameManager::Release()
{
	SAFE_DELETE(m_BackGround);
	SAFE_DELETE(m_Player);
	

	for (auto iter = m_vecBullet.begin(); iter != m_vecBullet.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	for (int i = 0; i < NUM_ENEMY; i++)
	{
		SAFE_DELETE(m_arrEnemy[i]);
	}
	SAFE_DELETE(m_Ufo);
	for (int i = 0; i < 3; i++)
		SAFE_DELETE(m_Barrier[i]);

	ResManager::GetInstance()->Release();
	SAFE_DELETE(p_This);

}

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}
