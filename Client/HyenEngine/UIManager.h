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
		void AddButton(int x, int y , string btimg, std::function<bool()> callback);
		void AddLabel(string str, int x, int y , DWORD DrawOption, bool bBmp);
		void Update();
		void Draw();
		void ReleaseUI();
	};

}