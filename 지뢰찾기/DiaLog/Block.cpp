#include "Block.h"
#include "BitMap.h"
#include "resource.h"
#include "Mecro.h"
#include "ResManager.h"
Block::Block()
{
	
}


Block::~Block()
{
}

void Block::Init(HDC hdc, HINSTANCE g_hInst,int blockinfo, POINT pt)
{
	m_hdc = hdc;
	m_hInst = g_hInst;
	m_BlockInfo = blockinfo;
	m_Block = ResManager::GetInstance()->GetImage(blockinfo);
	m_BackBlock = ResManager::GetInstance()->GetImage(INFO_BACKBLOCK);
	m_FlagBlock = ResManager::GetInstance()->GetImage(INFO_FLAG);
	m_pt = pt;
	m_bOpen = false;
	m_bFlag = false;
}

void Block::Draw(HDC hdc)
{
	if (m_bFlag)
		m_FlagBlock->Draw(hdc, m_pt.x * 26 + 42, m_pt.y * 26 + 47);
	else if (m_bOpen)
		m_Block->Draw(hdc, m_pt.x * 26 + 42, m_pt.y * 26 + 47);
	else
		m_BackBlock->Draw(hdc, m_pt.x * 26 + 42, m_pt.y * 26 + 47);
}

RECT Block::GetRect()
{
	RECT rt;
	rt.left = m_pt.x * 26 + 42;
	rt.top = m_pt.y * 26 + 47;
	rt.right = rt.left + 26;
	rt.bottom = rt.top + 26;

	return rt;
}
void Block::SetBlock(int blockinfo)
{
	m_Block->Init(m_hdc, m_hInst, blockinfo);
}
void Block::SetOpen()
{
	m_bOpen = true;
}
void Block::SetFlag()
{
	if (!m_bOpen)
		if (m_bFlag)
			m_bFlag = false;
		else
			m_bFlag = true;

}