#pragma once
#include "Object.h"
#include "Defines.h"

class Paper : public Object
{
private:
	int m_color;
	HEngine::POINTF m_MovePT;
	Paper_Position paper_position;
public:
	virtual void Draw();
	virtual bool Update(float fEtime, HEngine::RECT rect);
	virtual OBJECT_TYPE GetType();
	virtual bool Input(int key);
	virtual HEngine::POINTF GetPosition()
	{
		return m_MovePT;
	}
	Paper();
	~Paper();
};

