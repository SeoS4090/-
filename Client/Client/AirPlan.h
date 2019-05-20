#pragma once
#include "GlobalDefine.h"
#include "BitMap.h"
#include "POINT.h"

class AirPlan
{
private:
	HEngine::BitMap *	m_Player[2];
	HEngine::BitMap *	m_Explosion[3];
	HEngine::POINTF m_point;
	bool m_bDie;
	int m_iDieCount;
	float m_DuringTime;
public:
	AirPlan();
	void Init(HEngine::POINTF pt);
	void Update(float fEtime, HEngine::POINTF pt);
	void Draw();
	void Die();
	void Resurrection();
	bool GetDie()
	{
		return m_bDie;
	}
	HEngine::RECT GetRect();
	~AirPlan();
};

