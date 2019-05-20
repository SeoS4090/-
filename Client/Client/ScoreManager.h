#pragma once
#include "Defines.h"
#include "POINT.h"
#include "GlobalDefine.h"
class ScoreManager
{
	enum SCORE_DIRECT
	{
		UP,
		DOWN,
		RIGHT,
		LEFT,
		UPLEFT,
		UPRIGHT,
		DOWNLEFT,
		DOWNRIGHT
	};
	struct SCORE_Info
	{
		HEngine::POINTF pt;
		SCORE_DIRECT direct;
		bool m_bFadeOut;
	};
	int m_iScore;
	int m_iCombo;
	float m_fFeverRate;
	float m_fFeverRateDraw;
	bool m_bFeverState;
	int m_iSumScore;
	string m_SscoreRes[2];
	int  m_iScoreRes;
	float m_fFeverTime;
	vector<SCORE_Info*> m_vecScore;

public:
	ScoreManager();
	~ScoreManager();
	void AddScore();
	void Draw();
	void ClearScore();
	int GetScore();
	inline void OffFever()
	{
		m_bFeverState = false;
	}
	inline bool GetFever()
	{
		return m_bFeverState;
	}
	inline void PlusScore(int score)
	{
		m_iSumScore += score;
	}
	bool Update(float fEtime, HEngine::RECT airplan);
};

