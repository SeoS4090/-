#include "GameManager.h"
#include "ResManager.h"
#include "resource.h"
#include "Mecro.h"
#include "BitMap.h"
#include "BackGround.h"
#include "Tank.h"
#include "Bullet.h"
#include <random>
#include <fstream>
GameManager * GameManager::p_This = NULL;

bool GameManager::CreateEnemy()
{
	auto map_iter = m_vecMap.begin();
	auto enemy_iter = m_vecStructEnmey.begin();
	srand(GetTickCount());
	TankStruct enemy;
	enemy.tank = new Tank();
	RECT rtbuf;
	for(int i = 0 ; i < MAX_BULLET ; i++)
		enemy.bullet[i] = new Bullet();
	
	enemy.m_fBulletTime = 0;
	
	POINT position;
	position.x = (int)((rand() % (MAP_WIDTH- BITMAP_XSIZE - 10)) / 33 ) * 33 + 10;
	position.y = (int)((rand() % (MAP_HEIGHT - BITMAP_YSIZE - 10))/25 ) * 25 + 10;

	enemy.tank->Init(E_DOWN_00, position.x, position.y);
	
	for (; map_iter != m_vecMap.end(); map_iter++)
	{
		RECT rt;
		
		rt.left = (*map_iter)->bitmap_Point.x;
		rt.right = rt.left + 33;
		rt.top = (*map_iter)->bitmap_Point.y;
		rt.bottom = rt.top + 25;
		if (IntersectRect(&rtbuf, &enemy.tank->GetRect(), &rt) != 0 )
		{
			SAFE_DELETE(enemy.tank);
			for(int i = 0 ; i <MAX_BULLET ; i++)
				SAFE_DELETE(enemy.bullet[i]);
			
			return false;
		}
	}

	for (; enemy_iter != m_vecStructEnmey.end(); enemy_iter++)
	{
		
		if (IntersectRect(&rtbuf, &enemy.tank->GetRect(), &(*enemy_iter).tank->GetRect()))
		{
			SAFE_DELETE(enemy.tank);
			for (int i = 0; i <MAX_BULLET; i++)
				SAFE_DELETE(enemy.bullet[i]);
			return false;
		}
	}
	if (IntersectRect(&rtbuf, &enemy.tank->GetRect(), &m_structPlayer.tank->GetRect()))
	{
		SAFE_DELETE(enemy.tank);
		for (int i = 0; i <MAX_BULLET; i++)
			SAFE_DELETE(enemy.bullet[i]);
		return false;
	}
	m_vecStructEnmey.push_back(enemy);
	return true;

}

void GameManager::Init(HWND hWnd, HINSTANCE g_hInst)
{
	m_hWnd = hWnd;
	HDC hdc = GetDC(hWnd);
	ResManager::GetInstance()->Init(hdc, g_hInst);
	BackDC = CreateCompatibleDC(hdc);
	m_hBitMap = CreateCompatibleBitmap(hdc, MAP_WIDTH * 4 + 20 +100, MAP_HEIGHT * 4 + 20);
	m_hOldBit = (HBITMAP)SelectObject(BackDC, m_hBitMap);

	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();
	m_fDuringTime = 0;

	m_BackGround = new BackGround();

	m_structPlayer.tank = new Tank();
	for(int i = 0 ; i < MAX_BULLET ; i++)
		m_structPlayer.bullet[i] = new Bullet();
	
	
	m_BackGround->Init();
	
	ReleaseDC(hWnd, hdc);
}

