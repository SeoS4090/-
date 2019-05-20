#pragma once
#include <Windows.h>
#include <vector>
#include <map>
#include "SingleTone.h"
#include "BitMap.h"
using namespace std;

namespace HEngine
{
	/**
	* @brief HEngine::BitMapŬ������ �����ϴ� Ŭ����
	* @details HEngine::BitMapŬ������ �����ϴ� Ŭ���� , ���� HEngine::BitMap ��ü�� �����Ǵ� ���� �����ְ� ���Ϸε带 ����Ѵ�.
	* @author JBC classicaa@naver.com
	* @date 2016-10-14
	* @version 0.0.1
	*/

	class ResourceManager : public SingleTone<ResourceManager>
	{
	private:
		HEngine::BitMap*				m_pBack;
		map<string, HEngine::BitMap*>	m_mapBitmap;
		HEngine::BitMap*				initBitmap(std::string strFileName);
	public:
		void							initBack(HDC hdc, int cx, int cy);
		HDC								GetBackDC();
		void							DrawBack(HDC hdc);
		HEngine::BitMap*				GetBitmap(std::string strFileName);
		void							SetAnimation(std::string strFileName, int AniCount, bool OneTime);
		//void							SetAnimation(std::string strFileName, int AniCount);
		

		ResourceManager();
		~ResourceManager();
	};
}


