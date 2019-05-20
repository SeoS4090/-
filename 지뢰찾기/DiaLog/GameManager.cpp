#include "GameManager.h"
#include "resource.h"
#include <string>
#include "BitMap.h"
#include "Block.h"
#include "Mecro.h"
#include "ResManager.h"
using namespace std;

GameManager* GameManager::p_This = NULL;


GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

void GameManager::Init(HDC hdc, HINSTANCE g_hInst)
{
	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecBlock.clear();

	m_hdc = hdc;
	m_hInst = g_hInst;

	BackGround->Init(hdc, g_hInst, IDB_BITMAP_BACKGROUND);

	m_GameEnd = false;

	m_iTimer = 0;
	m_recTimer.left = 80;
	m_recTimer.top = 480;
	m_recTimer.right = 209;
	m_recTimer.bottom = 500;

	m_recMine.left = 655;
	m_recMine.top = 480;
	m_recMine.right = 700;
	m_recMine.bottom = 500;

	if (m_Mode == MODE_EASY)
	{
		m_iMine = 10;
		m_Width = 8;
		m_Height = 8;
	}
	else if (m_Mode == MODE_NOMAL)
	{
		m_iMine = 30;
		m_Width = 16;
		m_Height = 16;
	}
	else if (m_Mode == MODE_HARD)
	{
		m_iMine = 85;
		m_Width = 30;
		m_Height = 16;
	}
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 16; j++)
			m_Map[i][j] = INFO_NULL;
	}
	int count = 0;
	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			m_Map[i][j] = INFO_BLOCK0;
			if (count != m_iMine)
			{
				m_Map[i][j] = INFO_MINE;
				count++;
			}
		}
	}
	Suffle();

	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			if (m_Map[i][j] == INFO_MINE)
			{
				if (m_Map[i - 1][j] != INFO_MINE && i - 1 >= 0)
					m_Map[i - 1][j]++;
				if (m_Map[i + 1][j] != INFO_MINE && i + 1 < m_Width)
					m_Map[i + 1][j]++;
				if (m_Map[i][j - 1] != INFO_MINE && j - 1 >= 0)
					m_Map[i][j - 1]++;
				if (m_Map[i][j + 1] != INFO_MINE && j + 1 < m_Height)
					m_Map[i][j + 1]++;
				if (m_Map[i - 1][j - 1] != INFO_MINE && i - 1 >= 0 && j - 1 >= 0)
					m_Map[i - 1][j - 1]++;
				if (m_Map[i - 1][j + 1] != INFO_MINE && i - 1 >= 0 && j + 1 < m_Height)
					m_Map[i - 1][j + 1]++;
				if (m_Map[i + 1][j - 1] != INFO_MINE && i + 1 < m_Width && j - 1 >= 0)
					m_Map[i + 1][j - 1]++;
				if (m_Map[i + 1][j + 1] != INFO_MINE && i + 1 < m_Width && j + 1 < m_Height)
					m_Map[i + 1][j + 1]++;
			}
		}
	}



	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			Block * m_Block = new Block();
			POINT pt;
			pt.x = i;
			pt.y = j;

			m_Block->Init(hdc, g_hInst, m_Map[i][j], pt);

			m_vecBlock.push_back(m_Block);
		}
	}
}

void GameManager::Release()
{
	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecBlock.clear();
	SAFE_DELETE(BackGround);
	SAFE_DELETE(p_This);
	ResManager::GetInstance()->Release();
}

void GameManager::Draw(HDC hdc)
{
	BackGround->Draw(hdc, 0, 0);
	s = to_string(m_iTimer) + "ÃÊ";
	DrawText(hdc, s.c_str(), -1, &m_recTimer, DT_RIGHT);

	s = to_string(m_iMine) + "°³";
	DrawText(hdc, s.c_str(), -1, &m_recMine, DT_RIGHT);

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		(*iter)->Draw(hdc);
	}
}

