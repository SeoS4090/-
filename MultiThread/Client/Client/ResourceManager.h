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
	* @brief HEngine::BitMap클레스를 관리하는 클레스
	* @details HEngine::BitMap클레스를 관리하는 클레스 , 같은 HEngine::BitMap 객체가 생성되는 것을 막아주고 파일로드를 담당한다.
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


