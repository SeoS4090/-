#pragma once
#include "GlobalDefine.h"
#include "Scene.h"
#include "BitMap.h"
#include "Button.h"
class Paper;
class FeverEffect;
class ColorPaperScene : public HEngine::Scene
{

private:
	enum PAPER_STATE
	{
		STATE_READY,
		STATE_GO,
		STATE_PAUSE,
		STATE_GAME,
		STATE_END
	};

	HEngine::BitMap *	m_pBack;
	pBoolFunc			pause;
	string				m_strScore;
	int					m_iScore;
	bool				m_bPause;
	float				m_duringtime;
	string				m_strTime;;
	PAPER_STATE			m_state;
	int					m_lasttime;
	float				m_Timeremaining;
	FeverEffect*		m_Fever;

public:
	virtual void		Init(HWND hWnd) ;
	virtual bool		Input(float fEtime) ;
	virtual void		Draw(HDC hdc) ;
	virtual void		Update(float fEtime) ;
	virtual void		Release() ;
						ColorPaperScene();
						~ColorPaperScene();
};

