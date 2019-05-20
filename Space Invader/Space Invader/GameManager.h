#pragma once
#include <Windows.h>
#include <vector>
using namespace std;

class BitMap;
class Player;
class EnemyManager;
class BackGround;
class Barrier;
class Bullet;
class Enemy;

enum GAME_STATE
{
	START,
	GAME,
	WAIT,
	END
};
class GameManager
{
private:
	HDC BackDC;
	HBITMAP m_hBitmap;
	HBITMAP m_Oldbitmap;
	static GameManager* p_This;
	Enemy * m_arrEnemy[30];
	Enemy * m_Ufo;
	Enemy * m_Player;

	BackGround * m_BackGround;
	Barrier * m_Barrier[4];
	vector<Bullet*> m_vecBullet;
	
	int m_iBullet_Count;
	int m_iEnemy_Count;
	int m_DrawCount;
	bool m_Direction;
	GAME_STATE m_state;
public:
	static GameManager* GetInstance()
	{
		if (p_This == NULL)
		{
			p_This = new GameManager();
		}
		return p_This;
	}
	void Init(HDC hdc, HINSTANCE g_hInst);
	void Draw(HDC hdc);
	void Update(HWND hWnd);
	void Release();
	void Move(int key);
	void ReStart();

	GameManager();
	~GameManager();
};
