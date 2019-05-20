#pragma once
#include "GlobalDefine.h"
#include "EngineMain.h"
#include "BitMap.h"
#include "Scene.h"


class TitleScene : public HEngine::Scene
{
	HEngine::BitMap*	m_pBack;
	HWND m_hWnd;
public:
	virtual void Init(HWND hWnd);
	virtual bool Input(float fETime);
	virtual void Update(float fETime);
	virtual void Draw(HDC hdc);
	virtual void Release();

	TitleScene();
	virtual ~TitleScene();	
};

