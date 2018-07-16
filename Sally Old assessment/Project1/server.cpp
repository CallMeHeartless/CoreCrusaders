//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name	: server.cpp
// Description	: server Initialisation file
// Author		: Sally Gillbanks
// Mail			: sally.Gil7363@mediadesign.school.nz
//

//Library Includes
#include <WS2tcpip.h>
#include <iostream>
#include <utility>
#include <thread>
#include <chrono>


//Local Includes
#include "utils.h"
#include "network.h"
//#include "consoletools.h"
#include "socket.h"


//Local Includes
#include "server.h"

/***********************
* CServer
* @author: Sally Gillbanks
* @parameter: none
* @return: CServer
********************/
CServer::CServer()
	:m_pcPacketData(0),
	m_pServerSocket(0)
{
	ZeroMemory(&m_ClientAddress, sizeof(m_ClientAddress));
}

/***********************
* ~CServer
* @author: Sally Gillbanks
* @parameter: none
* @return: 
********************/
CServer::~CServer()
{
	m_bOnline = false;

	m_ServerReceiveThread.join();

	delete m_pConnectedClients;
	m_pConnectedClients = 0;

	delete m_pServerSocket;
	m_pServerSocket = 0;

	delete m_pWorkQueue;
	m_pWorkQueue = 0;

	delete m_pWorkQueueUser;
	m_pWorkQueueUser = 0;

	delete[] m_pcPacketData;
	m_pcPacketData = 0;
}

/***********************
* Initialise
* @author: Sally Gillbanks
* @parameter: none
* @return: bool
********************/
bool CServer::Initialise()
{
	m_pcPacketData = new char[MAX_MESSAGE_LENGTH];

	//Create a work queue to distribute messages between the main  thread and the receive thread.
	m_pWorkQueue = new CWorkQueue<std::string>();
	m_pWorkQueueUser = new CWorkQueue<std::string>();

	//Create a socket object
	m_pServerSocket = new CSocket();

	//Get the port number to bind the socket to
	unsigned short _usServerPort = DEFAULT_SERVER_PORT;

	//Initialise the socket to the local loop back address and port number
	if (!m_pServerSocket->Initialise(_usServerPort))
	{
		return false;
	}

	m_bOnline = true;

	//Qs 2: Create the map to hold details of all connected clients
	m_pConnectedClients = new std::map < std::string, TClientDetails >();

	m_iKeepAliveTimerAsk = 5;
	m_iKeepAliveTimerDelete = 5;

	m_iOldTimeServer = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	m_ServerReceiveThread = std::thread(&CServer::ReceiveData, this);
	return true;
}

/***********************
* AddClient
* @author: Sally Gillbanks
* @parameter: string
* @return: bool
********************/
bool CServer::AddClient(std::string _strClientName)
{
	//TO DO : Add the code to add a client to the map here...

	for (auto it = m_pConnectedClients->begin(); it != m_pConnectedClients->end(); ++it)
	{
		//Check to see that the client to be added does not already exist in the map, 
		if (it->first == ToString(m_ClientAddress))
		{
			return false;
		}
		//also check for the existence of the username
		/*else if (it->second.m_strName == _strClientName)
		{
		return false;
		}*/
	}
	if (m_pConnectedClients->size() == 3)
	{
		return false;
	}
	//Add the client to the map.
	TClientDetails _clientToAdd;
	//_clientToAdd.m_strName = _strClientName;
	_clientToAdd.m_ClientAddress = this->m_ClientAddress;

	std::string _strAddress = ToString(m_ClientAddress);
	m_pConnectedClients->insert(std::pair < std::string, TClientDetails >(_strAddress, _clientToAdd));
	return true;
}
/***********************
* GetNumberOfClients
* @author: Sally Gillbanks
* @parameter: none
* @return: int
********************/
int CServer::GetNumberOfClients()
{
	return(m_pConnectedClients->size());
}
/***********************
* GetAddress
* @author: Sally Gillbanks
* @parameter: none
* @return: sockaddr_in
********************/
sockaddr_in CServer::GetAddress()
{
	return(m_ClientAddress);
}
/***********************
* GetStrAddress
* @author: Sally Gillbanks
* @parameter: none
* @return: string
********************/
std::string CServer::GetStrAddress()
{
	return(ToString(m_ClientAddress));
}
/***********************
* GetStrAddressOfSpecificClient
* @author: Sally Gillbanks
* @parameter: int
* @return: string
********************/
std::string CServer::GetStrAddressOfSpecificClient(int _iClientNumber)
{
	TClientDetails _client;//_client = m_pConnectedClients[_iClientNumber];
	int iNumber = 0;
	for (auto it = m_pConnectedClients->begin(); it != m_pConnectedClients->end(); ++it)
	{
		if (_iClientNumber == iNumber)
		{
			_client = it->second;
		}
		++iNumber;
	}

	return(ToString(_client.m_ClientAddress));
}

