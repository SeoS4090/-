#pragma once
#include <Windows.h>
#include <vector>
#include "resource.h"
using namespace std;
enum ID_RADIO
{
	ID_EDIT00 = 100,
	ID_EDIT01,
	ID_EDIT02,
	ID_EDIT03,
	ID_EDIT04,
	ID_EDIT05,
	ID_EDIT06,
	ID_EDIT07,
	ID_EDIT08,
	ID_EDIT09,
	ID_EDIT10,
	ID_EDIT11,
	ID_EDIT12,
	ID_EDIT13,
	ID_EDIT14,
	ID_EDIT15,
	ID_EDIT16,
	ID_EDIT17,
	ID_EDIT18,
	ID_EDIT19,
	ID_EDIT20,
	ID_EDIT21,
	ID_EDIT22,
	ID_EDIT23,
	ID_EDIT24,
	ID_EDIT25,
	ID_EDIT26,
	ID_EDIT27,
	ID_EDIT28,
	ID_EDIT29,
	ID_EDIT30,
	ID_EDIT31,
	ID_EDIT32,
	ID_EDIT33,
	ID_EDIT34,
	ID_EDIT35,
	ID_EDIT36,
	ID_EDIT37,
	ID_EDIT38,
	ID_EDIT39,
	ID_EDIT40,
	ID_EDIT41,
	ID_EDIT42,
	ID_EDIT43,
	ID_EDIT44,
	ID_EDIT45,
	ID_EDIT46,
	ID_EDIT_END,
};
class BitMap;

struct INFO
{
	int bitmap_id;
	POINT bitmap_Point;
	bool Del;
};
class ToolManager
{
	static ToolManager* p_This;
	HDC BackDC;
	HBITMAP m_hBitmap;
	HBITMAP m_Oldbitmap;
	BitMap* m_MouseBitMap;
	POINT m_MousePoint;
	HWND m_hEdit[NUM_BITMAP];
	HWND m_hSave;
	HWND m_hLoad;
	vector<INFO*> m_vecinfo;
	int m_BitmapId;


	char str[128];
	OPENFILENAME OFN;

public:
	static ToolManager* GetInstance()
	{
		if (p_This == NULL)
		{
			p_This = new ToolManager();
		}
		return p_This;
	}
	void Init(HWND hWnd, HDC hdc , HINSTANCE g_hInst);
	void Draw(HDC hdc);
	void Command(HWND hWnd, WPARAM wParam);
	void Update(LPARAM lParam);
	void LBDown(LPARAM lParam);
	void RBDown(LPARAM lParam);
	ToolManager();
	~ToolManager();
};

