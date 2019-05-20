#pragma once
#include "UIObject.h"
namespace HEngine
{
	class Label : public HEngine::UIObject
	{
	private:
		DWORD					m_dwDrawOption;
		string					m_strPrint;
		bool					m_bBitMap;
		HEngine::POINT			m_pt;
		int						m_iStrlength;

	public:
		Label();
		void					Init(string str, int x, int y, DWORD DrawOption, bool m_bBmp);
		int						GetStringLength();
		virtual void			Draw();
		virtual bool			Update();
		virtual UITYPE			GetType();
		virtual ~Label();
	};
}
