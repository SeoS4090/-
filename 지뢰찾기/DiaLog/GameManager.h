#pragma once
#include <Windows.h>
#include <vector>
#include "BitMap.h"
#include "ResManager.h"
class Block;
enum GAME_MODE
{
	MODE_EASY,
	MODE_NOMAL,
	MODE_HARD
};

class GameManager
{
private:
	static GameManager * p_This;
	bool m_GameEnd;
	int m_iTimer;
	int m_Mode;
	int m_Height;
	int m_Width;
	RECT m_recTimer;
	RECT m_recMine;
	std::string s;
	int m_iMine;
	int m_Map[30][16];
	HDC m_hdc;
	HINSTANCE m_hInst;
	BitMap* BackGround;
	std::vector<Block *> m_vecBlock;

public:
	static GameManager* GetInstance()
	{
		if (p_This == NULL)
		{
			p_This = new GameManager();
			p_This->BackGround = new BitMap();
			p_This->SetMode(MODE_EASY);
		}
		return p_This;
	}
	GameManager();
	~GameManager();
	void SetMode(GAME_MODE mode)
	{
		m_Mode = mode;
	}
	void Init(HDC hdc , HINSTANCE g_hInst);
	void Release();
	void Draw(HDC hdc);
	void Timer() {
		if(!m_GameEnd)
			m_iTimer++;
	}
	void Suffle();
	void SetMode(HDC hdc, HINSTANCE g_hInst,int x);
	
	void LBClick(HWND hWnd, int x, int y);
	void LBDClick(HWND hWnd, int x, int y);
	void RBClick(HWND hWnd, int x, int y);
	
	void CheckBlock(int x, int y);
	void CheckEmptyBlock(int x, int y);


	bool CheckUp(int x, int y);
	bool CheckDown(int x, int y);
	bool CheckLeft(int x, int y);
	bool CheckRight(int x, int y);
	bool CheckRightUp(int x, int y);
	bool CheckLeftUp(int x, int y);
	bool CheckRightDown(int x, int y);
	bool CheckLeftDown(int x, int y);

};