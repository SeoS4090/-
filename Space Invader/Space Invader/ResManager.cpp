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

	m_arrImg[INFO_PLAYER]->Init(hdc, g_hInst, "./RES/baseshipa.bmp");
	m_arrImg[INFO_ENEMY1]->Init(hdc, g_hInst, "./RES/saucer1a.bmp");
	m_arrImg[INFO_ENEMY2]->Init(hdc, g_hInst, "./RES/saucer2a.bmp");
	m_arrImg[INFO_ENEMY3]->Init(hdc, g_hInst, "./RES/saucer3a.bmp");
	m_arrImg[INFO_ENEMY1_b]->Init(hdc, g_hInst, "./RES/saucer1b.bmp");
	m_arrImg[INFO_ENEMY2_b]->Init(hdc, g_hInst, "./RES/saucer2b.bmp");
	m_arrImg[INFO_ENEMY3_b]->Init(hdc, g_hInst, "./RES/saucer3b.bmp");
	m_arrImg[INFO_UFO]->Init(hdc, g_hInst, "./RES/mysterya.bmp");
	
	m_arrImg[INFO_BOOM1]->Init(hdc, g_hInst, "./RES/boom1.bmp");
	m_arrImg[INFO_BOOM2]->Init(hdc, g_hInst, "./RES/boom2.bmp");
	m_arrImg[INFO_BOOM3]->Init(hdc, g_hInst, "./RES/boom3.bmp");

	m_arrImg[INFO_BACKGROUND]->Init(hdc, g_hInst, "./RES/background.bmp");
	m_arrImg[INFO_BULLET]->Init(hdc, g_hInst, "./RES/bullet2.bmp");
	m_arrImg[INFO_BULLET2]->Init(hdc, g_hInst, "./RES/bullet.bmp");
	m_arrImg[INFO_BULLET_B]->Init(hdc, g_hInst, "./RES/bullet2_b.bmp");
	m_arrImg[INFO_BULLET2_B]->Init(hdc, g_hInst, "./RES/bullet_b.bmp");

	m_arrImg[INFO_START]->Init(hdc, g_hInst, "./RES/Start.bmp");
	m_arrImg[INFO_STRINGEND]->Init(hdc, g_hInst, "./RES/END.bmp");
	m_arrImg[INFO_BARRIER]->Init(hdc, g_hInst, "./RES/barrier.bmp");
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