#pragma once
#include <Windows.h>
class Barrier
{
	POINT m_Position;

public:
	void Init(int x);
	void Draw(HDC hdc);
	RECT GetRect();
	Barrier();
	~Barrier();
};