void GameManager::Suffle()
{
	srand(GetTickCount());
	for (int i = 0; i < 600; i++)
	{
		int indexA = rand() % m_Width;
		int indexB = rand() % m_Height;
		int indexC = rand() % m_Width;
		int indexD = rand() % m_Height;

		int temp = m_Map[indexA][indexB];
		m_Map[indexA][indexB] = m_Map[indexC][indexD];
		m_Map[indexC][indexD] = temp;
	}

}

void GameManager::SetMode(HDC hdc, HINSTANCE g_hInst, int x)
{
	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_vecBlock.clear();

	if (x == 1)
		m_Mode = MODE_EASY;
	else if (x == 2)
		m_Mode = MODE_NOMAL;
	else if (x == 3)
		m_Mode = MODE_HARD;

	
	Init(hdc, g_hInst);
	
}

bool GameManager::CheckUp(int x, int y)
{
	if (m_Map[x][y] > INFO_BLOCK0)
		return false;


	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if (m_Map[x][y - 1] <= INFO_BLOCK8 && y - 1 == ((*iter)->GetRect().top - 47) / 26 && x == ((*iter)->GetRect().left - 42) / 26) //UP
		{
			if ((*iter)->GetOpen())
				return false;
			(*iter)->SetOpen();
			return true;
		}
	}
	return true;
}
bool GameManager::CheckDown(int x, int y)
{
	if (m_Map[x][y] > INFO_BLOCK0)
		return false;


	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if (m_Map[x][y + 1] <= INFO_BLOCK8 && y + 1 == ((*iter)->GetRect().top - 47) / 26 && x == ((*iter)->GetRect().left - 42) / 26) //UP
		{
			if ((*iter)->GetOpen())
				return false;
			(*iter)->SetOpen();
			return true;
		}
	}
	return true;
}

bool GameManager::CheckLeft(int x, int y)
{
	if (m_Map[x][y] > INFO_BLOCK0)
		return false;

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if (m_Map[x - 1][y] <= INFO_BLOCK8 && x - 1 == ((*iter)->GetRect().left - 42) / 26 && y == ((*iter)->GetRect().top - 47) / 26) //LEFT
		{
			if ((*iter)->GetOpen())
				return false;
			(*iter)->SetOpen();
			return true;
		}
	}
	return true;
}

bool GameManager::CheckRight(int x, int y)
{
	if (m_Map[x][y] > INFO_BLOCK0)
		return false;

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if (m_Map[x + 1][y] <= INFO_BLOCK8 && x + 1 == ((*iter)->GetRect().left - 42) / 26 && y == ((*iter)->GetRect().top - 47) / 26) //LEFT
		{
			if ((*iter)->GetOpen())
				return false;
			(*iter)->SetOpen();
			return true;
		}
	}
	return true;
}

bool GameManager::CheckRightUp(int x, int y)
{
	if (m_Map[x][y] > INFO_BLOCK0)
		return false;

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if (m_Map[x + 1][y - 1] <= INFO_BLOCK8 && x + 1 == ((*iter)->GetRect().left - 42) / 26 && y - 1 == ((*iter)->GetRect().top - 47) / 26) //Rightup
		{
			if ((*iter)->GetOpen())
				return false;
			(*iter)->SetOpen();
			return true;
		}
	}
	return true;
}

bool GameManager::CheckRightDown(int x, int y)
{
	if (m_Map[x][y] > INFO_BLOCK0)
		return false;

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if (m_Map[x + 1][y + 1] <= INFO_BLOCK8 && x + 1 == ((*iter)->GetRect().left - 42) / 26 && y + 1 == ((*iter)->GetRect().top - 47) / 26) //Rightdown
		{
			if ((*iter)->GetOpen())
				return false;
			(*iter)->SetOpen();
			return true;
		}
	}
	return true;
}

