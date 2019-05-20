#include "UIManager.h"
#include "Mecro.h"
#include "Button.h"
#include "Label.h"

UIManager* UIManager::pThis = NULL;

bool UIManager::AddLabel(HDC hdc, string s, POINT pt)
{
	Label * pNew = new Label;
	pNew->Init(hdc, s, pt.x, pt.y);
	m_vecUI.push_back(pNew);
	if (pt.y >= 530)
	{
		for (auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
		{
			if ((*iter)->GetType() == LABEL)
			{
				(*iter)->Init(hdc, (*iter)->GetString(), (*iter)->GetPosition().x, (*iter)->GetPosition().y - 30);
			}
		}
		return false;
	}
	return true;
}

void UIManager::AddButton(HDC hdc, string s, POINT pt, pBoolFunc Func, int arg)
{
	Button * pNew = new Button();
	pNew->Init(hdc , s, pt.x , pt.y);	
	pNew->SetFunc(Func, arg);
	m_vecUI.push_back(pNew);

}
bool UIManager::Update(POINT pt)
{
	for (auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
	{
		if ((*iter)->Update(pt))
			return true;
	}
	return false;
}

void UIManager::Draw(HDC hdc)
{
	for(auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
	{
		if ((*iter)->GetType() == LABEL && (*iter)->GetPosition().y < 25)
			continue;
		(*iter)->Draw(hdc);
	}
}

void UIManager::Release()
{
	for(auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
	{
		(*iter)->Release();
		SAFE_DELETE(*iter);
	}
	m_vecUI.clear();
}

void UIManager::Destroy()
{
	Release();
	SAFE_DELETE(pThis);
}
