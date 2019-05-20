#pragma once
#include "Defines.h"
#include "POINT.h"
#include "SingleTone.h"
#include "Paper.h"
class Object;
class ObjectManager : public SingleTone<ObjectManager>
{
private:
	int m_score;
	int m_icombo;
	vector<Object * > m_vecObject;
public:
	bool AddBullet();
	bool AddStar(bool Fever);
	bool AddPaper(bool Fever);
	ColliderType Update(float fEtime, HEngine::RECT rect);
	bool Input(int key);
	void Draw();
	void ReleaseObject();
	int GetScore();
	ObjectManager();
	~ObjectManager();
};

