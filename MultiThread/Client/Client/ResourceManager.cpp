#include "ResourceManager.h"
#include "GlobalDefine.h"

namespace HEngine
{
	ResourceManager::ResourceManager()
	{
	}


	ResourceManager::~ResourceManager()
	{
		SAFE_DELETE(m_pBack);

		for (auto iter = m_mapBitmap.begin(); iter != m_mapBitmap.end(); iter++)
		{
			auto del = (*iter).second;
			SAFE_DELETE(del);
		}
	}

	void ResourceManager::initBack(HDC hdc, int cx, int cy)
	{
		m_pBack = new HEngine::BitMap();
		m_pBack->initBack(hdc, cx, cy);
	}


	HEngine::BitMap* ResourceManager::initBitmap(std::string strFileName)
	{
		auto iter = m_mapBitmap.find(strFileName);
		if (iter == m_mapBitmap.end())
		{
			HEngine::BitMap* pBitmap = new HEngine::BitMap();
			pBitmap->init(m_pBack->GetDC(), strFileName);

			m_mapBitmap.insert(make_pair(strFileName, pBitmap));
			return pBitmap;
		}
		else
			return iter->second;

	}

	HDC ResourceManager::GetBackDC()
	{
		return m_pBack->GetDC();
	}

	void ResourceManager::DrawBack(HDC hdc)
	{
		m_pBack->DrawBack(hdc);
	}

	HEngine::BitMap* ResourceManager::GetBitmap(std::string strFileName)
	{
		auto iter = m_mapBitmap.find(strFileName);
		if (iter != m_mapBitmap.end())
		{
			return iter->second;
		}
		else
		{
			return initBitmap(strFileName);
		}
	}
	void ResourceManager::SetAnimation(std::string strFileName , int AniCount, bool OneTime)
	{
		GetBitmap(strFileName)->SetAniMation(AniCount, OneTime);
	}
}

