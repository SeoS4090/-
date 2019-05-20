#pragma once
#include <Windows.h>
#include <vector>
#include "resource.h"
using namespace std;
class Tank
{
private:
	POINT m_Position;
	POINT m_PrevPosition;
	POINT m_FirstPosition;
	INFO_BITMAP m_id;
	DWORD		m_dwLastTime;
	DWORD		m_dwCurTime;
	float		m_fElapseTime;
	bool m_Die;
	int m_AIDirect;
	int m_iLife;
	float m_fDuringTime;
	bool m_bBarrier;
	bool m_bRespone;
public:
	void Init(INFO_BITMAP id, int x, int y);
	void Update();
	void TankMove(vector <INFO*> vecMap);
	void Draw(HDC hdc);
	void AIUpdate(vector<INFO*> vecMap);
	bool Resurrection();
	void SetBarrier(bool barrier)
	{
		m_bBarrier = barrier;
	}
	void BackPosition()
	{
		m_Position.x = m_PrevPosition.x;
		m_Position.y = m_PrevPosition.y;
	}
	void ChangeDirect();
	bool GetBarrier()
	{
		return m_bBarrier;
	}
	POINT GetPosition()
	{
		return m_Position;
	}
	bool GetRespone()
	{
		return m_bRespone;
	}
	void SetDie()
	{
		m_fDuringTime = 0;
		m_iLife--;
		m_Die = true;
	}
	int GetLife()
	{
		return m_iLife;
	}
	bool GetDie()
	{
		return m_Die;
	}
	RECT GetRect()
	{
		RECT rt;
		rt.left = m_Position.x + 1;
		rt.top = m_Position.y + 1;
		rt.right = rt.left + 31;
		rt.bottom = rt.top + 22;

		return rt;
	}
	INFO_BITMAP GetDirect()
	{
		return m_id;
	}
	Tank();
	~Tank();
};

