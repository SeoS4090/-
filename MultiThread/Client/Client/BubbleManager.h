#pragma once
#include "SingleTone.h"
#include "GlobalDefine.h"
#include "POINT.h"
#include "ResourceManager.h"

struct BUBBLESTRUCT
{
	float fEtime;
	bool bUpdate;
	bool bPlayer;
	HEngine::POINT pt;
	HEngine::BitMap* bitBubble;
	int Power;
};
class BubbleManager : public SingleTone<BubbleManager>
{
private:
	int temp;
	int						m_iBubbleCount;
	int						m_maxBubbleCount;
	vector <BUBBLESTRUCT * > m_vecBubble;
public:	
	//void AddBubble(HEngine::POINT pt);
	//bool AddBubble(HEngine::POINT pt, int Power);
	bool AddBubble(HEngine::POINT pt, int Power, bool Player);
	void PlusBubbleCount();
	int  GetBubbleCount();

	bool Update();
	void Draw();
	void Release();
	BubbleManager();
	~BubbleManager();
};