bool GameManager::CheckLeftUp(int x, int y)
{
	if (m_Map[x][y] > INFO_BLOCK0)
		return false;

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if (m_Map[x - 1][y - 1] <= INFO_BLOCK8 && x - 1 == ((*iter)->GetRect().left - 42) / 26 && y - 1 == ((*iter)->GetRect().top - 47) / 26) //LEFT
		{
			if ((*iter)->GetOpen())
				return false;
			(*iter)->SetOpen();
			return true;
		}
	}
	return true;
}

bool GameManager::CheckLeftDown(int x, int y)
{
	if (m_Map[x][y] > INFO_BLOCK0)
		return false;

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if (m_Map[x - 1][y + 1] <= INFO_BLOCK8 && x - 1 == ((*iter)->GetRect().left - 42) / 26 && y + 1 == ((*iter)->GetRect().top - 47) / 26) //LEFT
		{
			if ((*iter)->GetOpen())
				return false;
			(*iter)->SetOpen();
			return true;
		}
	}
	return true;
}

void GameManager::CheckEmptyBlock(int x, int y)
{
	if (x >= 0 && y >= 0 && x < m_Width && y < m_Height)
	{
		if (CheckUp(x, y))
		{
			CheckEmptyBlock(x, y - 1);
		}
		if (CheckDown(x, y))
		{
			CheckEmptyBlock(x, y + 1);
		}
		if (CheckLeft(x, y))
		{
			CheckEmptyBlock(x - 1, y);
		}
		if (CheckRight(x, y))
		{
			CheckEmptyBlock(x + 1, y);
		}
		if (CheckRightUp(x, y))
		{
			CheckEmptyBlock(x + 1, y - 1);
		}
		if (CheckLeftUp(x, y))
		{
			CheckEmptyBlock(x - 1, y - 1);
		}
		if (CheckRightDown(x, y))
		{
			CheckEmptyBlock(x + 1, y + 1);
		}
		if (CheckLeftDown(x, y))
		{
			CheckEmptyBlock(x - 1, y + 1);
		}
	}
	return;
}

