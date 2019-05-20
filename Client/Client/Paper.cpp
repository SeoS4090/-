#include "Paper.h"
#include "ResourceManager.h"


void Paper::Draw()
{
	switch (m_color)
	{
	case RED:
		HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/PaperGame/red.bmp")->Draw(m_MovePT);
		break;
	case GREEN:
		HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/PaperGame/green.bmp")->Draw(m_MovePT);
		break;
	case BLUE:
		HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/PaperGame/blue.bmp")->Draw(m_MovePT);
		break;
	case YELLOW:
		HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/PaperGame/yellow.bmp")->Draw(m_MovePT);
		break;
	}
}

bool Paper::Update(float fEtime, HEngine::RECT rect)
{
	
	if (m_MovePT.x > m_Object->pt.x)
	{
		m_MovePT.x -= fEtime*450;
		if (m_MovePT.x < m_Object->pt.x)
		{
			m_MovePT.x = m_Object->pt.x;
		}
	}
	if (m_MovePT.x < m_Object->pt.x)
	{
		m_MovePT.x += fEtime * 450;
		if (m_MovePT.x > m_Object->pt.x)
		{
			m_MovePT.x = m_Object->pt.x;
		}
	}
	if (m_MovePT.y < m_Object->pt.y)
	{
		m_MovePT.y += fEtime * 450;
		if (m_MovePT.y > m_Object->pt.y)
		{
			m_MovePT.y = m_Object->pt.y;
		}
	}
	if (m_MovePT.y > m_Object->pt.y)
	{
		m_MovePT.y -= fEtime * 450;
		if (m_MovePT.y < m_Object->pt.y)
		{
			m_MovePT.y = m_Object->pt.y;
		}
	}
	
	if (m_Object->pt.x != paper_position.Paper_CENTER.x || m_Object->pt.y != paper_position.Paper_CENTER.y)
	{
		if (m_color == RED)
		{
			if (m_Object->pt.x != paper_position.Paper_RIGHT.x || m_Object->pt.y != paper_position.Paper_RIGHT.y)
			{
				m_Object->pt = paper_position.Paper_CENTER;
				return false;
			}
		}
		if (m_color == GREEN)
		{
			if (m_Object->pt.x != paper_position.Paper_UP.x || m_Object->pt.y != paper_position.Paper_UP.y)
			{
				m_Object->pt = paper_position.Paper_CENTER;
				return false;
			}
		}
		if (m_color == BLUE)
		{
			if (m_Object->pt.x != paper_position.Paper_LEFT.x || m_Object->pt.y != paper_position.Paper_LEFT.y)
			{
				m_Object->pt = paper_position.Paper_CENTER;
				return false;
			}
		}
		if (m_color == YELLOW)
		{
			if (m_Object->pt.x != paper_position.Paper_DOWN.x || m_Object->pt.y != paper_position.Paper_DOWN.y)
			{
				m_Object->pt = paper_position.Paper_CENTER;
				return false;
			}
		}

		if (m_MovePT.x == paper_position.Paper_DOWN.x && m_MovePT.y == paper_position.Paper_DOWN.y)
			return true;
		if (m_MovePT.x == paper_position.Paper_LEFT.x && m_MovePT.y == paper_position.Paper_LEFT.y)
			return true;
		if (m_MovePT.x == paper_position.Paper_UP.x && m_MovePT.y == paper_position.Paper_UP.y)
			return true;
		if (m_MovePT.x == paper_position.Paper_RIGHT.x && m_MovePT.y == paper_position.Paper_RIGHT.y)
			return true;
	}

	return false;
}

OBJECT_TYPE Paper::GetType()
{
	return OBJECT_PAPER;
}

bool Paper::Input(int key)
{
	switch (key)
	{
	case VK_UP:
		m_Object->pt = paper_position.Paper_UP;
		if (m_color != GREEN)
		{
			m_MovePT.x -= 10;
			return false;
		}
		break;
	case VK_DOWN:
		m_Object->pt = paper_position.Paper_DOWN;
		if (m_color != YELLOW)
		{
			m_MovePT.x -= 10;
			return false;
		}
		break;
	case VK_RIGHT:
		m_Object->pt = paper_position.Paper_RIGHT;
		if (m_color != RED)
		{
			m_MovePT.x -= 10;
			return false;
		}
		break;
	case VK_LEFT:
		m_Object->pt = paper_position.Paper_LEFT;
		if (m_color != BLUE)
		{
			m_MovePT.x -= 10;
			return false;
		}
		break;
	}
}

Paper::Paper()
{
	m_color = (PAPER)rand() % 4;
	m_Object->pt = paper_position.Paper_CENTER;
	m_MovePT = paper_position.Paper_CENTER;
}


Paper::~Paper()
{
}
