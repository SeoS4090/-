#pragma once
#include <Windows.h>
#include "resource.h"
class BitMap;
class Player;
struct FIRE_STRUCT
{
	BitMap * m_Fire1;
	BitMap * m_Fire2;
	POINT m_iMove;
	bool m_bPrev;
	
	int m_InitPosition;
};
class Fire
{
private:
	FIRE_STRUCT arr[FIRE_COUNT];
	bool m_bTurn;
	bool m_bWin;
	bool m_bCash;
	bool m_bDie;
	int m_Cashturn;
	int m_iTurn_Count;
	int m_iMove;
	
public:
	void Init();	
	bool GetWin()
	{
		return m_bWin;
	}
	void Die()
	{
		m_Cashturn = 100;
		m_bDie = true;
	}
	bool GetCash()
	{
		return m_bCash;
	}
	void SetPosition(int x);
	void Draw(HDC hdc , Player *player);
	void SetMove(int move);
	void Update();
	void SetBonus();
	bool Collider(RECT rt);
	Fire();
	~Fire();
};

