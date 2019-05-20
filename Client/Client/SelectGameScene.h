#pragma once
#include "GlobalDefine.h"
#include "Scene.h"
#include "EngineMain.h"
#include "BitMap.h"
#include "Button.h"
class SelectGameScene : public HEngine::Scene
{
	HEngine::BitMap * m_pBack;
	pBoolFunc bf_btBack;
	pBoolFunc bf_btColorpapaer;
	pBoolFunc bf_btAirePlane;

public:
	SelectGameScene();
	virtual ~SelectGameScene();
	virtual void Init(HWND hWnd);
	virtual bool Input(float fETime);
	virtual void Update(float fETime);
	virtual void Draw(HDC hdc);
	virtual void Release();
};

