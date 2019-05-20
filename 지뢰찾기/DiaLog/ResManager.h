#pragma once
#include <Windows.h>

class BitMap;
class ResManager
{
private:
	BitMap* m_arrImg[13];
	static ResManager* s_this;
	

public:

	static ResManager* GetInstance()
	{
		if (s_this == NULL)
		{
			s_this = new ResManager();
		}

		return s_this;
	}

	void Init(HDC hdc, HINSTANCE g_hInst);
	void Release();
	BitMap* GetImage(int index);
	ResManager();
	~ResManager();
};