void GameManager::LoadMap(int stage)
{
	char ch_stage[128];
	sprintf_s(ch_stage, "Stage%d", stage);


	for (auto iter = m_vecMap.begin(); iter != m_vecMap.end(); iter++)
	{
		SAFE_DELETE((*iter));
	}
	for (auto iter = m_vecGrass.begin(); iter != m_vecGrass.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecGrass.clear();
	m_vecMap.clear();

	ifstream inFile(ch_stage);
	while (!inFile.eof())
	{
		char str[100];
		inFile.getline(str, 100);

		INFO * loadinfo = new INFO();

		loadinfo->bitmap_id = (INFO_BITMAP)atoi(str);
		inFile.getline(str, 100);
		loadinfo->bitmap_Point.x = atoi(str);
		inFile.getline(str, 100);
		loadinfo->bitmap_Point.y = atoi(str);
		loadinfo->bitmap_Fade = false;

		if (loadinfo->bitmap_id == 0 && loadinfo->bitmap_Point.x == 0 && loadinfo->bitmap_Point.y == 0)
		{
			SAFE_DELETE(loadinfo);
			break;
		}
		else if (loadinfo->bitmap_id == BLOCK06)
		{
			m_vecGrass.push_back(loadinfo);
			
		}
		else if (loadinfo->bitmap_id >= 0 && loadinfo->bitmap_id <= BLOCK14)
			m_vecMap.push_back(loadinfo);
		else
		{
			m_structPlayer.tank->Init((INFO_BITMAP)loadinfo->bitmap_id, loadinfo->bitmap_Point.x, loadinfo->bitmap_Point.y);
			m_structPlayer.tank->SetBarrier(true);
			SAFE_DELETE(loadinfo);
		}
	}

	inFile.close();
}

void GameManager::Update()
{
	m_dwCurTime = GetTickCount();
	m_fElapseTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = m_dwCurTime;
	m_fDuringTime += m_fElapseTime;


	if (m_BackGround->GetState() == STATE_START)
	{
		m_iScore = 0;

		if (m_fDuringTime > 0.1)
		{
			m_fDuringTime = 0;

			if (m_BackGround->SetStage())
			{
				LoadMap(m_BackGround->GetStage());
				while (true)
				{
					if (CreateEnemy())
						break;
				}
			}
			
		}
		if (!(GetKeyState(VK_UP) & 0x8000) && !(GetKeyState(VK_DOWN) & 0x8000))
			m_fDuringTime = 1;

	}

	else if (m_BackGround->GetState() == STATE_GAME || m_BackGround->GetState() == STATE_WAIT)
	{
		m_structPlayer.tank->Update();
		m_structPlayer.tank->TankMove(m_vecMap);
		m_structPlayer.m_fBulletTime += m_fElapseTime;
		/*적 다 죽였을 경우*/
		auto enemy_iter = m_vecStructEnmey.begin();
		for (; enemy_iter != m_vecStructEnmey.end(); enemy_iter++)
		{
			if (!(*enemy_iter).tank->GetDie())
				break;
		}
		if (enemy_iter == m_vecStructEnmey.end() && m_BackGround->GetEnemy() <= 0 && m_BackGround->GetState() != STATE_WAIT)
		{
			m_fDuringTime = 0;
			m_BackGround->SetState(STATE_WAIT);
		}

		//플레이어 총알 생산
		if (GetKeyState(VK_SPACE) & 0x8000 && !m_structPlayer.tank->GetRespone() && m_BackGround->GetState() != STATE_WAIT)
		{
			if (m_structPlayer.m_fBulletTime > 0.2)
			{
				for (int i = 0; i < MAX_BULLET; i++)
				{
					if (m_structPlayer.bullet[i]->GetFade())
					{
						m_structPlayer.bullet[i]->Init((m_structPlayer.tank->GetRect().left + m_structPlayer.tank->GetRect().right) / 2, (m_structPlayer.tank->GetRect().top + m_structPlayer.tank->GetRect().bottom) / 2, m_structPlayer.tank->GetDirect());
						break;
					}
				}
				m_structPlayer.m_fBulletTime = 0;
			}
		}

		/*적 생성*/
		srand(GetTickCount());
		if (rand() % 100 == 0 && m_BackGround->GetEnemy() > 0 && m_BackGround->GetState() != STATE_WAIT)
		{
			if (CreateEnemy())
				m_BackGround->DecreaseEnemy();
		}

		//적 업데이트
		for (auto iter = m_vecStructEnmey.begin(); iter != m_vecStructEnmey.end(); iter++)
		{
			RECT rtbuf;
			(*iter).tank->Update();
			(*iter).m_fBulletTime += m_fElapseTime;
			
			if (IntersectRect(&rtbuf, &(*iter).tank->GetRect(), &m_structPlayer.tank->GetRect()) == 0 && m_BackGround->GetState() != STATE_WAIT)
			{
				(*iter).tank->AIUpdate(m_vecMap);
			}
			else if (!(*iter).tank->GetDie())
			{
				(*iter).tank->BackPosition();
				(*iter).tank->ChangeDirect();
				m_structPlayer.tank->BackPosition();
			}

			for (auto iter2 = m_vecStructEnmey.begin(); iter2 != m_vecStructEnmey.end(); iter2++)
			{
				if ((*iter).tank != (*iter2).tank && IntersectRect(&rtbuf, &(*iter).tank->GetRect(), &(*iter2).tank->GetRect()) != 0 && !(*iter2).tank->GetDie())
				{
					(*iter).tank->BackPosition();
					(*iter).tank->ChangeDirect();
					(*iter2).tank->BackPosition();
					(*iter2).tank->ChangeDirect();
				}
			}
		}
		/*적 총알 생성*/
		if(m_BackGround->GetState() != STATE_WAIT)
		for (auto enemy_iter = m_vecStructEnmey.begin(); enemy_iter != m_vecStructEnmey.end(); enemy_iter++)
		{
			if (!(*enemy_iter).tank->GetDie()&&!(*enemy_iter).tank->GetRespone() && (*enemy_iter).m_fBulletTime > 0.5 && rand() % 100 == 0)
			{
				for (int i = 0 ; i < MAX_BULLET; i++)
				{
					if ((*enemy_iter).bullet[i]->GetFade() )
					{
						(*enemy_iter).bullet[i]->Init(((*enemy_iter).tank->GetRect().left + (*enemy_iter).tank->GetRect().right) / 2, ((*enemy_iter).tank->GetRect().top + (*enemy_iter).tank->GetRect().bottom) / 2, (*enemy_iter).tank->GetDirect());
						break;
					}
				}
				(*enemy_iter).m_fBulletTime = 0;
			}
		}


		//플레이어 총알 검사
		for (int i = 0; i < MAX_BULLET; i++)
		{
			m_structPlayer.bullet[i]->Update();

			//적 VS 총알
			for (auto enemy_iter = m_vecStructEnmey.begin(); enemy_iter != m_vecStructEnmey.end(); enemy_iter++)
			{
				if (!(*enemy_iter).tank->GetDie() && !m_structPlayer.bullet[i]->GetFade() && m_structPlayer.bullet[i]->CheckCollider((*enemy_iter).tank->GetRect()))
				{
					(*enemy_iter).tank->SetDie();
					m_iScore += 100;
				}
			}

			//지도 총알
			for (auto map_iter = m_vecMap.begin(); map_iter != m_vecMap.end(); map_iter++)
			{
				RECT rt;
				rt.left = (*map_iter)->bitmap_Point.x;
				rt.top = (*map_iter)->bitmap_Point.y;
				rt.right = rt.left + BITMAP_XSIZE;
				rt.bottom = rt.top + BITMAP_YSIZE;
				if ((*map_iter)->bitmap_id == BLOCK01 || (*map_iter)->bitmap_id == BLOCK09)
					rt.bottom = rt.bottom - (BITMAP_YSIZE / 2);
				if ((*map_iter)->bitmap_id == BLOCK02 || (*map_iter)->bitmap_id == BLOCK10)
					rt.right = rt.right - (BITMAP_XSIZE / 2);
				if ((*map_iter)->bitmap_id == BLOCK03 || (*map_iter)->bitmap_id == BLOCK11)
					rt.top = rt.top + (BITMAP_YSIZE / 2);
				if ((*map_iter)->bitmap_id == BLOCK04 || (*map_iter)->bitmap_id == BLOCK12)
					rt.left = rt.left + (BITMAP_XSIZE / 2);

				if ((*map_iter)->bitmap_id != BLOCK07 && !(*map_iter)->bitmap_Fade && !m_structPlayer.bullet[i]->GetFade() && (m_structPlayer.bullet[i])->CheckCollider(rt))
				{
					if ((*map_iter)->bitmap_id == BLOCK00)
					{
						if (m_structPlayer.bullet[i]->GetDirect() == MISSILE_00)
							(*map_iter)->bitmap_id = BLOCK01;
						else if (m_structPlayer.bullet[i]->GetDirect() == MISSILE_01)
							(*map_iter)->bitmap_id = BLOCK02;
						else if (m_structPlayer.bullet[i]->GetDirect() == MISSILE_02)
							(*map_iter)->bitmap_id = BLOCK03;
						else if (m_structPlayer.bullet[i]->GetDirect() == MISSILE_03)
							(*map_iter)->bitmap_id = BLOCK04;
					}

					else if ((*map_iter)->bitmap_id >= BLOCK01 && (*map_iter)->bitmap_id <= BLOCK04)
						(*map_iter)->bitmap_Fade = true;
					else if ((*map_iter)->bitmap_id == BLOCK13)
					{
						(*map_iter)->bitmap_id = BLOCK14;
						m_fDuringTime = 0;
						m_BackGround->SetState(STATE_WAIT);
					}
				}//충돌
			

			} //지도 총알 끝
			
		}//플레이어 총알 검사 끝
		
		 //적 총알 검사
		for (auto iter = m_vecStructEnmey.begin(); iter != m_vecStructEnmey.end(); iter++)
		{
			for (int i = 0; i < MAX_BULLET; i++)
			{
			//총알 업데이트
					(*iter).bullet[i]->Update();
					//지도 총알
					for (auto map_iter = m_vecMap.begin(); map_iter != m_vecMap.end(); map_iter++)
					{
						RECT rt;
						rt.left = (*map_iter)->bitmap_Point.x;
						rt.top = (*map_iter)->bitmap_Point.y;
						rt.right = rt.left + BITMAP_XSIZE;
						rt.bottom = rt.top + BITMAP_YSIZE;
						if ((*map_iter)->bitmap_id == BLOCK01 || (*map_iter)->bitmap_id == BLOCK09)
							rt.bottom = rt.bottom - (BITMAP_YSIZE / 2);
						if ((*map_iter)->bitmap_id == BLOCK02 || (*map_iter)->bitmap_id == BLOCK10)
							rt.right = rt.right - (BITMAP_XSIZE / 2);
						if ((*map_iter)->bitmap_id == BLOCK03 || (*map_iter)->bitmap_id == BLOCK11)
							rt.top = rt.top + (BITMAP_YSIZE / 2);
						if ((*map_iter)->bitmap_id == BLOCK04 || (*map_iter)->bitmap_id == BLOCK12)
							rt.left = rt.left + (BITMAP_XSIZE / 2);

						if ((*map_iter)->bitmap_id != BLOCK07 && !(*map_iter)->bitmap_Fade && !(*iter).bullet[i]->GetFade() && ((*iter).bullet[i])->CheckCollider(rt))
						{
							if ((*map_iter)->bitmap_id == BLOCK00)
							{
								if ((*iter).bullet[i]->GetDirect() == MISSILE_00)
									(*map_iter)->bitmap_id = BLOCK01;
								else if ((*iter).bullet[i]->GetDirect() == MISSILE_01)
									(*map_iter)->bitmap_id = BLOCK02;
								else if ((*iter).bullet[i]->GetDirect() == MISSILE_02)
									(*map_iter)->bitmap_id = BLOCK03;
								else if ((*iter).bullet[i]->GetDirect() == MISSILE_03)
									(*map_iter)->bitmap_id = BLOCK04;
							}

							else if ((*map_iter)->bitmap_id >= BLOCK01 && (*map_iter)->bitmap_id <= BLOCK04)
								(*map_iter)->bitmap_Fade = true;
							else if ((*map_iter)->bitmap_id == BLOCK13)
							{
								(*map_iter)->bitmap_id = BLOCK14;
								m_fDuringTime = 0;
								m_BackGround->SetState(STATE_WAIT);
							}
						}//충돌
					
					} //지도 총알 끝
					//플레이어 적총알 충돌
					if (!m_structPlayer.tank->GetDie() && !(*iter).bullet[i]->GetFade() && (*iter).bullet[i]->CheckCollider(m_structPlayer.tank->GetRect()))
					{
						if (m_structPlayer.tank->GetBarrier())
							m_structPlayer.tank->SetBarrier(false);
						else
							m_structPlayer.tank->SetDie();
						

					}

			}
		}

		if (m_structPlayer.tank->GetDie())
		{
			auto iter = m_vecStructEnmey.begin();

			for (; iter != m_vecStructEnmey.end(); iter++)
			{
				RECT rtbuf;
				if (IntersectRect(&rtbuf, &(*iter).tank->GetRect(), &m_structPlayer.tank->GetRect()))
					break;
			}
			if (iter == m_vecStructEnmey.end() && m_structPlayer.tank->GetLife() >= 0)
			{
				if(m_structPlayer.tank->Resurrection())
				m_BackGround->PlayerDie();
			}
			else if (iter == m_vecStructEnmey.end() && m_BackGround->GetState() != STATE_WAIT)
			{
				m_fDuringTime = 0;
				m_BackGround->SetState(STATE_WAIT);
			}
		}

		if (m_BackGround->GetState() == STATE_WAIT && m_fDuringTime > 3)
			m_BackGround->SetState(STATE_END);

	}
	//STATE_GAME 끝

	

	if (m_BackGround->GetState() == STATE_END)
	{
		if (m_fDuringTime > 0.1)
		{
			if (m_BackGround->SetStage())
			{
				m_BackGround->Init();

			}
			m_fDuringTime = 0;
		}
		if (!(GetKeyState(VK_UP) & 0x8000) && !(GetKeyState(VK_DOWN) & 0x8000))
			m_fDuringTime = 1;

		for (auto iter = m_vecStructEnmey.begin(); iter != m_vecStructEnmey.end(); iter++)
		{
			SAFE_DELETE((*iter).tank);
			for (int i = 0; i < MAX_BULLET; i++)
			{
				SAFE_DELETE((*iter).bullet[i]);
				(*iter).bullet[i] = new Bullet();
			}
			(*iter).tank = new Tank();
		}
	}
	Draw();
}

void GameManager::Draw()
{
	HDC hdc = GetDC(m_hWnd);

	//배경
	m_BackGround->Draw(BackDC);

	

	if (m_BackGround->GetState() == STATE_GAME || m_BackGround->GetState() == STATE_WAIT)
	{
		for (auto iter = m_vecMap.begin(); iter != m_vecMap.end(); iter++)
		{
			if (!(*iter)->bitmap_Fade)
				ResManager::GetInstance()->GetImg((*iter)->bitmap_id)->Draw(BackDC, (*iter)->bitmap_Point.x, (*iter)->bitmap_Point.y);
		}

		//플레이어
		m_structPlayer.tank->Draw(BackDC);

		for(int i = 0 ; i <MAX_BULLET ; i++) 
			m_structPlayer.bullet[i]->Draw(BackDC);


		//적
		
		for (auto iter = m_vecStructEnmey.begin(); iter != m_vecStructEnmey.end(); iter++)
		{
				(*iter).tank->Draw(BackDC);
			for(int i = 0 ; i < MAX_BULLET ; i++)
				(*iter).bullet[i]->Draw(BackDC);
		}

		//풀
		for (auto iter = m_vecGrass.begin(); iter != m_vecGrass.end(); iter++)
		{
			ResManager::GetInstance()->GetImg((*iter)->bitmap_id)->Draw(BackDC, (*iter)->bitmap_Point.x, (*iter)->bitmap_Point.y);
		}
	}
	else if (m_BackGround->GetState() == STATE_END && m_fDuringTime > 0.5)
	{
		ResManager::GetInstance()->GetImg(E_RIGHT_00)->Draw(BackDC, (MAP_WIDTH + 10) / 2 - 100, (MAP_HEIGHT + 10) / 2);
		
		m_BackGround->SetScore(m_iScore);	
	}

	if (m_BackGround->GetState() == STATE_WAIT)
	{
		if(m_fDuringTime * 100 < (MAP_HEIGHT / 2) + 50)
			ResManager::GetInstance()->GetImg(GAME_OVER)->Draw(BackDC, MAP_WIDTH / 2 - 100, MAP_HEIGHT - m_fDuringTime * 100);
	}

	TransparentBlt(hdc, 0, 0, MAP_WIDTH * 2 + 20 + 100, MAP_HEIGHT * 2 + 20, BackDC, 0, 0, MAP_WIDTH +10 + 100, MAP_HEIGHT + 5, SRCCOPY);
	ReleaseDC(m_hWnd, hdc);
}

void GameManager::Release()
{

	for (auto iter = m_vecMap.begin(); iter != m_vecMap.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	for (auto iter = m_vecGrass.begin(); iter != m_vecGrass.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	for (auto iter = m_vecStructEnmey.begin(); iter != m_vecStructEnmey.end(); iter++)
	{
		SAFE_DELETE((*iter).tank);
		for (int i = 0; i < MAX_BULLET; i++)
		{
			SAFE_DELETE((*iter).bullet[i]);
		}
	}

	SAFE_DELETE(m_structPlayer.tank);
	for (int i = 0; i < MAX_BULLET; i++)
		SAFE_DELETE(m_structPlayer.bullet[i]);
	
	SAFE_DELETE(m_BackGround);
	ResManager::GetInstance()->Release();
	SAFE_DELETE(p_This);
}

GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}
