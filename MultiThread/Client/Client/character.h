#pragma once
#include "GlobalDefine.h"
#include "POINT.h"
#include "BitMap.h"

class character
{
private:
	HEngine::POINT		PlayerPt;
	HEngine::POINT		BackPt;
	HEngine::BitMap *	m_bitPlayer[END + 1];
	float				m_fTime;
	HEngine::POINTF		m_DrawPt;
	bool				m_bMove;
	DIRECT				m_State;
	int					m_Speed;
	int					m_BubblePower;

public:
	character();
	~character();

	void SetState(DIRECT State);

	int GetState();
	void Init();
	bool Update();
	void SetPt(HEngine::POINT pt);
	void Draw();
	void PowerUp();
	int GetPower();
	int GetSpeed();
	HEngine::POINTF GetDrawPT();
	HEngine::POINT GetPt();
	HEngine::POINT GetBackPt();

};

