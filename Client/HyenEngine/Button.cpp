#include "Button.h"
#include "ResourceManager.h"
#include "InputManager.h"

namespace HEngine
{
	Button::Button()
	{
		m_anchor = ANCHOR_LT;
	}


	Button::~Button()
	{
	}
	void Button::Init(int x, int y, string btnImg, pBoolFunc callback)
	{
		m_ptDraw.x = x;
		m_ptDraw.y = y;

		m_pCallBack = callback;
		m_pDefault = HEngine::ResourceManager::GetInstance()->GetBitmap(btnImg);
		m_rcCol.Set(x, y, x + m_pDefault->GetWidth(), y + m_pDefault->GetHeight());

	}
	void Button::Draw()
	{
		m_pDefault->Draw(m_ptDraw);
	}
	bool Button::Update()
	{
		if (InputManager::GetInstance()->isKeyUp(VK_LBUTTON))
		{
			if (m_rcCol.isPtin(InputManager::GetInstance()->GetMousePoint()))
				return m_pCallBack();
		}

		return false;
	}
}