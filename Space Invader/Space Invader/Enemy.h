#pragma once
#include <Windows.h>
#include "ResManager.h"

class Enemy
{
private:
	POINT m_Position;
	bool m_Die;
	IMG_RES m_enemy;
	int m_DieCount;
	bool m_Direction;
	bool m_bMove;
	int m_Score;
	int m_Life;
public:
	void Init(IMG_RES info_enemy, int x, int y);
	int GetLife()
	{
		return m_Life;
	}
	int GetScore()
	{
		return m_Score;
	}
	void ReverseDirection(bool direction);
	
	bool GetDirection()
	{
		return m_Direction;
	}
	void Draw(HDC hdc);
	void Update();
	void Move(int key);
	IMG_RES GetEnemyID()
	{
		return m_enemy;
	}
	int Get_XPosition()
	{
		return m_Position.x;
	}
	int Get_YPosition()
	{
		return m_Position.y;
	}
	RECT GetRect();
	void SetDie();
	bool GetDie()
	{
		return m_Die;
	}
	void Live();
	
	Enemy();
	~Enemy();
};

