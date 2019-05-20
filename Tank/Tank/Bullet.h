#pragma once
#include <Windows.h>
#include "resource.h"
class Bullet
{
	POINT m_Position;
	INFO_BITMAP m_iDirect;
	DWORD		m_dwLastTime;
	DWORD		m_dwCurTime;
	float		m_fElapseTime;
	bool m_Fade;
	float m_ExplosionCount;
public:
	void Init(int x, int y, INFO_BITMAP Direct);
	void Update();
	void Draw(HDC hdc);
	void SetFade();
	bool GetFade()
	{
		return m_Fade;
	}
	bool CheckCollider(RECT rect);
	INFO_BITMAP GetDirect()
	{
		return m_iDirect;
	}
	Bullet();
	~Bullet();
};

