#include "BitMap.h"
#include "ResourceManager.h"
#include "TimeManager.h"
namespace HEngine
{
	BitMap::BitMap()
	{
		m_eAnchorType = ANCHOR_LT;
		m_iAniCount = 0;
		m_bAnimation = FALSE;
		m_fDrawTime = 0.0f;
		m_fSpeed = 0.8f;
	}
	

	BitMap::~BitMap()
	{
		SelectObject(m_hMemDC, m_hOldBitmap);
		DeleteObject(m_hBitMap);
		DeleteDC(m_hMemDC);
	}
	void BitMap::SetAniMation(int count, bool OneTime)
	{
		m_iAniCount = count;
		m_iDrawCount = 0;
		m_bAnimation = TRUE;
		m_bOneTime = OneTime;
	}
	void BitMap::initBack(HDC hdc, int x, int y)
	{
		m_hMemDC = CreateCompatibleDC(hdc);
		m_hBitMap = CreateCompatibleBitmap(hdc, x, y);
		m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);

		m_size.cx = x;
		m_size.cy = y;
	}

	void BitMap::init(HDC hdc, string file_name)
	{
		m_BitmapName = file_name;
		m_hMemDC = CreateCompatibleDC(hdc);
		m_hBitMap = (HBITMAP)LoadImage(NULL, file_name.c_str(), IMAGE_BITMAP, 0, 0
			, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

		if (m_hBitMap == NULL)
		{
			MessageBox(NULL, file_name.c_str(), "File Not Find", MB_OK);
			return;
		}

		m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);


		BITMAP bmp_info;
		GetObject(m_hBitMap, sizeof(bmp_info), &bmp_info);
		m_size.cx = (int)bmp_info.bmWidth;
		m_size.cy = (int)bmp_info.bmHeight;
	}

	void BitMap::AdjustAnchorPoint(int& x, int& y)
	{
		x -= m_size.cx * m_ptAnchor.x;
		y -= m_size.cy * m_ptAnchor.y;
	}

	void BitMap::DrawBitblt(int x, int y)
	{
		AdjustAnchorPoint(x, y);
		BitBlt(ResourceManager::GetInstance()->GetBackDC(), x, y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, SRCCOPY);
	}

	void BitMap::Draw(int x, int y)
	{
		AdjustAnchorPoint(x, y);
		if (m_bAnimation)
		{
			m_fDrawTime += TimeManager::GetInstance()->GetElipseTime();
			if (m_fDrawTime > m_fSpeed)
			{
				m_iDrawCount++;
				m_fDrawTime -= m_fSpeed;
			}

			if (m_iAniCount <= m_iDrawCount)
			{
				if (m_bOneTime)
					m_iDrawCount = m_iAniCount - 1;
				else
					m_iDrawCount = 0;

			}

			TransparentBlt(ResourceManager::GetInstance()->GetBackDC(), x, y, m_size.cx / m_iAniCount, m_size.cy, m_hMemDC, m_size.cx / m_iAniCount * m_iDrawCount, 0, m_size.cx / m_iAniCount, m_size.cy, RGB(255, 0, 255));

		}
		else
			TransparentBlt(ResourceManager::GetInstance()->GetBackDC(), x, y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 0, 255));
	}

	void BitMap::Draw(int x, int y, float Percent)
	{
		AdjustAnchorPoint(x, y);
		if (Percent > 1.0f)
			Percent -= 1.0f;
		TransparentBlt(ResourceManager::GetInstance()->GetBackDC(), x, y, m_size.cx * Percent, m_size.cy, m_hMemDC, 0, 0, m_size.cx*Percent, m_size.cy, RGB(255, 0, 255));
	}

	void BitMap::DrawBack(HDC hdc)
	{
		BitBlt(hdc, 0, 0, m_size.cx, m_size.cy, m_hMemDC, 0, 0, SRCCOPY);

	}

	void BitMap::SetAnchor(ANCHOR type)
	{
		m_eAnchorType = type;
		switch (m_eAnchorType)
		{
		case ANCHOR_LT:
			m_ptAnchor.x = 0.0f;
			m_ptAnchor.y = 0.0f;
			break;
		case ANCHOR_TOP:
			m_ptAnchor.x = 0.5f;
			m_ptAnchor.y = 0.0f;
			break;
		case ANCHOR_RT:
			m_ptAnchor.x = 1.0f;
			m_ptAnchor.y = 0.0f;
			break;
		case ANCHOR_LEFT:
			m_ptAnchor.x = 0.0f;
			m_ptAnchor.y = 0.5f;
			break;
		case ANCHOR_CENTER:
			m_ptAnchor.x = 0.5f;
			m_ptAnchor.y = 0.5f;
			break;
		case ANCHOR_RIGHT:
			m_ptAnchor.x = 1.0f;
			m_ptAnchor.y = 0.5f;
			break;
		case ANCHOR_LB:
			m_ptAnchor.x = 0.0f;
			m_ptAnchor.y = 1.0f;
			break;
		case ANCHOR_BOTTOM:
			m_ptAnchor.x = 0.5f;
			m_ptAnchor.y = 1.0f;
			break;
		case ANCHOR_RB:
			m_ptAnchor.x = 1.0f;
			m_ptAnchor.y = 1.0f;
			break;
		}
	}

	void BitMap::SetAnchorPoint(HEngine::POINTF pt)
	{
		m_eAnchorType = ANCHOR_CUSTOM;
		m_ptAnchor = pt;
	}

	HDC BitMap::GetDC()
	{
		return m_hMemDC;
	}

	void BitMap::SetSpeed(float Speed)
	{
		m_fSpeed = Speed;
	}

	void BitMap::Draw(HEngine::POINT pt)
	{
		Draw(pt.x, pt.y);
	}

	void BitMap::Draw(HEngine::POINTF pt)
	{
		Draw(pt.x, pt.y);
	}
}