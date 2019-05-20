#pragma once
#include <Windows.h>
#include <vector>
using namespace std;
class Enemy;
class Bullet;
class EnemyManager
{
private:
	vector<Enemy *> m_vecEnemy;
	int m_CheckCount;
	bool m_EnemyDirection;
	vector<Enemy *>::iterator m_iter;
	vector<Bullet *> m_vecBullet;
	Enemy * m_Ufo;
	int m_Score;
	int m_UfoBonus;
public:
	void init();
	int GetScore();
	void Draw(HDC hdc);
	void Update();
	bool CheckCollider(RECT rec_bullet);
	void Release();
	EnemyManager();
	~EnemyManager();
};

