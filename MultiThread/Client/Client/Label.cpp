#include "Label.h"
#include "ResourceManager.h"

namespace HEngine
{
	Label::Label()
	{
	}

	void Label::Init(string str, int x, int y, DWORD DrawOption , bool m_bBmp)
	{
		SetString(str);
		m_dwDrawOption = DrawOption;
		::SIZE size;
		GetTextExtentPoint32(HEngine::ResourceManager::GetInstance()->GetBackDC(), GetString().c_str(), GetString().length(), &size);
		SetPos(x, y, x + size.cx, y + size.cy);
		m_bBitMap = m_bBmp;
		m_pt.x = x;
		m_pt.y = y;
		m_iStrlength = str.length();
		if (m_bBmp)
		{
			m_iStrlength = str.length() * 33;
		}
	}

	int Label::GetStringLength()
	{
		return m_iStrlength;
	}

	void Label::Draw()
	{
		if(!m_bBitMap)
			DrawText(HEngine::ResourceManager::GetInstance()->GetBackDC(),  GetString().c_str(),  GetString().length(),&m_RecPos.ToWinRECT(), m_dwDrawOption);
		else
		{
			char m_arrbonus[16];
			strcpy(m_arrbonus,  GetString().c_str());

			for (int i = 0; i <  GetString().size(); i++)
			{
				switch (m_arrbonus[i])
				{
				case '1':
					HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/1.bmp")->Draw(m_pt.x +  33* i , m_pt.y);
					break;
				case '2':
					HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/2.bmp")->Draw(m_pt.x +  33* i, m_pt.y);
					break;
				case '3':
					HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/3.bmp")->Draw(m_pt.x +  33* i, m_pt.y);
					break;
				case '4':
					HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/4.bmp")->Draw(m_pt.x +  33* i, m_pt.y);
					break;
				case '5':
					HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/5.bmp")->Draw(m_pt.x +  33* i, m_pt.y);
					break;
				case '6':
					HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/6.bmp")->Draw(m_pt.x +  33* i, m_pt.y);
					break;
				case '7':
					HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/7.bmp")->Draw(m_pt.x +  33* i, m_pt.y);
					break;
				case '8':
					HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/8.bmp")->Draw(m_pt.x +  33* i, m_pt.y);
					break;
				case '9':
					HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/9.bmp")->Draw(m_pt.x +  33* i, m_pt.y);
					break;
				case '0':
					HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/0.bmp")->Draw(m_pt.x + 33 * i, m_pt.y);
					break;
				case '.':
					HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/,.bmp")->Draw(m_pt.x + 33 * i, m_pt.y + 30);
					break;
				}
			}
		}

	}

	bool Label::Update()
	{

		return false;
	}

	UITYPE Label::GetType()
	{
		return LABEL;
	}


	Label::~Label()
	{
	}
}