#pragma once
#include "Object.h"
#include "GlobalDefine.h"
#include "Defines.h"

class ScoreStar : public Object
{
private:
	
	int m_iScore;
	int m_iCombo;
	float m_fFeverRate;
	float m_fFeverRateDraw;
	bool m_bFeverState;
	int m_iSumScore;
	string m_SscoreRes[2];
	int  m_iScoreRes;
	float m_fFeverTime;
public:
	virtual void Init();
	virtual void Draw();
	virtual void Add();
	virtual void Clear();
	virtual bool Update(float fEtime, HEngine::POINT airplan);
	ScoreStar();
	~ScoreStar();
};

