//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name	: server.h
// Description	: server declaration file
// Author		: Sally Gillbanks
// Mail			: sally.Gil7363@mediadesign.school.nz
//

#ifndef __SERVER_H__
#define __SERVER_H__

// Library Includes
#include <Windows.h>
#include <map>
#include <time.h>

// Local Includes
#include "networkentity.h"
#include "WorkQueue.h"

#include "Dependencies\glew\glew.h" 
#include "Dependencies\freeglut\freeglut.h" 
#include "Dependencies\soil\soil.h" 

// Types

// Constants

//Forward Declaration
class CSocket;

class CServer : public INetworkEntity
{
public:
	// Default Constructors/Destructors
	CServer();
	~CServer();

	// Virtual Methods from the Network Entity Interface.
	virtual bool Initialise(); //Implicit in the intialization is the creation and binding of the socket
	virtual bool SendData(char* _pcDataToSend);
	virtual bool SendToAll(char* _pcDataToSend);
	virtual bool SendToAllExcept(char* _pcDataToSend, std::string _Avoid);
	virtual void ReceiveData();
	virtual void GetRemoteIPAddress(char* _pcSendersIP);
	virtual unsigned short GetRemotePort();

	CWorkQueue<std::string>* GetWorkQueue();
	CWorkQueue<std::string>* GetWorkQueue2();
	//Qs 2: Function to add clients to the map.

	void ActiveUsers();
	int CheckActive(std::vector<std::string>& _Replies);

	bool AddClient(std::string _strClientName);
	int GetNumberOfClients();
	sockaddr_in GetAddress();
	std::string GetStrAddress();

	std::string GetStrAddressOfSpecificClient(int _iClientNumber);
private:

private:
	//A Buffer to contain all packet data for the server
	char* m_pcPacketData;
	//A server has a socket object to create the UDP socket at its end.
	CSocket* m_pServerSocket;
	// Make a member variable to extract the IP and port number of the sender from whom we are receiving
	//Since it is a UDP socket capable of receiving from multiple clients; these details will change depending on who has sent the packet we are currently processing.
	sockaddr_in m_ClientAddress;

	//Qs 2 : Make a map to hold the details of all the client who have connected. 
	//The structure maps client addresses to client details
	std::map<std::string, TClientDetails>* m_pConnectedClients;

	//A workQueue to distribute messages between the main thread and Receive thread.
	CWorkQueue<std::string>* m_pWorkQueue;
	CWorkQueue<std::string>* m_pWorkQueueUser;

	//std::string m_strLastUser;

	int m_iKeepAliveTimerAsk;
	int m_iKeepAliveTimerDelete;
	bool m_bKeepAliveTimerDeleteCounting;

	std::mutex m_Temp;

	std::thread m_ServerReceiveThread;

	GLfloat m_iOldTimeServer;
};

#endif