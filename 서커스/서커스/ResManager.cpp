#include "ResManager.h"
#include "Mecro.h"
#include "BitMap.h"
ResManager* ResManager::s_This = NULL;

ResManager::ResManager()
{
}


ResManager::~ResManager()
{
}

void ResManager::Init(HDC hdc, HINSTANCE g_hInst)
{
	for (int i = 0; i < INFO_END; i++)
	{
		m_arrImg[i] = new BitMap();

	}

	m_arrImg[BACK]->Init(hdc, g_hInst, "./RES/back.bmp");
	m_arrImg[BACKDECO]->Init(hdc, g_hInst, "./RES/back_deco.bmp");
	m_arrImg[BACKNOMAL]->Init(hdc, g_hInst, "./RES/back_normal.bmp");
	m_arrImg[BACKNOMAL2]->Init(hdc, g_hInst, "./RES/back_normal2.bmp");
	m_arrImg[CASH]->Init(hdc, g_hInst, "./RES/cash.bmp");

	m_arrImg[DIE]->Init(hdc, g_hInst, "./RES/die.bmp");

	m_arrImg[END]->Init(hdc, g_hInst, "./RES/end.bmp");

	m_arrImg[ENEMY]->Init(hdc, g_hInst, "./RES/enemy.bmp");
	m_arrImg[ENEMY_1B]->Init(hdc, g_hInst, "./RES/enemy_1b.bmp");

	m_arrImg[ENEMY_1F]->Init(hdc, g_hInst, "./RES/enemy_1f.bmp");

	m_arrImg[ENEMY_B]->Init(hdc, g_hInst, "./RES/enemy_b.bmp");

	m_arrImg[ENEMY_F]->Init(hdc, g_hInst, "./RES/enemy_f.bmp");

	m_arrImg[ENEMY_L]->Init(hdc, g_hInst, "./RES/enemy_l.bmp");

	m_arrImg[ENEMY_L_B]->Init(hdc, g_hInst, "./RES/enemy_l_b.bmp");

	m_arrImg[ENEMY_L_F]->Init(hdc, g_hInst, "./RES/enemy_l_f.bmp");

	m_arrImg[ENEMY1]->Init(hdc, g_hInst, "./RES/enemy1.bmp");
	//m_arrImg[ENEMY1]->SetSize(120, 200);

	m_arrImg[FRONT]->Init(hdc, g_hInst, "./RES/front.bmp");
	m_arrImg[FRONT2]->Init(hdc, g_hInst, "./RES/front2.bmp");
	m_arrImg[ICON]->Init(hdc, g_hInst, "./RES/icon.bmp");

	m_arrImg[MITER]->Init(hdc, g_hInst, "./RES/miter.bmp");

	m_arrImg[PLAYER0]->Init(hdc, g_hInst, "./RES/player0.bmp");

	m_arrImg[PLAYER1]->Init(hdc, g_hInst, "./RES/player1.bmp");

	m_arrImg[PLAYER2]->Init(hdc, g_hInst, "./RES/player2.bmp");

	m_arrImg[STAR]->Init(hdc, g_hInst, "./RES/star.bmp");

	m_arrImg[STAR1]->Init(hdc, g_hInst, "./RES/star1.bmp");

	m_arrImg[STAR2]->Init(hdc, g_hInst, "./RES/star2.bmp");

	m_arrImg[WIN]->Init(hdc, g_hInst, "./RES/win.bmp");

	m_arrImg[WIN2]->Init(hdc, g_hInst, "./RES/win2.bmp");
	m_arrImg[PANEL]->Init(hdc, g_hInst, "./RES/Panel.bmp");
	m_arrImg[INT_1]->Init(hdc, g_hInst, "./RES/1.bmp");
	m_arrImg[INT_2]->Init(hdc, g_hInst, "./RES/2.bmp");
	m_arrImg[INT_3]->Init(hdc, g_hInst, "./RES/3.bmp");
	m_arrImg[INT_4]->Init(hdc, g_hInst, "./RES/4.bmp");

	m_arrImg[INT_5]->Init(hdc, g_hInst, "./RES/5.bmp");

	m_arrImg[INT_6]->Init(hdc, g_hInst, "./RES/6.bmp");

	m_arrImg[INT_7]->Init(hdc, g_hInst, "./RES/7.bmp");

	m_arrImg[INT_8]->Init(hdc, g_hInst, "./RES/8.bmp");

	m_arrImg[INT_9]->Init(hdc, g_hInst, "./RES/9.bmp");
	m_arrImg[INT_0]->Init(hdc, g_hInst, "./RES/0.bmp");
	
	m_arrImg[CASHSCORE]->Init(hdc, g_hInst, "./RES/CashScore.bmp");

}

void ResManager::Release()
{
	for (int i = 0; i < INFO_END; i++)
	{
		SAFE_DELETE(m_arrImg[i]);
	}
	SAFE_DELETE(s_This);
}

BitMap * ResManager::GetImg(int x)
{
	return m_arrImg[x];
}
