#pragma once
#include "Object.h"

class Bullet : public Object
{
private:
public:
	virtual void Draw();
	virtual bool Update(float fEtime, HEngine::RECT rect);
	virtual OBJECT_TYPE GetType();
	
	Bullet();
	~Bullet();
};

