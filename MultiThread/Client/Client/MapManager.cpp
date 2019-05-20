#include "MapManager.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "UIManager.h"
#include <fstream>
#include <random>

void MapManager::Init(PACKET_GAMESTART packet)
{
	HEngine::ResourceManager::GetInstance()->SetAnimation("./RES/BlueBub.bmp", 4, FALSE);
	srand(packet.Rand_Seed);
	int Playerpt = rand() % 8;

	//맵 읽기
	FILE *p = fopen("../../Common/map.txt", "r");
	for (int i = 0; i < MAPSIZE_Y; i++)
	{
		for (int j = 0; j < MAPSIZE_X; j++)
		{
			int value;
			fscanf(p, "%d,", &value);
			m_iBubbleMap_Origin[j][i] = value;

			if (value == 1)
			{
				srand(packet.Rand_Seed + i + j);
				int ran = rand() % 2;

				if (ran == 0)
				{
					srand(GetTickCount() + i * j);
					int rand_1 = rand() % 3;
					m_iBubbleMap_Origin[j][i] = rand_1 + 4;

				}
			}
		}
		fscanf(p, "\n", NULL);
	}
	fclose(p);

	for (int i = 0; i < MAPSIZE_Y; i++)
	{
		for (int j = 0; j < MAPSIZE_X; j++)
		{
			m_iBubbleMap[j][i] = m_iBubbleMap_Origin[j][i];
			if (m_iBubbleMap_Origin[j][i] == PLAYER)
			{
				m_iBubbleMap[j][i] = EMPTY;
				Init_pt[Playerpt].x = j;
				Init_pt[Playerpt].y = i;
				Playerpt++;
				if (Playerpt == 8)
					Playerpt = 0;
			}
			else if (m_iBubbleMap_Origin[j][i] >= SPEED && m_iBubbleMap_Origin[j][i] <= BUBBLE_COUNT)
			{
				m_iBubbleMap[j][i] = BLOCK;
			}
		}
	}


	m_Charactor[UP] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Up.bmp");
	m_Charactor[DOWN] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Down.bmp");
	m_Charactor[RIGHT] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Right.bmp");
	m_Charactor[LEFT] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Left.bmp");
	m_Charactor[DIEBUBBLE] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Characbub1.bmp");
	m_Charactor[DIE] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Die.bmp");

	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/item_Speed.bmp")->SetAniMation(4, FALSE);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/item_Power.bmp")->SetAniMation(4, FALSE);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/item_Bubble.bmp")->SetAniMation(4, FALSE);

	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Up.bmp")->SetAniMation(8, FALSE);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Down.bmp")->SetAniMation(8, FALSE);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Right.bmp")->SetAniMation(8, FALSE);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Left.bmp")->SetAniMation(8, FALSE);

	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Characbub1.bmp")->SetAniMation(4, FALSE);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Die.bmp")->SetAniMation(8, TRUE);

	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BoomUpEx.bmp")->SetAniMation(8, TRUE);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BoomDownEx.bmp")->SetAniMation(8, TRUE);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BoomRightEx.bmp")->SetAniMation(8, TRUE);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BoomLeftEx.bmp")->SetAniMation(8, TRUE);

	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BoomUpEx.bmp")->SetSpeed(BUBBLE_BOOM_DURINGTIME / 8);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BoomDownEx.bmp")->SetSpeed(BUBBLE_BOOM_DURINGTIME / 8);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BoomRightEx.bmp")->SetSpeed(BUBBLE_BOOM_DURINGTIME / 8);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BoomLeftEx.bmp")->SetSpeed(BUBBLE_BOOM_DURINGTIME / 8);
}

bool MapManager::AddType(HEngine::POINT pt, Type Object)
{
	if (pt.x < 0 || pt.x >= MAPSIZE_X || pt.y < 0 || pt.y >= MAPSIZE_Y)
		return false;

	m_iBubbleMap[pt.x][pt.y] = Object;

	return true;
}
void MapManager::BoomBlock(HEngine::POINT pt)
{	
	if (pt.x < 0 || pt.x >= MAPSIZE_X || pt.y < 0 || pt.y >= MAPSIZE_Y)
		return ;

	if (m_iBubbleMap_Origin[pt.x][pt.y] != BLOCK && m_iBubbleMap_Origin[pt.x][pt.y] != PLAYER)
	{
		m_iBubbleMap[pt.x][pt.y] = m_iBubbleMap_Origin[pt.x][pt.y];
		
	}
	else
		m_iBubbleMap[pt.x][pt.y] = EMPTY;


	m_iBubbleMap_Origin[pt.x][pt.y] = EMPTY;

}
Type MapManager::GetType(HEngine::POINT pt)
{
	return (Type)m_iBubbleMap[pt.x][pt.y];
}

