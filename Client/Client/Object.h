#pragma once
#include "GlobalDefine.h"
#include "POINT.h"
#include "Defines.h"
class Object
{
protected:

	struct Object_Info
	{
		HEngine::POINTF pt;
		DIRECT direct;
		bool m_bFade;
		int score;
	};

	Object_Info * m_Object;
	
public:
	virtual void Draw() = 0;
	virtual bool Update(float fEtime, HEngine::RECT rect = {0,0,0,0}) = 0;
	virtual OBJECT_TYPE GetType() = 0;
	virtual bool Input(int key);
	bool GetFade();
	void Release();
	void SetScore(int score);
	virtual HEngine::POINTF GetPosition();
	int GetScore();
	void SetFade(bool fade)
	{
		m_Object->m_bFade = fade;
	}
	void SetPosition(HEngine::POINTF pt)
	{
		m_Object->pt = pt;
	}
	Object();
	~Object();
};

