#pragma once
#include <Windows.h>
#include <vector>

using namespace std;

class Player
{
private:
	POINT m_Position;
	bool m_Die;
	int m_PlayerCenter;
	int m_Count;
	int m_Life;
public:
	void Init();
	void Live();
	void Die();
	int GetLife()
	{
		return m_Life;
	}
	void Draw(HDC hdc);
	void Move(int key);
	RECT GetRect();
	int GetPlayerCenter();
	void Release();
	Player();
	~Player();
};

