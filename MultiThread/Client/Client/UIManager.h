#pragma once
#include "GlobalDefine.h"
#include "SingleTone.h"
#include "UIObject.h"

namespace HEngine
{
	class UIManager :public SingleTone<UIManager>
	{
	private:
		vector<HEngine::UIObject*> m_vecUI;
	public:
		UIManager();
		~UIManager();
		void AddButton(int x, int y, string btimg, pBoolFunc callback, int arg, bool Ani);
		bool AddLabel(string str, int x, int y , DWORD DrawOption, bool bBmp);
		void Update();
		void Draw();
		void ReleaseUI();
		void ReleaseButton(string bitstring);
	};

}