void GameManager::CheckBlock(int x, int y)
{

	int mine = m_Map[x][y];

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if (x == ((*iter)->GetRect().left - 42) / 26 && y + 1 == ((*iter)->GetRect().top - 47) / 26 && (*iter)->GetBlockInfo() == INFO_MINE && (*iter)->GetFlag())
			mine--; //down
		if (x == ((*iter)->GetRect().left - 42) / 26 && y - 1 == ((*iter)->GetRect().top - 47) / 26 && (*iter)->GetBlockInfo() == INFO_MINE && (*iter)->GetFlag())
			mine--;
		if (x - 1 == ((*iter)->GetRect().left - 42) / 26 && y == ((*iter)->GetRect().top - 47) / 26 && (*iter)->GetBlockInfo() == INFO_MINE && (*iter)->GetFlag())
			mine--;
		if (x + 1 == ((*iter)->GetRect().left - 42) / 26 && y == ((*iter)->GetRect().top - 47) / 26 && (*iter)->GetBlockInfo() == INFO_MINE && (*iter)->GetFlag())
			mine--;

		if (x - 1 == ((*iter)->GetRect().left - 42) / 26 && y + 1 == ((*iter)->GetRect().top - 47) / 26 && (*iter)->GetBlockInfo() == INFO_MINE && (*iter)->GetFlag())
			mine--;
		if (x - 1 == ((*iter)->GetRect().left - 42) / 26 && y - 1 == ((*iter)->GetRect().top - 47) / 26 && (*iter)->GetBlockInfo() == INFO_MINE && (*iter)->GetFlag())
			mine--;
		if (x + 1 == ((*iter)->GetRect().left - 42) / 26 && y + 1 == ((*iter)->GetRect().top - 47) / 26 && (*iter)->GetBlockInfo() == INFO_MINE && (*iter)->GetFlag())
			mine--;
		if (x + 1 == ((*iter)->GetRect().left - 42) / 26 && y - 1 == ((*iter)->GetRect().top - 47) / 26 && (*iter)->GetBlockInfo() == INFO_MINE && (*iter)->GetFlag())
			mine--;

	}
	if (mine > INFO_BLOCK0)
		return;


	if (x >= 0 && y >= 0 && x < m_Width && y < m_Height)
	{

		if (m_Map[x][y + 1] <= INFO_BLOCK8 && x >= 0 && y + 1 < m_Height) //down
				for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
				{
					if (x == ((*iter)->GetRect().left - 42) / 26 && y + 1 == ((*iter)->GetRect().top - 47) / 26)
						(*iter)->SetOpen();
				}
		if (m_Map[x][y + 1] == INFO_BLOCK0)
				CheckEmptyBlock(x, y + 1);

		if (m_Map[x - 1][y + 1] <= INFO_BLOCK8 && x - 1 >= 0 && y + 1 < m_Height) //Left Down
			
				for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
				{
					if (x - 1 == ((*iter)->GetRect().left - 42) / 26 && y + 1 == ((*iter)->GetRect().top - 47) / 26)
						(*iter)->SetOpen();
				}
		if (m_Map[x - 1][y + 1] == INFO_BLOCK0)
			CheckEmptyBlock(x - 1, y + 1);

		if (m_Map[x - 1][y] <= INFO_BLOCK8 && x - 1 >= 0 && y < m_Height) //Left
			for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
				{
					if (x - 1 == ((*iter)->GetRect().left - 42) / 26 && y == ((*iter)->GetRect().top - 47) / 26)
						(*iter)->SetOpen();
				}
		if (m_Map[x - 1][y] == INFO_BLOCK0)
			CheckEmptyBlock(x - 1, y);


		if (m_Map[x - 1][y - 1] <= INFO_BLOCK8 && x - 1 >= 0 && y - 1 < m_Height) //LeftUp
			for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
				{
					if (x - 1 == ((*iter)->GetRect().left - 42) / 26 && y - 1 == ((*iter)->GetRect().top - 47) / 26)
						(*iter)->SetOpen();
				}
		if (m_Map[x - 1][y - 1] == INFO_BLOCK0)
			CheckEmptyBlock(x - 1, y + 1);


		if (m_Map[x][y - 1] <= INFO_BLOCK8 && x >= 0 && y - 1 >= 0) //Up
			for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
				{
					if (x == ((*iter)->GetRect().left - 42) / 26 && y - 1 == ((*iter)->GetRect().top - 47) / 26)
						(*iter)->SetOpen();
				}
		if (m_Map[x][y - 1] == INFO_BLOCK0)
			CheckEmptyBlock(x, y - 1);

		if (m_Map[x + 1][y - 1] <= INFO_BLOCK8 && x + 1 >= 0 && y - 1 >= 0) //RightUp
			for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
				{
					if (x + 1 == ((*iter)->GetRect().left - 42) / 26 && y - 1 == ((*iter)->GetRect().top - 47) / 26)
						(*iter)->SetOpen();
				}
		if (m_Map[x + 1][y - 1] == INFO_BLOCK0)
			CheckEmptyBlock(x + 1, y - 1);

		if (m_Map[x + 1][y] <= INFO_BLOCK8 && x + 1 >= 0 && y >= 0) //Right
			for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
				{
					if (x + 1 == ((*iter)->GetRect().left - 42) / 26 && y == ((*iter)->GetRect().top - 47) / 26)
						(*iter)->SetOpen();
				}
		if (m_Map[x + 1][y] == INFO_BLOCK0)
			CheckEmptyBlock(x + 1, y);

		if (m_Map[x + 1][y + 1] <= INFO_BLOCK8 && x + 1 >= 0 && y + 1 >= 0) //RightDown
			for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
				{
					if (x + 1 == ((*iter)->GetRect().left - 42) / 26 && y + 1 == ((*iter)->GetRect().top - 47) / 26)
						(*iter)->SetOpen();
				}
		if (m_Map[x + 1][y + 1] == INFO_BLOCK0)
			CheckEmptyBlock(x + 1, y + 1);


	}
	return;
}