void MapManager::MovePlayer(HEngine::POINT Before, HEngine::POINT After, string id, int Speed)
{
	if(m_iBubbleMap[Before.x][Before.y] != BUBBLE)
		m_iBubbleMap[Before.x][Before.y] = EMPTY;
	m_iBubbleMap[After.x][After.y] = PLAYER;

	for (auto iter = m_vecPlayerPt.begin(); iter != m_vecPlayerPt.end(); iter++)
	{
		if ((*iter)->Id == id)
		{
			(*iter)->queue_pt.push(After);
			(*iter)->m_iSpeed = Speed;
		}
	}
}

void MapManager::MovePlayer(HEngine::POINT Before, HEngine::POINT After)
{
	if (m_iBubbleMap[Before.x][Before.y] != BUBBLE)
		m_iBubbleMap[Before.x][Before.y] = EMPTY;
	m_iBubbleMap[After.x][After.y] = PLAYER;
}

bool MapManager::Update()
{
	m_fTime += HEngine::TimeManager::GetInstance()->GetElipseTime();
	

	int Count = 0;
	int i = 0;


	// 플레이어 방향 전환 및 출력 업데이트
	for (auto iter = m_vecPlayerPt.begin(); iter != m_vecPlayerPt.end(); iter++)
	{
		if ((*iter)->m_bDie)
			Count++;
		else
			HEngine::UIManager::GetInstance()->AddLabel((*iter)->Id, (*iter)->DrawPt.x, (*iter)->DrawPt.y - 15, NULL, FALSE);
				
		i++;
		HEngine::UIManager::GetInstance()->AddLabel((*iter)->Id, 705, 40 * i + 120, NULL, false);

		if ((*iter)->State >= DIEBUBBLE)
		{
			(*iter)->m_fEtime += HEngine::TimeManager::GetInstance()->GetElipseTime();
			if ((*iter)->m_fEtime >= 1.0f)
			{
				(*iter)->State = DIE;
				if ((*iter)->m_fEtime >= 2.0f)
					(*iter)->m_bDie = true;
				continue;
			}
		}
		

		if ((*iter)->pt.x * 40 + 20 != (*iter)->DrawPt.x || (*iter)->pt.y * 40 != (*iter)->DrawPt.y)
		{
			if ((*iter)->DrawPt.x > (*iter)->pt.x * 40 + 20)
			{
				SetPlayerState((*iter)->Id, LEFT);
				(*iter)->DrawPt.x -= HEngine::TimeManager::GetInstance()->GetElipseTime() * (*iter)->m_iSpeed;
				if ((*iter)->DrawPt.x <= (*iter)->pt.x * 40 + 20)
				{
					(*iter)->DrawPt.x = (*iter)->pt.x * 40 + 20;
				}
				return true;
			}

			if ((*iter)->DrawPt.x < (*iter)->pt.x * 40 + 20)
			{
				SetPlayerState((*iter)->Id, RIGHT);
				(*iter)->DrawPt.x += HEngine::TimeManager::GetInstance()->GetElipseTime() * (*iter)->m_iSpeed;
				if ((*iter)->DrawPt.x >= (*iter)->pt.x * 40 + 20)
				{
					(*iter)->DrawPt.x = (*iter)->pt.x * 40 + 20;
				}
				return true;
			}

			if ((*iter)->DrawPt.y >(*iter)->pt.y * 40)
			{
				SetPlayerState((*iter)->Id, UP);
				(*iter)->DrawPt.y -= HEngine::TimeManager::GetInstance()->GetElipseTime() * (*iter)->m_iSpeed;
				if ((*iter)->DrawPt.y <= (*iter)->pt.y * 40)
				{
					(*iter)->DrawPt.y = (*iter)->pt.y * 40;
				}	
				return true;
			}
			if ((*iter)->DrawPt.y < (*iter)->pt.y * 40)
			{
				SetPlayerState((*iter)->Id, DOWN);
				(*iter)->DrawPt.y += HEngine::TimeManager::GetInstance()->GetElipseTime() * (*iter)->m_iSpeed;
				if ((*iter)->DrawPt.y >= (*iter)->pt.y * 40)
				{
					(*iter)->DrawPt.y = (*iter)->pt.y * 40;
				}
				return true;
			}
		}
		else if ((*iter)->pt.x * 40 + 20 == (*iter)->DrawPt.x && (*iter)->pt.y * 40 == (*iter)->DrawPt.y) {
			if ((*iter)->queue_pt.size() > 0)
			{
				HEngine::POINT pt = (*iter)->queue_pt.front();
				(*iter)->queue_pt.pop();
				(*iter)->pt = pt;
			}
		}
	} 

	if (Count != m_vecPlayerPt.size())
		return true;
	else
		return false;
}
bool MapManager::CheckWin()
{
	int Count = 0;
	for (auto iter = m_vecPlayerPt.begin(); iter != m_vecPlayerPt.end(); iter++)
	{
		if ((*iter)->State == DIE)
			Count++;
	}
	if (Count == m_vecPlayerPt.size())
		return true;
	else
		return false;
}
void MapManager::ClearBoom(HEngine::POINT pt, int Power)
{	
	if (GetType(pt) == EMPTY)
		return;
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BoomUpEx.bmp")->SetAniMation(8, TRUE);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BoomDownEx.bmp")->SetAniMation(8, TRUE);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BoomRightEx.bmp")->SetAniMation(8, TRUE);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BoomLeftEx.bmp")->SetAniMation(8, TRUE);

	m_iBubbleMap[pt.x][pt.y] = EMPTY;

	for (int i = 1; i < Power + 1; i++)
	{
		if (m_iBubbleMap[pt.x + i][pt.y] == BOOM_RIGHT && pt.x + i < MAPSIZE_X)
		{
			m_iBubbleMap[pt.x + i][pt.y] = EMPTY;
			BoomBlock({ pt.x + i, pt.y });
		}
	}

	for (int i = 1; i < Power + 1; i++)
	{
		if (m_iBubbleMap[pt.x - i][pt.y] == BOOM_LEFT && pt.x - i >= 0)
		{
			m_iBubbleMap[pt.x - i][pt.y] = EMPTY;
			BoomBlock({ pt.x - i, pt.y });

		}
	}


	for (int i = 1; i < Power + 1; i++)
	{
		if (m_iBubbleMap[pt.x][pt.y + i] == BOOM_DOWN && pt.y + i < MAPSIZE_Y)
		{
			m_iBubbleMap[pt.x][pt.y + i] = EMPTY;
			BoomBlock({ pt.x, pt.y + i });

		}
	}
	for (int i = 1; i < Power + 1; i++)
	{
		if (m_iBubbleMap[pt.x][pt.y - i] == BOOM_UP && pt.y - i >= 0)
		{
			m_iBubbleMap[pt.x][pt.y - i] = EMPTY;
			BoomBlock({ pt.x, pt.y - i });
		}
	}
}
void MapManager::BoomBubble(HEngine::POINT pt, int Power)
{
	if (GetType(pt) == BOOM_CENTER)
		return;

	m_iBubbleMap[pt.x][pt.y] = BOOM_CENTER;

	for (int i = 1; i < Power + 1; i++)
	{
		if (pt.x + i < MAPSIZE_X)
			m_iBubbleMap[pt.x + i][pt.y] = BOOM_RIGHT;

		if (m_iBubbleMap_Origin[pt.x + i][pt.y] >= BLOCK && m_iBubbleMap_Origin[pt.x + i][pt.y] <= BUBBLE_COUNT)
			break;
	}

	for (int i = 1; i < Power + 1; i++)
	{
		if (pt.x - i >= 0)
			m_iBubbleMap[pt.x - i][pt.y] = BOOM_LEFT;

		if (m_iBubbleMap_Origin[pt.x - i][pt.y] >= BLOCK && m_iBubbleMap_Origin[pt.x - i][pt.y] <= BUBBLE_COUNT)
			break;
	}

	for (int i = 1; i < Power + 1; i++)
	{
		if (pt.y - i >=0)
			m_iBubbleMap[pt.x ][pt.y - i] = BOOM_UP;

		if (m_iBubbleMap_Origin[pt.x][pt.y-i] >= BLOCK && m_iBubbleMap_Origin[pt.x][pt.y-i] <= BUBBLE_COUNT)
			break;
	}

	for (int i = 1; i < Power + 1; i++)
	{
		if (pt.y + i < MAPSIZE_Y)
			m_iBubbleMap[pt.x][pt.y + i] = BOOM_DOWN;

		if (m_iBubbleMap_Origin[pt.x][pt.y + i] >= BLOCK && m_iBubbleMap_Origin[pt.x][pt.y + i] <= BUBBLE_COUNT)
			break;
	}
}

