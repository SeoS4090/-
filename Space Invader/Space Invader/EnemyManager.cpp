#include "EnemyManager.h"
#include "Enemy.h"
#include "Mecro.h"
#include "ResManager.h"
#include "resource.h"
#include "BitMap.h"
#include "Bullet.h"
#include <random>
void EnemyManager::init()
{
	
	IMG_RES enemy_res = INFO_ENEMY3;

	for (int i = 0; i < 3; i++)
	{
		if(i == 1)
			enemy_res = INFO_ENEMY2;
		else if(i == 2)
			enemy_res = INFO_ENEMY1;

		for (int j = 0; j < 10; j++)
		{
			Enemy* enemy = new Enemy();
			enemy->Init(enemy_res ,j*50 + 75 ,i*40 + 50 + BACKGROUND_HEIGHT);
			m_vecEnemy.push_back(enemy);
		}
	}
	m_Ufo = new Enemy();
	m_Ufo->Init(INFO_UFO, -150, 10 + BACKGROUND_HEIGHT);
	m_Ufo->Die();



	m_CheckCount = 0;
	m_EnemyDirection = false;
	m_UfoBonus = 0;
	m_iter = m_vecEnemy.begin();
}

int EnemyManager::GetScore()
{
	m_Score = 0;

	for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
	{
		if ((*iter)->GetDie() && (*iter)->GetEnemyID() == INFO_ENEMY1)
			m_Score += 10;
		else if ((*iter)->GetDie() && (*iter)->GetEnemyID() == INFO_ENEMY2)
			m_Score += 20;
		else if ((*iter)->GetDie() && (*iter)->GetEnemyID() == INFO_ENEMY3)
			m_Score += 30;
	}
	m_Score += m_UfoBonus;
	return m_Score;
}

void EnemyManager::Draw(HDC hdc)
{
	for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
	{
		(*iter)->Draw(hdc);
	}
	for (auto iter = m_vecBullet.begin(); iter != m_vecBullet.end(); iter++)
	{
		(*iter)->Draw(hdc);
	}
	m_Ufo->Draw(hdc);
}

void EnemyManager::Update()
{
	srand(GetTickCount());
	for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
	{
		if (rand() % 5000 == 0)
		{/*적이 쏜 총알*/
			Bullet * bullet = new Bullet();
			bullet->Init((*iter)->Get_XPosition(), (*iter)->Get_YPosition(), true);
			m_vecBullet.push_back(bullet);
		}
	}
	for (auto iter = m_vecBullet.begin(); iter != m_vecBullet.end(); iter++)
	{
		(*iter)->Update();
	}

	if (m_iter == m_vecEnemy.end())
	{
		m_iter = m_vecEnemy.begin();

		if (((*m_iter)->Get_XPosition() < 50 || (*m_iter)->Get_XPosition() > 100))
		{
			m_EnemyDirection = !m_EnemyDirection;
			for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
			{
				(*iter)->Move(0, 10);
			}
		}
	}

	if (m_CheckCount > 10)
	{
		m_CheckCount = 0;
		

		for (int i = 0; i < 10; i++, m_iter++)
		{
			if (m_EnemyDirection)
				(*m_iter)->Move(16, 0);
			else
				(*m_iter)->Move(-16, 0);
		}

		
	}
	m_CheckCount++;

	srand(GetTickCount());

	if (rand() % 1024 == 0)
	{
		m_Ufo->Move(-m_Ufo->Get_XPosition(), 0);
		m_Ufo->Live();
	}

	
	m_Ufo->Move(3, 0);

	if (m_Ufo->Get_XPosition() > 700)
	{
		m_Ufo->Die();
	}

	
}

bool EnemyManager::CheckCollider(RECT rec_bullet)
{
	RECT Testrt;
	RECT rt;

	for (auto iter = m_vecBullet.begin(); iter != m_vecBullet.end(); iter++)
	{
		if (!(*iter)->GetDie() && IntersectRect(&Testrt, &rt, &rec_bullet) != 0)
		{
			(*iter)->Die();
			return true;
		}
	}



	//일반 적 충돌 검사
	for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
	{
		rt.left = (*iter)->Get_XPosition();
		rt.top = (*iter)->Get_YPosition();
		rt.right = rt.left + 32;
		rt.bottom = rt.top + 32;

		if (!(*iter)->GetDie() && IntersectRect(&Testrt, &rt, &rec_bullet) != 0)
		{
			(*iter)->Die();
			
			return true;
		}
	}

	//UFO 충돌 검사
	rt.left = m_Ufo->Get_XPosition();
	rt.top = m_Ufo->Get_YPosition();
	rt.right = rt.left + 32;
	rt.bottom = rt.top + 32;
	if (!m_Ufo->GetDie() && IntersectRect(&Testrt, &rt, &rec_bullet) != 0)
	{
		m_Ufo->Die();
		m_UfoBonus += 100;
		return true;
	}

	return false;
}

void EnemyManager::Release()
{
	for (auto iter = m_vecEnemy.begin(); iter != m_vecEnemy.end(); iter++)
	{
		SAFE_DELETE((*iter));
	}
	SAFE_DELETE(m_Ufo);
}

EnemyManager::EnemyManager()
{
	
}


EnemyManager::~EnemyManager()
{
	Release();
}
