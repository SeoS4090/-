#include "Object.h"

bool Object::Input(int key)
{
	return true;
}

bool Object::GetFade()
{
	return m_Object->m_bFade;
}

void Object::Release()
{
	SAFE_DELETE(m_Object);
}

void Object::SetScore(int score)
{
	m_Object->score = score;
}

HEngine::POINTF Object::GetPosition()
{
	return m_Object->pt;
}

int Object::GetScore()
{
	return m_Object->score;
}
Object::Object()
{
	m_Object = new Object_Info();
	srand(GetTickCount());
	int m_rand = rand() % 4;

	if (m_rand == 0 || m_rand == 1)
	{
		m_Object->pt.x = rand() % (GAME_WIDTH);
		if (m_rand == 0)
		{
			m_Object->pt.y = GAME_HEIGHT_START;
			while (true)
			{
				m_Object->direct = DIRECT(rand() % 8);
				if (m_Object->direct != UP && m_Object->direct != UPLEFT && m_Object->direct != UPRIGHT && m_Object->direct != LEFT && m_Object->direct != RIGHT)
					break;
			}

		}
		else
		{
			m_Object->pt.y = GAME_HEIGHT_END;
			while (true)
			{
				m_Object->direct = DIRECT(rand() % 8);
				if (m_Object->direct != DOWN && m_Object->direct != DOWNLEFT && m_Object->direct != DOWNRIGHT && m_Object->direct != LEFT && m_Object->direct != RIGHT)
					break;
			}
		}
	}
	else
	{
		m_Object->pt.y = (rand() % (GAME_HEIGHT_END)) + GAME_HEIGHT_START;

		if (m_rand == 2)
		{
			m_Object->pt.x = 1;

			while (true)
			{
				m_Object->direct = DIRECT(rand() % 8);
				if (m_Object->direct != LEFT && m_Object->direct != UPLEFT && m_Object->direct != DOWNLEFT && m_Object->direct != UP && m_Object->direct != DOWN)
					break;
			}
		}
		else
		{
			m_Object->pt.x = GAME_WIDTH;
			while (true)
			{
				m_Object->direct = DIRECT(rand() % 8);
				if (m_Object->direct != RIGHT && m_Object->direct != UPRIGHT && m_Object->direct != DOWNRIGHT && m_Object->direct != UP && m_Object->direct != DOWN)
					break;
			}
		}
	}

	m_Object->m_bFade = false;
	m_Object->score = 0;
}


Object::~Object()
{
}