void MapManager::AddPlayer(HEngine::POINT pt, string id)
{
	m_iBubbleMap[pt.x][pt.y] = PLAYER;
	PLAYERINFO * pNew = new PLAYERINFO();
	pNew->pt.x = pt.x;
	pNew->pt.y = pt.y;
	pNew->DrawPt.x = pt.x * 40  + 20;
	pNew->DrawPt.y = pt.y * 40;
	pNew->State = DOWN;
	pNew->Id = id;
	pNew->m_bDie = false;
	pNew->m_fEtime = 0.0f;
	pNew->m_iSpeed = 300;
	m_vecPlayerPt.push_back(pNew);
}

void MapManager::SetPlayerState(string id, DIRECT state)
{
	for (auto iter = m_vecPlayerPt.begin(); iter != m_vecPlayerPt.end(); iter++)
	{
		if ((*iter)->Id == id && (*iter)->State <= DIEBUBBLE)
			(*iter)->State = state;
	}
}

void MapManager::Draw()
{
	for (int i = 0; i < MAPSIZE_X; i++)
	{
		for (int j = 0; j < MAPSIZE_Y; j++)
		{
			if (m_iBubbleMap[i][j] == BLOCK)
				HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Wall00.bmp")->Draw(i * 40 + 20, j * 40 + 40);
			if (m_iBubbleMap[i][j] == BOOM_CENTER)
				HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Boomcenter.bmp")->Draw(i * 40 + 20, j * 40 + 40);
			if (m_iBubbleMap[i][j] == BOOM_UP)
				HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BoomUpEx.bmp")->Draw(i * 40 + 20, j * 40 + 40);
			if (m_iBubbleMap[i][j] == BOOM_DOWN)
				HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BoomDownEx.bmp")->Draw(i * 40 + 20, j * 40 + 40);
			if (m_iBubbleMap[i][j] == BOOM_RIGHT)
				HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BoomRightEx.bmp")->Draw(i * 40 + 20, j * 40 + 40);
			if (m_iBubbleMap[i][j] == BOOM_LEFT)
				HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BoomLeftEx.bmp")->Draw(i * 40 + 20, j * 40 + 40);
			if (m_iBubbleMap[i][j] == SPEED)
				HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/item_Speed.bmp")->Draw(i * 40 + 20, j * 40 + 40);
			if (m_iBubbleMap[i][j] == POWER)
				HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/item_Power.bmp")->Draw(i * 40 + 20, j * 40 + 40); 
			if (m_iBubbleMap[i][j] == BUBBLE_COUNT)
				HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/item_Bubble.bmp")->Draw(i * 40 + 20, j * 40 + 40);
		}
	}

	for (auto iter = m_vecPlayerPt.begin(); iter != m_vecPlayerPt.end(); iter++)
	{
		if(!(*iter)->m_bDie)
			m_Charactor[(*iter)->State]->Draw((*iter)->DrawPt.x, (*iter)->DrawPt.y);
	}

	for (int i = 0; i < m_vecPlayerPt.size() + 1; i++)
	{
		HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Character.bmp")->Draw(655, 40 * i + 96);
	}
	
}

void MapManager::Release()
{
	for (auto iter = m_vecPlayerPt.begin(); iter != m_vecPlayerPt.end(); iter++)
	{
		while (!(*iter)->queue_pt.empty()) (*iter)->queue_pt.pop();
		SAFE_DELETE(*iter);

	}
	m_vecPlayerPt.clear();
}

POINT MapManager::GetInitPt(int i)
{
	return Init_pt[i];
}

MapManager::MapManager()
{
}

MapManager::~MapManager()
{
}
