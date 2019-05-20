#pragma once
#include <Windows.h>

class Bullet
{
private:
	POINT m_Position;
	bool m_bDie;
	bool m_bEnemy;
	RECT SaveRt;
public:
	void Init(int x, int y, bool enemy);
	void Update();
	void Draw(HDC hdc);
	RECT GetRect();

	void Die()
	{
		m_bDie = true;
	}
	bool GetEnemyBullet()
	{
		return m_bEnemy;
	}
	bool GetDie();
	bool CheckCollider(RECT rt);
	Bullet();
	~Bullet();
};

