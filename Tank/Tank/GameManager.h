#pragma once
#include <Windows.h>
#include <vector>
#include "resource.h"
using namespace std;

class BackGround;
class Tank;
class Bullet;
struct TankStruct
{
	Tank * tank;
	Bullet * bullet[MAX_BULLET];
	float		m_fBulletTime;
};
class GameManager
{
	static GameManager* p_This;
	HBITMAP			m_hBitMap;
	HBITMAP			m_hOldBit;
	HDC				BackDC;
	HWND			m_hWnd;

	DWORD			m_dwLastTime;
	DWORD			m_dwCurTime;
	float			m_fElapseTime;
	float			m_fDuringTime;
	
	BackGround*		m_BackGround;
	vector<INFO *>	m_vecMap;
	vector<INFO *>	m_vecGrass;

	vector<TankStruct> m_vecStructEnmey;
	TankStruct		m_structPlayer;
	int				m_iScore;
	int				m_iStage;
public:
	static GameManager * GetInstance()
	{
		if (p_This == NULL)
			p_This = new GameManager;
		return p_This;
	}
	bool CreateEnemy();
	void Init(HWND hWnd, HINSTANCE g_hInst);
	void LoadMap(int a);
	void Update();
	void Draw();
	void Release();
	GameManager();
	~GameManager();
};

