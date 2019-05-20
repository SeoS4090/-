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
	void Button::Init(int x, int y, string btnImg, pBoolFunc callback, bool Ani, int arg)
	{
		m_ptDraw.x = x;
		m_ptDraw.y = y;
		m_iarg = arg;
		m_pCallBack = callback;
		SetString(btnImg);

		m_pDefault = HEngine::ResourceManager::GetInstance()->GetBitmap(btnImg);

		m_rcCol.Set(x, y, x + m_pDefault->GetWidth(), y + m_pDefault->GetHeight());

	}

	void Button::Draw()
	{
		m_pDefault->Draw(m_ptDraw.x, m_ptDraw.y);
	}
	bool Button::Update()
	{
		if (m_rcCol.isPtin(InputManager::GetInstance()->GetMousePoint()) && GetActiveWindow() )
		{
			if (InputManager::GetInstance()->isKeyUp(VK_LBUTTON))
				return m_pCallBack(m_iarg);
		}
		return false;
	}
	UITYPE Button::GetType()
	{
		return BUTTON;
	}
}