void GameManager::LBClick(HWND hWnd, int x, int y)
{
	if (m_GameEnd)
		return;
	POINT pt;
	pt.x = x;
	pt.y = y;
	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if (PtInRect(&(*iter)->GetRect(), pt))
		{
			if ((*iter)->GetFlag())
				return;
			(*iter)->SetOpen();
			if ((*iter)->GetBlockInfo() == INFO_MINE)
			{
				m_GameEnd = true;
				if (MessageBox(hWnd, "GAME OVER", "GAMEOVER", MB_OK))
				{
					for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
					{
						SAFE_DELETE(*iter);
					}
					m_vecBlock.clear();

					HDC hdc;
					hdc = GetDC(hWnd);
					Init(hdc, m_hInst);
					ReleaseDC(hWnd, hdc);
					InvalidateRect(hWnd, NULL, TRUE);
				}

				return;
			}
			if ((*iter)->GetBlockInfo() == INFO_BLOCK0)
				CheckEmptyBlock(((*iter)->GetRect().left - 42) / 26, ((*iter)->GetRect().top - 47) / 26);
		}
	}
}

void GameManager::LBDClick(HWND hWnd, int x, int y)
{
	if (m_GameEnd)
		return;
	POINT pt;
	pt.x = x;
	pt.y = y;

	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if (PtInRect(&(*iter)->GetRect(), pt))
		{
			if ((*iter)->GetFlag())
				return;

			(*iter)->SetOpen();
			if ((*iter)->GetBlockInfo() == INFO_MINE)
			{
				m_GameEnd = true;
				if (MessageBox(hWnd, "GAME OVER", "GAMEOVER", MB_OK))
				{
					for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
					{
						SAFE_DELETE(*iter);
					}
					m_vecBlock.clear();

					HDC hdc;
					hdc = GetDC(hWnd);
					Init(hdc, m_hInst);
					ReleaseDC(hWnd, hdc);
					InvalidateRect(hWnd, NULL, TRUE);

				}
				return;
			}
			if ((*iter)->GetBlockInfo() <= INFO_BLOCK8)
				CheckBlock(((*iter)->GetRect().left - 42) / 26, ((*iter)->GetRect().top - 47) / 26);
		}
	}
}

void GameManager::RBClick(HWND hWnd, int x, int y)
{
	if (m_GameEnd)
		return;

	POINT pt;
	pt.x = x;
	pt.y = y;


	for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
	{
		if (PtInRect(&(*iter)->GetRect(), pt))
		{
			if ((*iter)->GetOpen())
				return;

			(*iter)->SetFlag();
			if ((*iter)->GetFlag())
				m_iMine--;
			else
				m_iMine++;
		}
	}

	if (m_iMine == 0)
	{
		for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
		{
			(*iter)->SetOpen();
		}

		vector<Block*>::iterator iter;
		for (iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
		{
			if ((*iter)->GetBlockInfo() == INFO_MINE && (*iter)->GetOpen() && !(*iter)->GetFlag())
			{
				m_GameEnd = true;
				break;
			}
		}
		if (iter != m_vecBlock.end())
			MessageBox(hWnd, "Game OVER", "GAME OVER", MB_OK);
		else
			MessageBox(hWnd, "YOU WIN", "GAME OVER", MB_OK);
		m_GameEnd = true;
		
		for (auto iter = m_vecBlock.begin(); iter != m_vecBlock.end(); iter++)
		{
			SAFE_DELETE(*iter);
		}
		m_vecBlock.clear();

		HDC hdc;
		hdc = GetDC(hWnd);
		Init(hdc, m_hInst);
		ReleaseDC(hWnd, hdc);
		InvalidateRect(hWnd, NULL, TRUE);

		return;
	}
}