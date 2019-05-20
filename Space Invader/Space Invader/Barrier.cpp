#include "Barrier.h"
#include "ResManager.h"
#include "BitMap.h"


void Barrier::Init(int x)
{
	m_Position.x = GAME_WIDTH / NUM_BARRIER * x + 43;
	m_Position.y = GAME_HEIGHT - BITMAP_HEIGHT - 34 - 100;
}

void Barrier::Draw(HDC hdc)
{
	ResManager::GetInstance()->GetImg(INFO_BARRIER)->Draw(hdc, m_Position.x, m_Position.y);	
}

RECT Barrier::GetRect()
{
	RECT rt;
	rt.left = m_Position.x;
	rt.top = m_Position.y;
	rt.right = rt.left + 43;
	rt.bottom = rt.top + 34;
	return rt;
}

Barrier::Barrier()
{
}


Barrier::~Barrier()
{
}