/***********************
* SendData
* @author: Sally Gillbanks
* @parameter: char*
* @return: bool
********************/
bool CServer::SendData(char* _pcDataToSend)
{
	int _iBytesToSend = (int)strlen(_pcDataToSend) + 1;

	int iNumBytes = sendto(
		m_pServerSocket->GetSocketHandle(),				// socket to send through.
		_pcDataToSend,									// data to send
		_iBytesToSend,									// number of bytes to send
		0,												// flags
		reinterpret_cast<sockaddr*>(&m_ClientAddress),	// address to be filled with packet target
		sizeof(m_ClientAddress)							// size of the above address struct.
	);
	//iNumBytes;
	if (_iBytesToSend != iNumBytes)
	{
		std::cout << "There was an error in sending data from client to server" << std::endl;
		return false;
	}

	//Need to see if can't send, to return false
	return true;
}

/***********************
* SendToAll
* @author: Sally Gillbanks
* @parameter: char*
* @return: bool
********************/
bool CServer::SendToAll(char* _pcDataToSend)
{
	int _iBytesToSend = (int)strlen(_pcDataToSend) + 1;

	int iNumBytes;
	sockaddr_in TempClientAddress;

	for (auto it = m_pConnectedClients->begin(); it != m_pConnectedClients->end(); ++it)
	{
		TempClientAddress = it->second.m_ClientAddress;
		iNumBytes = sendto(
			m_pServerSocket->GetSocketHandle(),				// socket to send through.
			_pcDataToSend,									// data to send
			_iBytesToSend,									// number of bytes to send
			0,												// flags
			reinterpret_cast<sockaddr*>(&TempClientAddress),	// address to be filled with packet target
			sizeof(TempClientAddress)							// size of the above address struct.
		);
	}
	return true;
}
/***********************
* SendToAllExcept
* @author: Sally Gillbanks
* @parameter: char*, string
* @return: bool
********************/
bool CServer::SendToAllExcept(char* _pcDataToSend, std::string _Avoid)
{
	int _iBytesToSend = (int)strlen(_pcDataToSend) + 1;

	int iNumBytes;
	sockaddr_in TempClientAddress;

	for (auto it = m_pConnectedClients->begin(); it != m_pConnectedClients->end(); ++it)
	{
		TempClientAddress = it->second.m_ClientAddress;

		if (ToString(it->second.m_ClientAddress) == _Avoid)
		{
			return true;
			break;
		}

		iNumBytes = sendto(
			m_pServerSocket->GetSocketHandle(),				// socket to send through.
			_pcDataToSend,									// data to send
			_iBytesToSend,									// number of bytes to send
			0,												// flags
			reinterpret_cast<sockaddr*>(&TempClientAddress),	// address to be filled with packet target
			sizeof(TempClientAddress)							// size of the above address struct.
		);
	}
	return true;
}

/***********************
* ReceiveData
* @author: Sally Gillbanks
* @parameter: none
* @return: void
********************/
void CServer::ReceiveData()
{
	//set a timer on the socket for 15 sec
	struct timeval timeValue;

	int iSizeOfAdd = sizeof(m_ClientAddress);
	int _iNumOfBytesReceived;
	int _iPacketSize;

	//Make a thread local buffer to receive data into
	char _buffer[MAX_MESSAGE_LENGTH];

	while (m_bOnline)
	{
		timeValue.tv_sec = 15;
		timeValue.tv_usec = 0;
		setsockopt(m_pServerSocket->GetSocketHandle(), SOL_SOCKET, SO_RCVTIMEO,
			(char*)&timeValue, sizeof(timeValue));


		// pull off the packet(s) using recvfrom()
		_iNumOfBytesReceived = recvfrom(			// pulls a packet from a single source...
			m_pServerSocket->GetSocketHandle(),						// client-end socket being used to read from
			_buffer,							// incoming packet to be filled
			MAX_MESSAGE_LENGTH,					   // length of incoming packet to be filled
			0,										// flags
			reinterpret_cast<sockaddr*>(&m_ClientAddress),	// address to be filled with packet source
			&iSizeOfAdd								// size of the above address struct.
		);
		if (_iNumOfBytesReceived < 0)
		{
			//Error in receiving data 
			int _iError = WSAGetLastError();
			//std::cout << "recvfrom failed with error " << _iError;
			if (_iError == WSAETIMEDOUT) // Socket timed out on Receive
			{
				//m_bOnline = false; //Do not broadcast any more
				//break;
			}
		}
		else
		{
			_iPacketSize = static_cast<int>(strlen(_buffer)) + 1;

			char _IPAddress[100];
			inet_ntop(AF_INET, &m_ClientAddress.sin_addr, _IPAddress, sizeof(_IPAddress));

			std::cout << "Server Received \"" << _buffer << "\" from " <<
				_IPAddress << ":" << ntohs(m_ClientAddress.sin_port) << std::endl;
			//Push this packet data into the WorkQ

			std::string Temp = ToString(m_ClientAddress);

			m_pWorkQueue->push(_buffer);
			m_pWorkQueueUser->push(Temp);

		}
		//std::this_thread::yield();

	} //End of while (true)
}

