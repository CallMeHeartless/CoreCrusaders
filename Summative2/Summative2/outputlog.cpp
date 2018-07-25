/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	outputlog.cpp
Description :	COutputLog class implementation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/

// Local includes
#include "outputlog.h"

// Disable C4996 for now
#pragma warning(disable : 4996)

// Static instance
COutputLog* COutputLog::m_spInstance = nullptr;

COutputLog::COutputLog(){}

COutputLog::~COutputLog(){}

/***********************
* GetInstance: Returns a pointer to the singleton instance. Creates a new instance if none exists
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: COutputLog* (m_spInstance)
********************/
COutputLog* COutputLog::GetInstance() {
	if (!m_spInstance) {
		m_spInstance = new COutputLog();
	}

	return m_spInstance;
}

/***********************
* DestroyInstance: Releases memory for the singleton instance
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void COutputLog::DestroyInstance() {
	delete m_spInstance;
	m_spInstance = nullptr;
}

/***********************
* GetLogFileTimeStamp: Returns the timestamp in a filename compatable format
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: std::string (time stamp)
********************/
std::string GetLogFileTimeStamp() {
	// Get time
	auto time = std::chrono::system_clock::now();
	char pcBuffer[80];
	time_t tTime = std::chrono::system_clock::to_time_t(time);
	struct tm* tTimeInfo;
	tTimeInfo = localtime(&tTime);

	// Format time info
	size_t result = strftime(pcBuffer, 80, "%F %H-%M-%S", tTimeInfo);

	if (result == 0) {
		// Invalid timestamp
		return std::string("ERROR: Could not generate timestamp");
	}

	std::string strMessage(pcBuffer);
	return strMessage;

}

/***********************
* BeginLogging: Creates a new log and initialises files
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (true)
********************/
bool COutputLog::BeginLogging() {
	// Create filename
	m_strFileName = "Logs/Log " + GetLogFileTimeStamp() + ".txt";

	// Open file
	m_OutputStream.open(m_strFileName, std::ios::out | std::ios::app);
	_ASSERT(m_OutputStream.is_open());

	// Log 
	m_OutputStream << GetTime() + " >> Beginning Logging..." << std::endl;

	// Close file
	m_OutputStream.close();
	return true;
}

/***********************
* LogMessage: Outputs a message to file and command window
* @author: Kerry Pellett (2018)
* @parameter: const char* _kcMessage (message to be logged)
* @return: bool (true)
********************/
bool COutputLog::LogMessage(const char* _kcMessage) {
	// Open log
	m_OutputStream.open(m_strFileName, std::ios::out | std::ios::app);

	// return false if file could not be opened
	if (!m_OutputStream.is_open()) {
		return false;
	}

	// Get timestamp
	auto time = std::chrono::system_clock::now();
	time_t tTime = std::chrono::system_clock::to_time_t(time);

	// Form and output message
	std::string strMessage = GetTime() + " >> " + std::string(_kcMessage) + "\n";
	m_OutputStream << strMessage;
	std::cout << strMessage;
	// Close file
	m_OutputStream.close();
	return true;
}

/***********************
* GetTime: Gets the current time in string format
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: std::string (current time)
********************/
std::string COutputLog::GetTime() {
	// Get time
	auto time = std::chrono::system_clock::now();
	char pcBuffer[80];
	time_t tTime = std::chrono::system_clock::to_time_t(time);
	struct tm* tTimeInfo;
	tTimeInfo = localtime(&tTime);

	// Format time info
	size_t result = strftime(pcBuffer, 80, "%D:%T", tTimeInfo);

	if (result == 0) {
		// Invalid timestamp
		return std::string("ERROR: Could not generate timestamp");
	}

	std::string strMessage(pcBuffer);
	return strMessage;
}