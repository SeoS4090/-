#pragma once
#include <Windows.h>

class BitMap;



class Block
{
private:
	HDC m_hdc;
	HINSTANCE m_hInst;
	bool m_bOpen;
	bool m_bFlag;
	int m_BlockInfo;
	POINT m_pt;
	BitMap * m_Block;
	BitMap * m_BackBlock;
	BitMap * m_FlagBlock;

public:
	void Init(HDC hdc, HINSTANCE g_hInst,int blockinfo, POINT pt);
	void Draw(HDC hdc);
	void SetBlock(int blockinfo);
	void SetOpen();
	void SetFlag();
	int GetBlockInfo()
	{
		return m_BlockInfo;
	}
	bool GetFlag()
	{
		return m_bFlag;
	}
	bool GetOpen()
	{
		return m_bOpen;
	}
	RECT GetRect();
	Block();
	~Block();
};

