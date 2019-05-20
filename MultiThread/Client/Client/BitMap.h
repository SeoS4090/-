#pragma once
#include <Windows.h>
#include <string>
#include "POINT.h"
using namespace std;


namespace HEngine
{
	enum ANCHOR
	{
		ANCHOR_LT,
		ANCHOR_TOP,
		ANCHOR_RT,
		ANCHOR_LEFT,
		ANCHOR_CENTER,
		ANCHOR_RIGHT,
		ANCHOR_LB,
		ANCHOR_BOTTOM,
		ANCHOR_RB,
		ANCHOR_CUSTOM
	};

	/**
	* @brief ��Ʈ�� Ŭ����
	* @details ��Ŀ �������ִ� ��Ʈ�� ���� Ŭ�����Դϴ�.
	* @author JBC classicaa@naver.com
	* @date 2016-10-14
	* @version 0.0.1
	*/

	class BitMap
	{
	private:
		string			m_BitmapName;
		HDC				m_hMemDC;
		HBITMAP			m_hBitMap;
		HBITMAP			m_hOldBitmap;
		SIZE			m_size;
		ANCHOR			m_eAnchorType;
		HEngine::POINTF	m_ptAnchor;
		int				m_iAniCount;
		int				m_iDrawCount;
		bool			m_bAnimation;
		float			m_fDrawTime;
		float			m_fSpeed;
		bool			m_bOneTime;
	public:
		void			initBack(HDC hdc, int x, int y);
		void			init(HDC hdc, string file_name);
		void			AdjustAnchorPoint(int& x, int& y);
		void			DrawBitblt(int x, int y);
		void			Draw(int x, int y);
		void			Draw(int x, int y, float Percent);
		void			Draw(HEngine::POINT pt);
		void			Draw(HEngine::POINTF pt);
		void			DrawBack(HDC hdc);
		void			SetAnchor(ANCHOR type);
		void			SetAnchorPoint(HEngine::POINTF pt);
		//void			SetAniMation(int count);
		HDC				GetDC();
		void			SetSpeed(float Speed);
		inline int		GetWidth()
		{
			return m_size.cx;
		}

		inline int		GetHeight()
		{
			return m_size.cy;
		}

		BitMap();
		~BitMap();
		void SetAniMation(int count, bool OneTime);
	};

}

