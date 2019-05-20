#pragma once
#include <Windows.h>
class Player
{
private:
	int m_State;
	POINT m_Position;
	int m_Move;
	POINT m_JumpPoint;
	int m_Radian;
	bool m_bjump;
	int m_jumpDirection;
	int Turn_Count;
public:
	void Init();
	void Draw(HDC hdc);
	void SetPosition(int move);
	int GetPosition()
	{
		return m_Move;
	}
	void Move(int key);
	void Jump();
	bool GetJump()
	{
		return m_bjump;
	}
	void Die();
	void Win();
	RECT GetRect();
	Player();
	~Player();
};

