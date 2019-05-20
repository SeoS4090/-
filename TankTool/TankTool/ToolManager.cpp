#include "ToolManager.h"
#include "ResManager.h"
#include "BitMap.h"
#include "Mecro.h"
#include <stdio.h>
#include <fstream>
using namespace std;

ToolManager * ToolManager::p_This = NULL;


void ToolManager::Init(HWND hWnd, HDC hdc, HINSTANCE g_hInst)
{
	ResManager::GetInstance()->Init(hdc , g_hInst);

	
	BackDC = CreateCompatibleDC(hdc);
	m_hBitmap = CreateCompatibleBitmap(hdc, MAP_WIDTH+20, MAP_HEIGHT + 20);
	m_Oldbitmap = (HBITMAP)SelectObject(BackDC, m_hBitmap);
	m_MousePoint = { 0,0 };

	CreateWindow("button", "TILE", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, MAP_WIDTH*2+30, 10, 460, MAP_HEIGHT*2 - 50, hWnd, (HMENU)0, g_hInst, NULL);
	m_hEdit[0] = CreateWindow("button", NULL, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, MAP_WIDTH*2 + 40,30, 20, 20, hWnd, (HMENU)(ID_EDIT00), g_hInst, NULL);
	for(int i = 1 ; i < NUM_BITMAP ; i++)
		m_hEdit[i] = CreateWindow("button", NULL, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, MAP_WIDTH*2 + 40 + (i/10)*90, 60*(i%10)+30, 20, 20, hWnd, (HMENU)(ID_EDIT00+i), g_hInst, NULL);
	CheckRadioButton(hWnd, ID_EDIT00, ID_EDIT46, NULL);

	m_hSave = CreateWindow("button", "SAVE", WS_CHILD | WS_VISIBLE | DFCS_BUTTONPUSH, MAP_WIDTH * 2 + 40, MAP_HEIGHT*2-30, 100, 30, hWnd, (HMENU)0, g_hInst, NULL);
	m_hLoad = CreateWindow("button", "Load", WS_CHILD | WS_VISIBLE | DFCS_BUTTONPUSH, MAP_WIDTH * 2 + 40 + 150, MAP_HEIGHT * 2 - 30, 100, 30, hWnd, (HMENU)1, g_hInst, NULL);

	m_MouseBitMap = new BitMap();
	m_BitmapId = -1;


}

void ToolManager::Draw(HDC hdc)
{
	ResManager::GetInstance()->GetImg(BACKGROUND)->Draw(BackDC, 0, 0);

	HBRUSH hbrush, obrush;
	hbrush = CreateSolidBrush(RGB(0, 0, 0));
	obrush = (HBRUSH)SelectObject(BackDC, hbrush);


	for (int i = 0; i < MAP_WIDTH / 33; i++)
	{
		for (int j = 0; j < MAP_HEIGHT/25; j++)
		{
			Rectangle(BackDC, 10 + i*33, 10 + j*25, 10 + i * 33+ 33, 10 + j * 25+ 25);
		}
	}
	SelectObject(hdc, obrush);
	DeleteObject(hbrush);
	for (auto iter = m_vecinfo.begin(); iter != m_vecinfo.end(); iter++)
	{
		if(!(*iter)->Del)
			ResManager::GetInstance()->GetImg((*iter)->bitmap_id)->Draw(BackDC, (*iter)->bitmap_Point.x, (*iter)->bitmap_Point.y);
	}
	m_MouseBitMap->Draw(BackDC, m_MousePoint.x, m_MousePoint.y);
	

	for (int i = 0; i < INFO_END; i++)
	{
		if (i == BACKGROUND)
			continue;

		ResManager::GetInstance()->GetImg(i)->Draw(hdc, MAP_WIDTH * 2 + 40 + (i / 10) * 90 + 20, 60 * (i % 10) + 30);
	}


	//BitBlt(hdc, 0, 0, MAP_WIDTH * 2 + 20 + 300, MAP_HEIGHT * 2 + 20, BackDC, 0, 0, SRCCOPY);
	TransparentBlt(hdc, 0, 0, MAP_WIDTH*2+ 20, MAP_HEIGHT*2 + 20, BackDC, 0, 0, MAP_WIDTH + 20, MAP_HEIGHT + 20, SRCCOPY);
	
}

