#include "ResManager.h"
#include "resource.h"
#include "BitMap.h"
#include "Mecro.h"

ResManager* ResManager::s_this = NULL;


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

	m_arrImg[INFO_BACKGROUND]->Init(hdc,g_hInst, IDB_BITMAP_BACKGROUND);
	m_arrImg[INFO_BACKBLOCK]->Init(hdc, g_hInst, IDB_BITMAP_BLOCK);
	m_arrImg[INFO_BLOCK0]->Init(hdc, g_hInst, IDB_BITMAP_OPENBLOCK);
	m_arrImg[INFO_BLOCK1]->Init(hdc, g_hInst, IDB_BITMAP1);
	m_arrImg[INFO_BLOCK2]->Init(hdc, g_hInst, IDB_BITMAP2);
	m_arrImg[INFO_BLOCK3]->Init(hdc, g_hInst, IDB_BITMAP3);
	m_arrImg[INFO_BLOCK4]->Init(hdc, g_hInst, IDB_BITMAP4);
	m_arrImg[INFO_BLOCK5]->Init(hdc, g_hInst, IDB_BITMAP5);
	m_arrImg[INFO_BLOCK6]->Init(hdc, g_hInst, IDB_BITMAP6);
	m_arrImg[INFO_BLOCK7]->Init(hdc, g_hInst, IDB_BITMAP7);
	m_arrImg[INFO_BLOCK8]->Init(hdc, g_hInst, IDB_BITMAP8);
	m_arrImg[INFO_FLAG]->Init(hdc, g_hInst, IDB_BITMAP_FLAG);
	m_arrImg[INFO_MINE]->Init(hdc, g_hInst, IDB_BITMAP_MINE);
}

void ResManager::Release()
{
	for (int i = 0; i < INFO_END; i++)
	{
		SAFE_DELETE(m_arrImg[i]);
	}
	SAFE_DELETE(s_this);
}

BitMap* ResManager::GetImage(int index)
{

	return m_arrImg[index];
}