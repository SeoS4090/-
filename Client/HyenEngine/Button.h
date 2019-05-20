#pragma once
#include "GlobalDefine.h"
#include "UIObject.h"
#include "BitMap.h"


namespace HEngine
{
	class Button : public HEngine::UIObject
	{	
	private:
		ANCHOR					m_anchor;
		POINT					m_ptDraw;
		RECT					m_rcCol;
		BitMap*					m_pDefault;
		pBoolFunc				m_pCallBack;
	public:
								Button();
		virtual					~Button();
		void					Init(int x, int y, string btnImg, pBoolFunc callback);
		virtual void			Draw();
		virtual bool			Update();
	};

}