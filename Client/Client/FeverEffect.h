#pragma once
#include "GlobalDefine.h"
#include "Defines.h"
#include "POINT.h"
class FeverEffect
{
private:
	HEngine::POINTF m_pt;
	float	m_fFeverPercent;
	float	m_fFeverRateDraw;
	bool	m_bFever;
	string	m_sFeverRes[2];
	int		m_iFeverRes;
	float	m_FeverTime;
public:
	void Draw();
	bool Update(float fEtime, ColliderType collider);
	void MinusFeverRate(float rate);
	inline bool GetFever()
	{
		return m_bFever;
	}
	FeverEffect();
	~FeverEffect();
};