#pragma once
#include "GlobalDefine.h"
#include "Scene.h"
#include "EngineMain.h"
#include "BitMap.h"
#include "SingleTone.h"
#include "Defines.h"

class AirPlan;
class ObjectManager;
class FeverEffect;
class AirPlanScene : public HEngine::Scene
{
	enum AIRPLAN_STATE
	{
		STATE_READY,
		STATE_GO,
		STATE_PAUSE,
		STATE_GAME,
		STATE_END
	};
private:
	string				m_strScore;
	int					m_iStarScore;
	pBoolFunc			bf_btpause;
	HEngine::BitMap *	m_pBack;
	HEngine::POINTF		pt_plan;
	AirPlan*			m_player;
	bool				m_bPause;
	float				m_duringtime;
	string				m_strTime;;
	AIRPLAN_STATE		m_state;
	int					m_lasttime;
	float				m_Timeremaining;
	FeverEffect*		m_Fever;
public:
	AirPlanScene();
	~AirPlanScene();

	virtual void Init(HWND hWnd);
	virtual bool Input(float fEtime);
	virtual void Draw(HDC hdc);
	virtual void Update(float fEtime);
	virtual void Release();
};

