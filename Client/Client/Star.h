#pragma once
#include "Object.h"
#include "Label.h"

class Star : public Object
{
private:
	bool m_bCanMove;
	HEngine::Label* m_laScore;
public:
	virtual void Draw();
	virtual bool Update(float fEtime, HEngine::RECT rect);
	virtual OBJECT_TYPE GetType();
	void SetCanMove();
	Star();
	~Star();
};

