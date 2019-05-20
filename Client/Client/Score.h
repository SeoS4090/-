#pragma once
#include "Defines.h"
#include "POINT.h"
#include "GlobalDefine.h"
class Score
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

	vector<SCORE_Info> m_vecScore;
public:
	Score();
	~Score();
	void AddScore();
	void Draw();
	void ClearScore();
	bool Update(float fEtime, HEngine::RECT airplan);
};

