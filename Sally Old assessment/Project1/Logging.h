//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		Logging.h
// Description :	Logging declariation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#pragma once

#include <iostream>
#include <fstream>

using namespace std;

class CLogging
{
public:
	static CLogging* GetInstance();
	
	void AddStatus(std::string _AddToFile);

	void DeleteInstance();

private:
	CLogging();
	~CLogging() {};
	CLogging(CLogging const&);
	CLogging& operator=(CLogging const&);

	static CLogging* m_pInstance;

	std::string m_StrInFile;
};
