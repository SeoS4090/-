#pragma once
#include <Windows.h>
class BackGround
{
private :
	int	m_Score;
public:
	void Init();
	void Draw(HDC hdc);
	inline void SetScore(int x)
	{
		m_Score += x;
	}
	BackGround();
	~BackGround();
};

