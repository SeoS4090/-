#include "Star.h"
#include "ResourceManager.h"
#include "UIManager.h"
#include "Label.h"

void Star::Draw()
{
	if (!m_Object->m_bFade)
	{
		HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->Draw(m_Object->pt);
		if(m_Object->pt.y > GAME_HEIGHT_START - 25 && m_Object->pt.x > -25)
			m_laScore->Draw();
	}

}

bool Star::Update(float fEtime, HEngine::RECT rect)
{
	char buf[16];
	string strscore;
	sprintf(buf, "%d", m_Object->score);
	strscore = buf;

	if (!m_Object->m_bFade && m_Object->pt.y + 25 < GAME_HEIGHT_END && m_Object->pt.y + 25 > GAME_HEIGHT_START &&m_Object->pt.x + 20 > 0 && m_Object->pt.x + 20 < GAME_WIDTH)
	{
		m_laScore->Init(strscore, m_Object->pt.x + 20, m_Object->pt.y + 25, DT_CENTER | DT_WORDBREAK,false);
	}

	if(!m_bCanMove)
		return false;

	if (m_Object->direct == LEFT || m_Object->direct == UPLEFT || m_Object->direct == DOWNLEFT)
		m_Object->pt.x -= 70 * fEtime;
	else if (m_Object->direct == RIGHT || m_Object->direct == UPRIGHT || m_Object->direct == DOWNRIGHT)
		m_Object->pt.x += 70 * fEtime;

	if (m_Object->direct == UP || m_Object->direct == UPLEFT || m_Object->direct == UPRIGHT)
		m_Object->pt.y -= 70 * fEtime;
	else if (m_Object->direct == DOWN || m_Object->direct == DOWNLEFT || m_Object->direct == DOWNRIGHT)
		m_Object->pt.y += 70 * fEtime;

	if (m_Object->pt.x < 0 - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/score.bmp")->GetWidth() || m_Object->pt.x > GAME_WIDTH || m_Object->pt.y < GAME_HEIGHT_START - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/score.bmp")->GetHeight() || m_Object->pt.y > GAME_HEIGHT_END)
	{
		m_Object->m_bFade = true;
	}

	HEngine::RECT rt;
	rt.left = m_Object->pt.x;
	rt.top = m_Object->pt.y;
	rt.right = m_Object->pt.x + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/score.bmp")->GetWidth();
	rt.bottom = m_Object->pt.y + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/score.bmp")->GetHeight();
	if (rt.isCollision(rect) && !m_Object->m_bFade)
	{
		m_Object->m_bFade = true;
		return true;
	}

	return false;
}

OBJECT_TYPE Star::GetType()
{
	return OBJECT_STAR;
}

void Star::SetCanMove()
{
	m_bCanMove = true;
}

Star::Star()
{
	m_bCanMove = false;
	m_Object->score = 100;
	m_laScore = new HEngine::Label();
}


Star::~Star()
{
	SAFE_DELETE(m_laScore);
}
