//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name :		camera.cpp
// Description :	camera Initialisation file.
// Author : Sally Gillbanks
// Mail : sally.Gil7363@mediadesignschool.com
//

#include "Logging.h"

CLogging* CLogging::m_pInstance = nullptr;

/***********************
* CLogging
* @author: Sally Gillbanks
* @parameter: Creates the CLogging Instance
* @return: CLogging
********************/
CLogging::CLogging()
{
}

/***********************
* GetInstance
* @author: Sally Gillbanks
* @parameter: Gets the CLogging singlton instance
* @return: CLogging*
********************/
CLogging* CLogging::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_pInstance = new CLogging();
	}
	return(m_pInstance);
}

/***********************
* AddStatus
* @author: Sally Gillbanks
* @parameter: Add to file
* @return: void
********************/
void CLogging::AddStatus(std::string _AddToFile)
{
	std::ofstream myfile("Log.txt");
	if (myfile.is_open())
	{
		m_StrInFile += _AddToFile + "\n";
		myfile << m_StrInFile.c_str();
	}
	myfile.close();
}

/***********************
* DeleteInstance
* @author: Sally Gillbanks
* @parameter: Deletes the instance
* @return: void
********************/
void CLogging::DeleteInstance()
{
	delete m_pInstance;
}