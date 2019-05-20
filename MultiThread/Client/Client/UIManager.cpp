#include "UIManager.h"
#include "ResourceManager.h"
#include "Label.h"
#include "button.h"

namespace HEngine
{
	UIManager::UIManager()
	{
		SetBkMode(HEngine::ResourceManager::GetInstance()->GetBackDC(), TRANSPARENT);
	}


	UIManager::~UIManager()
	{
		for (auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
		{
			auto del = (*iter);
			SAFE_DELETE(del);
		}
	}
	void UIManager::AddButton(int x, int y, string btimg, pBoolFunc callback, int arg, bool Ani)
	{
		HEngine::Button* bt = new HEngine::Button();
		bt->Init(x, y, btimg,callback,Ani,arg);
		m_vecUI.push_back(bt);
	}

	bool UIManager::AddLabel(string str, int x, int y, DWORD drawOption, bool bBmp)
	{
		HEngine::Label *pNew = new HEngine::Label();
		pNew->Init(str, x, y, drawOption, bBmp);
		m_vecUI.push_back(pNew);
		if (y >= 490)
		{
			vector<UIObject * >::iterator change = m_vecUI.begin();
			for (auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
			{
				if ((*iter)->GetType() == LABEL)
				{
					if (change != iter)
					{
						(*change)->SetString((*iter)->GetString());
						change = iter;
					}
				}
			}
			UIObject * pdel = m_vecUI.back();
			SAFE_DELETE( pdel);
			m_vecUI.pop_back();
			return false;
		}
		return true;
	}

	void UIManager::Update()
	{
		for (auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
		{
			if ((*iter)->Update())
				return;
		}
	}
	void UIManager::Draw()
	{
		for (auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
		{
			(*iter)->Draw();
		}
	}
	void UIManager::ReleaseUI()
	{
		for (auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
		{
			SAFE_DELETE(*iter);
		}
		m_vecUI.clear();
	}
	void UIManager::ReleaseButton(string bitstring)
	{
		for (auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
		{
			if ((*iter)->GetString() == bitstring)
			{
				SAFE_DELETE(*iter);
				m_vecUI.erase(iter);
				break;	
			}
		}
	}
}