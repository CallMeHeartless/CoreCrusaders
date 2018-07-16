//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name	: networkentity.h
// Description	: networkentity Initialisation file
// Author		: Sally Gillbanks
// Mail			: sally.Gil7363@mediadesign.school.nz
//

#ifndef __NETWORKENTITY_H__
#define __NETWORKENTITY_H__

#include <string>
#include <sstream>

enum EMessageType : unsigned char
{
	HANDSHAKE,
	DATA,
	KEEPALIVE,
	COMMAND,
	BROADCAST,
	USEREXISTS,
	QUIT,
};

enum EMessageSendType : unsigned char
{
	JUSTATYPE,
	NUMBEROFUSERS,
	START,
	PLAYER,
	ENTITIES,
	PICKUPS,
	BULLETS,
	LEVEL,
	LEVELINCREASE,
	SERVERLEAVE,
};

struct TPacket
{
	unsigned short MessageType;
	unsigned short MessageSendType;
	char MessageContent[500];
	char PacketData[610];
	unsigned short PacketSize;

	//void set_packet(short _x, short _y, WORD _object_type, short _object_index, WORD _param)
	void Serialize(EMessageType _MessageType, EMessageSendType _MessageSending, char* _message)
	{
		MessageType = _MessageType;
		MessageSendType = _MessageSending;
		strcpy_s(MessageContent, strlen(_message) + 1, _message);

		std::ostringstream oss;
		oss << MessageType;
		oss << " ";
		oss << MessageSendType;
		oss << " ";
		oss << MessageContent;

		std::string _strToSend = oss.str();
		const char* _pcToSend = _strToSend.c_str();

		strcpy_s(PacketData, strlen(_pcToSend) + 1, _pcToSend);

		PacketSize = static_cast<unsigned short>(_strToSend.size());
	}

	TPacket Deserialize(char* _PacketData)
	{
		std::string _strTemp(_PacketData);
		std::istringstream iss(_strTemp);

		iss >> this->MessageType;
		iss >> this->MessageSendType;

		//iss >> this->MessageContent;
		std::string _tempMessageBuffer;
		getline(iss, _tempMessageBuffer);
		strcpy_s(MessageContent, _tempMessageBuffer.length() + 1, _tempMessageBuffer.c_str());

		return *this;
	}

};

//Structure to hold the details of all connected clients
struct TClientDetails
{
	sockaddr_in m_ClientAddress;
	//bool m_bIsActive;
	std::string m_strAddress;
	//std::string m_strAddress;
	//time_t m_timeOfLastMessage;
};

class INetworkEntity
{
public:
	virtual bool Initialise() = 0; //Implicit in the intialization is the creation and binding of the socket
	virtual bool SendData(char* _pcDataToSend) = 0;
	virtual void ReceiveData() = 0;
	virtual void GetRemoteIPAddress(char *_pcSendersIP) = 0;
	virtual unsigned short GetRemotePort() = 0;
	virtual ~INetworkEntity() {};

protected:
	//Additional state variable to indicate whether a network entity is online or not
	bool m_bOnline;
};
#endif 