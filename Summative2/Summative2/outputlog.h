/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	outputlog.h
Description :	COutputLog class header file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/

#ifndef _OUTPUTLOG_H__
#define _OUTPUTLOG_H__

// Library includes
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <time.h>

class COutputLog {
	private:
		COutputLog();
		COutputLog(const COutputLog&) = delete;
		COutputLog& operator=(const COutputLog&) = delete;
		static COutputLog* m_spInstance;

		std::ofstream m_OutputStream;
		std::string m_strFileName;

	public:
		~COutputLog();

		static COutputLog* GetInstance();
		void DestroyInstance();

		bool BeginLogging();
		bool LogMessage(const char* _kcMessage);
		std::string GetTime();
};

#endif // !_OUTPUTLOG_H__