void ToolManager::Command(HWND hWnd, WPARAM wParam)
{
	if (LOWORD(wParam) == 0) //save
	{
		char filename[256];
		OPENFILENAME OFN;
		char lpstrfile[256] = "";
		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = hWnd;
		OFN.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt\0";
		OFN.lpstrFile = lpstrfile;
		OFN.nMaxFile = 256;
		OFN.lpstrInitialDir = "c:\\";
		if (GetSaveFileName(&OFN) != 0) {
			
			ofstream outFile(OFN.lpstrFile);
			for (auto iter = m_vecinfo.begin(); iter != m_vecinfo.end(); iter++)
			{
				if (!(*iter)->Del)
					outFile << (*iter)->bitmap_id << endl << (*iter)->bitmap_Point.x << endl << (*iter)->bitmap_Point.y << endl;

			}
			outFile.close();
			sprintf_s(filename, "%s 파일을 저장하였습니다..", OFN.lpstrFile);
			MessageBox(hWnd, filename, "FILE SAVE", MB_OK);
		}

	}
	else if(LOWORD(wParam) == 1) //load
	{

		char filename[128];
		OPENFILENAME OFN;
		char lpstrfile[256] = "";
		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = hWnd;
		OFN.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt\0";
		OFN.lpstrFile = lpstrfile;
		OFN.nMaxFile = 256;
		OFN.lpstrInitialDir = "c:\\";
		if (GetOpenFileName(&OFN) != 0) {
			for (auto iter = m_vecinfo.begin(); iter != m_vecinfo.end(); iter++)
			{
				SAFE_DELETE((*iter));
			}
			m_vecinfo.clear();

			
			ifstream inFile(OFN.lpstrFile);
			while (!inFile.eof())
			{
				char str[100];
				inFile.getline(str, 100);

				INFO * loadinfo = new INFO();

				loadinfo->bitmap_id = atoi(str);
				inFile.getline(str, 100);
				loadinfo->bitmap_Point.x = atoi(str);
				inFile.getline(str, 100);
				loadinfo->bitmap_Point.y = atoi(str);
				loadinfo->Del = false;

				if (loadinfo->bitmap_id == 0 && loadinfo->bitmap_Point.x == 0 && loadinfo->bitmap_Point.y == 0)
					break;


				m_vecinfo.push_back(loadinfo);
			}

			inFile.close();
			sprintf_s(filename, "%s 파일을 열었습니다.", OFN.lpstrFile);
			MessageBox(hWnd, filename, "FILE OPEN", MB_OK);
		}
		
		RECT rt = { 0,0,MAP_WIDTH * 2 + 20, MAP_HEIGHT * 2 + 20 };
		InvalidateRect(hWnd, &rt, FALSE);
	}
	else
	{
		m_BitmapId = LOWORD(wParam) - 100;
		m_MouseBitMap = ResManager::GetInstance()->GetImg(m_BitmapId);
	}
	
}

void ToolManager::Update(LPARAM lParam)
{
	if (LOWORD(lParam) < 10 + MAP_WIDTH*2 - 33)
		m_MousePoint.x = LOWORD(lParam)/2 - BITMAP_XSIZE / 2;
	if(HIWORD(lParam) < MAP_HEIGHT * 2 - 25 + 10)
		m_MousePoint.y = HIWORD(lParam) / 2 - BITMAP_YSIZE / 2;

	m_MousePoint.x = (int)(m_MousePoint.x / 33) * 33 + 10;
	m_MousePoint.y = (int)(m_MousePoint.y / 25) * 25 + 10;

}

void ToolManager::LBDown(LPARAM lParam)
{
	if (m_BitmapId < 0)
		return;

	if (LOWORD(lParam) > MAP_WIDTH * 2 + 20 || HIWORD(lParam) > MAP_HEIGHT * 2 + 20)
		return;


	for(auto iter = m_vecinfo.begin(); iter != m_vecinfo.end(); iter++)
	{
		if ((*iter)->bitmap_Point.x == (int)(m_MousePoint.x / 33) * 33 + 10 && (*iter)->bitmap_Point.y == (int)(m_MousePoint.y / 25) * 25 + 10 && !(*iter)->Del)
			return;
	}
	INFO* info = new INFO();

	info->bitmap_Point.x = (int)(m_MousePoint.x / 33) * 33 + 10;
	info->bitmap_Point.y = (int)(m_MousePoint.y / 25) * 25 + 10;
	info->bitmap_id = m_BitmapId;
	info->Del = false;

	m_vecinfo.push_back(info);
}

void ToolManager::RBDown(LPARAM lParam)
{
	for (auto iter = m_vecinfo.begin(); iter != m_vecinfo.end(); iter++)
	{
		if ((*iter)->bitmap_Point.x == (int)(m_MousePoint.x / 33) * 33 + 10 && (*iter)->bitmap_Point.y == (int)(m_MousePoint.y / 25) * 25 + 10)
			(*iter)->Del = true ;
	}
}

ToolManager::ToolManager()
{
}


ToolManager::~ToolManager()
{
}