void CServer::GetRemoteIPAddress(char *_pcSendersIP)
{
	char _temp[MAX_ADDRESS_LENGTH];
	int _iAddressLength;
	inet_ntop(AF_INET, &(m_ClientAddress.sin_addr), _temp, sizeof(_temp));
	_iAddressLength = static_cast<int>(strlen(_temp)) + 1;
	strcpy_s(_pcSendersIP, _iAddressLength, _temp);
}

/***********************
* GetRemotePort
* @author: Sally Gillbanks
* @parameter: none
* @return: unsigned short
********************/
unsigned short CServer::GetRemotePort()
{
	return ntohs(m_ClientAddress.sin_port);
}

/***********************
* ActiveUsers
* @author: Sally Gillbanks
* @parameter: none
* @return: void
********************/
void CServer::ActiveUsers()
{
	std::string strStringToSend = "Active users are: ";
	//Broadcast all current active users
	for (auto it = m_pConnectedClients->begin(); it != m_pConnectedClients->end(); ++it)
	{
		//strStringToSend += it->second.m_strName.c_str();
		strStringToSend += ", ";
	}
	TPacket packetToSend;
	packetToSend.Serialize(DATA, JUSTATYPE, const_cast<char*>(strStringToSend.c_str()));
	SendToAll(packetToSend.PacketData);
}

/***********************
* CheckActive
* @author: Sally Gillbanks
* @parameter: vector<string>&
* @return: int
********************/
int CServer::CheckActive(std::vector<std::string>& _Replies)
{
	/* To send a package from the server */
	TPacket packetToSend;
	std::string strStringToSend;


	/* KeepAlive timer */
	if (-1.0f >= (m_iOldTimeServer - (GLfloat)glutGet(GLUT_ELAPSED_TIME)) / 1000)
	{
		packetToSend.Serialize(KEEPALIVE, JUSTATYPE, "Test");
		SendToAll(packetToSend.PacketData);

		m_iKeepAliveTimerDelete -= 1;

		//Keeps sending message every second, if message doesn't send, it wil be picked up by recieve data error, and therefore drop the user
		if (0 > m_iKeepAliveTimerDelete)
		{
			bool foundReply;
			int j = 1;
			for (auto it = m_pConnectedClients->begin(); it != m_pConnectedClients->end();)
			{
				foundReply = false;
				for (unsigned int i = 0; i < _Replies.size(); ++i)
				{
					if (_Replies[i] == ToString(it->second.m_ClientAddress))
					{
						foundReply = true;
					}
				}

				if (false == foundReply)
				{
					//See if it can send data to the client, if can't, then remove that player
					packetToSend.Serialize(QUIT, JUSTATYPE, "You are not active");
					SendData(packetToSend.PacketData);
					it = m_pConnectedClients->erase(it);
					_Replies.clear();

					m_iKeepAliveTimerDelete = 30;
					m_iOldTimeServer = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
					return j;
					/*if (!SendData(packetToSend.PacketData)) // If can't send to that client, send false
					{
						it = m_pConnectedClients->erase(it);
						_Replies.clear();

						m_iKeepAliveTimerDelete = 10;
						m_iOldTimeServer = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
						return j;
					}*/
				}
				else
				{
					++it;
				}

				++j;
				if (5 < j)
				{
					break;
				}
			}
			_Replies.clear();
			m_iKeepAliveTimerDelete = 30;
		}
		m_iOldTimeServer = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	}
	return 5;
}

/***********************
* GetWorkQueue
* @author: Sally Gillbanks
* @parameter: none
* @return: CWorkQueue<string>*
********************/
CWorkQueue<std::string>* CServer::GetWorkQueue()
{
	return m_pWorkQueue;
}

/***********************
* GetWorkQueue2
* @author: Sally Gillbanks
* @parameter: none
* @return: CWorkQueue<string>*
********************/
CWorkQueue<std::string>* CServer::GetWorkQueue2()
{
	return m_pWorkQueueUser;
}
