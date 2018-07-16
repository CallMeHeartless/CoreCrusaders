//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name	: client.h
// Description	: client declaration file
// Author		: Sally Gillbanks
// Mail			: sally.Gil7363@mediadesign.school.nz
//

#ifndef __CLIENT_H__
#define __CLIENT_H__

// Library Includes
#include <string>

// Local Includes
#include "networkentity.h"
#include "WorkQueue.h"

#include "Dependencies\freeglut\freeglut.h" 

// Types

// Constants


//Forward Declaration
class CSocket;

class CClient : public INetworkEntity
{
public:
	// Default Constructors/Destructors
	CClient();
	~CClient();

	// Methods

	virtual bool Initialise(); //Implicit in the intialization is the creation and binding of the socket
	virtual bool SendData(char* _pcDataToSend);
	virtual void ReceiveData();
	virtual void GetRemoteIPAddress(char* _pcSendersIP);
	virtual unsigned short GetRemotePort();

	bool GetOnline() const { return m_bOnlineCopyToSend; };

	void GetPacketData(char* _pcLocalBuffer);
	CWorkQueue<std::string>* GetWorkQueue();

	//Qs7 : Broadcast to Detect Servers
	bool BroadcastForServers();

	std::string GetUsername() const;

	void Reconnect();

	std::vector<std::string> GetServerList();
	bool SelectServer(unsigned int _iServer);

private:
	// Question 7 : Broadcast to Detect Servers
	void ReceiveBroadcastMessages(char* _pcBufferToReceiveData);

private:
	//A buffer to contain all packet data for the client
	char* m_pcPacketData;
	//A client has a socket object to create the UDP socket at its end.
	CSocket* m_pClientSocket;
	// A Sockaddress structure which will have the details of the server 
	sockaddr_in m_ServerSocketAddress;
	//A username to associate with a client
	char m_cUserName[50];
	//A workQueue to distribute messages between the main thread and Receive thread.
	CWorkQueue<std::string>* m_pWorkQueue;

	//Question 7
	//A vector to hold all the servers found after broadcasting
	std::vector<sockaddr_in> m_vecServerAddr;
	bool m_bDoBroadcast;

	//Username
	std::string m_strUsername;

	time_t m_iPreviousTime;

	std::thread m_ClientReceiveThread;

	std::string m_strAddress;

	bool m_bOnlineCopyToSend;
};

#endif