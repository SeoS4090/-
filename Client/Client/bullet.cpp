#include "Bullet.h"
#include "ResourceManager.h"


void Bullet::Draw()
{
	if (!m_Object->m_bFade)
		HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/bullet.bmp")->Draw(m_Object->pt);
}

bool Bullet::Update(float fEtime, HEngine::RECT rect)
{
	if (m_Object->direct == LEFT || m_Object->direct == UPLEFT || m_Object->direct == DOWNLEFT)
		m_Object->pt.x -= 50 * fEtime;
	else if (m_Object->direct == RIGHT || m_Object->direct == UPRIGHT || m_Object->direct == DOWNRIGHT)
		m_Object->pt.x += 50 * fEtime;

	if (m_Object->direct == UP || m_Object->direct == UPLEFT || m_Object->direct == UPRIGHT)
		m_Object->pt.y -= 50 * fEtime;
	else if (m_Object->direct == DOWN || m_Object->direct == DOWNLEFT || m_Object->direct == DOWNRIGHT)
		m_Object->pt.y += 50 * fEtime;

	if (m_Object->pt.x < 0 - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/bullet.bmp")->GetWidth() || m_Object->pt.x > GAME_WIDTH || m_Object->pt.y < GAME_HEIGHT_START - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/bullet.bmp")->GetHeight() || m_Object->pt.y > GAME_HEIGHT_END)
	{
		m_Object->m_bFade = true;
	}

	HEngine::RECT rt;
	rt.left = m_Object->pt.x;
	rt.top = m_Object->pt.y;
	rt.right = m_Object->pt.x + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/bullet.bmp")->GetWidth();
	rt.bottom = m_Object->pt.y + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/bullet.bmp")->GetHeight();
	if (rt.isCollision(rect) && !m_Object->m_bFade)
	{
		return true;
	}

	return false;
}

OBJECT_TYPE Bullet::GetType()
{
	return OBJECT_BULLET;
}



Bullet::Bullet()
{
	
}


Bullet::~Bullet()
{
	
}
