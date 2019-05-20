#include "BackGround.h"
#include "ResManager.h"
#include "BitMap.h"
#include <string>
using namespace std;

void BackGround::Init()
{
	m_iEnemy = 10;
	m_iStage = 1;
	m_iLife = PLAYER_LIFE;
	m_State = STATE_START;
	m_iScore = 0;
}
void SetString(HDC hdc, int num, int x, int y, bool score = false)
{
	string m_sbonus;


	if (!score)
		m_sbonus = to_string(num);
	else
	{
		int i = 3 - to_string(num).size();
		m_sbonus = "";
		for (int j = 0; j < i; j++)
			m_sbonus.append("0");
		m_sbonus.append(to_string(num));
	}
	char m_arrbonus[5];
	strcpy_s(m_arrbonus, m_sbonus.c_str());

	for (int i = 0; i < 8; i++)
	{
		switch (m_arrbonus[i])
		{
		case '1':
			ResManager::GetInstance()->GetImg(INT_1)->Draw(hdc, 13 * i + x, y);
			break;
		case '2':
			ResManager::GetInstance()->GetImg(INT_2)->Draw(hdc, 13 * i + x, y);
			break;
		case '3':
			ResManager::GetInstance()->GetImg(INT_3)->Draw(hdc, 13 * i + x, y);
			break;
		case '4':
			ResManager::GetInstance()->GetImg(INT_4)->Draw(hdc, 13 * i + x, y);
			break;
		case '5':
			ResManager::GetInstance()->GetImg(INT_5)->Draw(hdc, 13 * i + x, y);
			break;
		case '6':
			ResManager::GetInstance()->GetImg(INT_6)->Draw(hdc, 13 * i + x, y);
			break;
		case '7':
			ResManager::GetInstance()->GetImg(INT_7)->Draw(hdc, 13 * i + x, y);
			break;
		case '8':
			ResManager::GetInstance()->GetImg(INT_8)->Draw(hdc, 13 * i + x, y);
			break;
		case '9':
			ResManager::GetInstance()->GetImg(INT_9)->Draw(hdc, 13 * i + x, y);
			break;
		case '0':
			ResManager::GetInstance()->GetImg(INT_0)->Draw(hdc, 13 * i + x, y);
			break;
		}
	}
}
void BackGround::Draw(HDC hdc)
{
	ResManager::GetInstance()->GetImg(BACKGROUND)->Draw(hdc, 0, 0);
	
	if (m_State == STATE_START)
	{
		for (int i = 0; i < MAP_WIDTH / 33; i++)
		{
			for (int j = 0; j < MAP_HEIGHT / 25; j++)
			{
				ResManager::GetInstance()->GetImg(BLOCK00)->Draw(hdc, i * 33+10, j * 25+10);
			}
		}
		SetString(hdc, m_iStage, (MAP_WIDTH + 10) / 2, (MAP_HEIGHT + 10) / 2);
		
	}
	else if (m_State == STATE_GAME)
	{
		for (int i = 0; i < m_iEnemy; i++)
		{
			ResManager::GetInstance()->GetImg(ENEMY_ICON)->Draw(hdc, 20 * (i % 2) + MAP_WIDTH + 50, 20 * (i / 2) + 50);
		}

		ResManager::GetInstance()->GetImg(STAGE_ICON)->Draw(hdc, MAP_WIDTH + 30 - 10, MAP_HEIGHT + 20 - 120);
		SetString(hdc, m_iStage, MAP_WIDTH + 30+20, MAP_HEIGHT + 20 - 100, true);
		ResManager::GetInstance()->GetImg(PLAYER_ICON)->Draw(hdc, MAP_WIDTH + 30 + 50 - 20, MAP_HEIGHT + 20 - 170);
		SetString(hdc, m_iLife, MAP_WIDTH + 30 + 50, MAP_HEIGHT + 20 - 170);

	}
	
	else if (m_State == STATE_END)
	{
		for (int i = 0; i < MAP_WIDTH / 33; i++)
		{
			for (int j = 0; j < MAP_HEIGHT / 25; j++)
			{
				ResManager::GetInstance()->GetImg(BLOCK00)->Draw(hdc, i * 33 + 10, j * 25 + 10);
			}
		}
		SetString(hdc, m_iScore, (MAP_WIDTH + 10) / 2, (MAP_HEIGHT + 10) /2 );
	}
}

bool BackGround::SetStage()
{
	if (m_State == STATE_START)
	{
		m_iScore = 0;

		if (GetKeyState(VK_UP) & 0x8000)
		{
 			if (m_iStage < MAX_STAGE)
				m_iStage++;
			return false;
		}
		if (GetKeyState(VK_DOWN) & 0x8000)
		{
			if (m_iStage > 1)
				m_iStage--;
			return false;
		}
		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			m_State = STATE_GAME;
			return true;
		}
	}
	
	if (m_State == STATE_END && GetKeyState(VK_RETURN) & 0x8000)
	{
		m_State = STATE_START;
		return true;
	}

	return false;

}

void BackGround::GameEnd()
{
	m_State = STATE_END;
}
void BackGround::SetState(GAME_STATE gamestate)
{
	m_State = gamestate;
}

BackGround::BackGround()
{
}


BackGround::~BackGround()
{
}
