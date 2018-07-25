/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	scenemanager.cpp
Description :	CSceneManager class implementation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/

// Local includes
#include "scenemanager.h"
#include "network.h"
#include "server.h"
#include "client.h"
#include "menu.h"
#include "Model.h"

// Static instance
CSceneManager* CSceneManager::m_pInstance = nullptr;

CSceneManager::CSceneManager(){}

CSceneManager::~CSceneManager() {
	// Shut down server instances
	if (m_pcPacketData) {
		delete[] m_pcPacketData;
		m_pcPacketData = nullptr;
	}
	if (m_pClient) {
		m_pClient->Deactivate();
		m_tClientReceiveThread.join();
	}
	else if (m_pServer) {
		m_pServer->Deactivate();
		m_tServerReceiveThread.join();
	}

	// Check here that is online
	CNetwork::GetInstance().DestroyInstance();

}

/***********************
* GetInstance: Returns a pointer to the singleton instance. Creates one if it does not exist.
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: CSceneManager* (m_pInstance)
********************/
CSceneManager* CSceneManager::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new CSceneManager();
	}

	return m_pInstance;
}

/***********************
* DestroyInstance: Releases memory for the singleton instance
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CSceneManager::DestroyInstance() {

	delete m_pInstance;
	m_pInstance = nullptr;
	COutputLog::GetInstance()->LogMessage("Scene Manager destroyed.");

}

/***********************
* Render: Renders the active scene or menu
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CSceneManager::Render() {
	m_pActiveScene->Render();
}

/***********************
* Process: Process one frame of game logic
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (seconds between frames)
* @return: void
********************/
void CSceneManager::Process(float _fDeltaTick){
	// Clear workqueue before processing scene
	//while (!CNetwork::GetInstance()->GetNetworkEntity()->GetWorkQueue()->empty()) {
	//	this->ProcessWork();
	//}


	
	if (IsDedicatedServer() && m_bInProgress) {
		// Clear work queue
		while (!m_pServer->GetWorkQueue()->empty()) {
			this->ProcessWork();
		}
		m_fLockStepTimer += _fDeltaTick;
		if (m_fLockStepTimer > Utility::fSERVER_UPDATE_COOLDOWN) {
			// Update
			m_fLockStepTimer = 0.0f;
			m_pActiveScene->UpdateClients();
		}
	}
	else if (m_pClient && m_bInProgress) {
		// Clear work queue
		while (!m_pClient->GetWorkQueue()->empty()) {
			this->ProcessWork();
		}
		m_fLockStepTimer += _fDeltaTick;
		if (m_fLockStepTimer > Utility::fCLIENT_UPDATE_COOLDOWN) {
			m_fLockStepTimer = 0.0f;
			m_pActiveScene->NotifyServer();
		}
	}

	m_pActiveScene->Process(_fDeltaTick);
}

/***********************
* LoadLevel: Loads a game scene with a specified map
* @author: Kerry Pellett (2018)
* @parameter: int _iMap (the map index to load)
* @return: bool (true on successful loading)
********************/
bool CSceneManager::LoadLevel(int _iMap) {
	m_bInProgress = true;
	m_pActiveScene.reset();
	m_iScore = 0;
	m_pActiveScene = std::make_unique<CScene>();

	return m_pActiveScene->Initialise(_iMap);
}

/***********************
* LoadGameMenu: Loads a game menu instance
* @author: Kerry Pellett (2018)
* @parameter: EMENUTYPE _eType (the desired menu to load)
* @return: bool (true)
********************/
bool CSceneManager::LoadGameMenu(EMENUTYPE _eType) {
	m_pActiveScene.reset();
	m_pActiveScene = std::make_unique<CMenu>(_eType);

	return true;
}

/***********************
* :
* @author: Kerry Pellett (2018)
* @parameter:
* @return:
********************/
void CSceneManager::PreloadModels() {
	// Preload models
	COutputLog::GetInstance()->LogMessage("Preloading models...");
	/// Removed for AI component
	///auto mod1 = std::make_pair<std::string, std::shared_ptr<Model>>("Resources/Models/JSTL/JSTL2.obj", std::make_shared<Model>("Resources/Models/JSTL/JSTL2.obj"));
	///m_pModels.insert(std::move(mod1));

	///auto mod2 = std::make_pair<std::string, std::shared_ptr<Model>>("Resources/Models/Wraith Raider Starship/Wraith Raider Starship2.obj", std::make_shared<Model>("Resources/Models/Wraith Raider Starship/Wraith Raider Starship2.obj"));
	///m_pModels.insert(std::move(mod2));

	auto mod3 = std::make_pair<std::string, std::shared_ptr<Model>>("Resources/Models/Tank/Tank.obj", std::make_shared<Model>("Resources/Models/Tank/Tank.obj"));
	m_pModels.insert(std::move(mod3));

	if (m_pModels.size() == 1) {
		COutputLog::GetInstance()->LogMessage("Models preloaded");
	}
}

/***********************
* SetScore: Store a reference to the player's score when switching scenes
* @author: Kerry Pellett (2018)
* @parameter: int _iScore (player's score)
* @return: void
********************/
void CSceneManager::SetScore(int _iScore) {
	m_iScore = _iScore;
}

/***********************
* GetScore: Return the stored reference to player's score
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: int (m_iScore)
********************/
int CSceneManager::GetScore()const {
	return m_iScore;
}

/***********************
* ToggleGodMode: Toggles the flag for godmode
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CSceneManager::ToggleGodMode() {
	m_bGodMode = !m_bGodMode;
}

/***********************
* ToggleMusic: Toggles flag to enable/disable music
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CSceneManager::ToggleMusic() {
	m_bPlayMusic = !m_bPlayMusic;
}

/***********************
* CheckForGodMode: Returns flag state for godmode
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (m_bGodMode)
********************/
bool CSceneManager::CheckForGodMode()const {
	return m_bGodMode;
}

/***********************
* CheckForMusic: Returns flag state for music
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (m_bPlayMusic)
********************/
bool CSceneManager::CheckForMusic()const {
	return m_bPlayMusic;
}

/***********************
* ToggleAITest: Toggles flag state for AI Testing
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CSceneManager::ToggleAITest() {
	m_bAITest = !m_bAITest;
}

/***********************
* CheckForAITest: Returns flag state for AI testing
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (m_bAITest)
********************/
bool CSceneManager::CheckForAITest()const {
	return m_bAITest;
}

/***********************
* SetServerName: Sets the name of a hosted server
* @author: Kerry Pellett (2018)
* @parameter: std::string (server's name)
* @return: void
********************/
void CSceneManager::SetServerName(std::string _strName) {
	m_strServerName = _strName;
}

/***********************
* GetServerName: Returns the local server's name
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: std::string (m_strServerName)
********************/
std::string CSceneManager::GetServerName()const {
	return m_strServerName;
}

/***********************
* HostServer: Initialises a dedicated server instance
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (true if the server is successfully initalised, false otherwise)
********************/
bool CSceneManager::HostServer() {
	// Enable network
	CNetwork::GetInstance().StartUp();

	// Ensure no music is played
	m_bPlayMusic = false;

	if (CNetwork::GetInstance().Initialise(SERVER)) {
		COutputLog::GetInstance()->LogMessage("Initialised as server.");
		// Set server pointer
		m_pServer = static_cast<CServer*>(CNetwork::GetInstance().GetNetworkEntity());
		m_pServer->SetServerName(m_strServerName);
		COutputLog::GetInstance()->LogMessage("Initialising data reception thread...");
		m_pcPacketData = new char[MAX_MESSAGE_LENGTH];
		strcpy_s(m_pcPacketData, strlen("") + 1, "");
		m_tServerReceiveThread = std::thread(&CServer::ReceiveData, m_pServer, std::ref(m_pcPacketData));
	}
	else {
		COutputLog::GetInstance()->LogMessage("CRITICAL ERROR: Server was unable to initialise");
		return false;
	}

	return true;
}

/***********************
* CreateClient: Creates a client instance
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (true if the client instance is successfully created, false otherwise)
********************/
bool CSceneManager::CreateClient() {
	// Enable network
	CNetwork::GetInstance().StartUp();

	if (CNetwork::GetInstance().Initialise(CLIENT)) {
		COutputLog::GetInstance()->LogMessage("Creating client instance.");
		// Set client pointer
		m_pClient = static_cast<CClient*>(CNetwork::GetInstance().GetNetworkEntity());
		COutputLog::GetInstance()->LogMessage("Initialising data reception thread...");
		//m_pcPacketData = new char[MAX_MESSAGE_LENGTH];
		//strcpy_s(m_pcPacketData, strlen("") + 1, "");
		//m_tClientReceiveThread = std::thread(&CClient::ReceiveData, m_pClient, std::ref(m_pcPacketData));
	}
	else {
		COutputLog::GetInstance()->LogMessage("CRITICAL ERROR: Client instance was unable to initailise.");
		return false;
	}

	return true;
}

/***********************
* IsServerReadyToLaunch: Returns the flag that launches the server game when connected clients are ready
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (m_bReadyToLaunch)
********************/
bool CSceneManager::IsServerReadyToLaunch()const {
	return m_bReadyToLaunch;
}

/***********************
* LaunchServer: Launches the server game for connected clients
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CSceneManager::LaunchServer() {
	// Load scene
	m_bInProgress = true;
	COutputLog::GetInstance()->LogMessage("Starting game...");
	m_pServer->LaunchGame();
	this->LoadLevel(1);
}

/***********************
* IsDedicatedServer: Checks if the game is running as a dedicated server
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (true if m_pServer is not a nullptr)
********************/
bool CSceneManager::IsDedicatedServer()const {
	return !(m_pServer == nullptr);
}

/***********************
* ProcessWork: Checks the network's work queue and processes any pending data
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CSceneManager::ProcessWork() {
	if (m_pServer) {
		if (!m_pServer->GetWorkQueue()->empty()) {
			CNetwork::GetInstance().GetNetworkEntity()->GetRemoteIPAddress(m_cIPAddress);
			// Extract data to process
			std::string strData;
			m_pServer->GetWorkQueue()->pop(strData);
			m_pServer->ProcessData(const_cast<char*>(strData.c_str()));
			//m_pServer->ProcessData(m_pcPacketData);
		}
	}
	else if (m_pClient) {
		if (!m_pClient->GetWorkQueue()->empty()) {
			CNetwork::GetInstance().GetNetworkEntity()->GetRemoteIPAddress(m_cIPAddress);
			// Extract data to process
			std::string strData;
			m_pClient->GetWorkQueue()->pop(strData);
			m_pClient->ProcessData(const_cast<char*>(strData.c_str()));
		}
	}
}

/***********************
* GetClientInstance: Returns the client instance
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: CClient* (m_pClient)
********************/
CClient* CSceneManager::GetClientInstance() {
	return m_pClient;
}

/***********************
* LaunchClientDataThread: Launches a thread to receive data asynchronously for the client
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CSceneManager::LaunchClientDataThread() {
	m_pcPacketData = new char[MAX_MESSAGE_LENGTH];
	strcpy_s(m_pcPacketData, strlen("") + 1, "");
	m_tClientReceiveThread = std::thread(&CClient::ReceiveData, m_pClient, std::ref(m_pcPacketData));
}

/***********************
* SetInProgress: Sets a flag that controls if the scene manager has launched an instance of the game
* @author: Kerry Pellett (2018)
* @parameter: bool _bState (the target flag state)
* @return: void
********************/
void CSceneManager::SetInProgress(bool _bState) {
	m_bInProgress = _bState;
}

/***********************
* CheckIfInProgress: Checks to see if the server has launched the game already
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (m_bInProgress)
********************/
bool CSceneManager::CheckIfInProgress()const {
	return m_bInProgress;
}

/***********************
* GetNumberOfPlayers: Gets the number of clients connected to the server
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: unsigned int (m_pServer's );
********************/
unsigned int CSceneManager::GetNumberOfPlayers()const {
	//return m_pServer->GetNumberOfConnectedClients();
	return m_vecstrPlayerNames.size();
}

/***********************
* IsOnline: Checks if the game is being run online
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool (m_bIsOnline)
********************/
bool CSceneManager::IsOnline()const {
	return m_bIsOnline;
}

/***********************
* SetOnline: Sets a flag marking if the game is multiplayer
* @author: Kerry Pellett (2018)
* @parameter: bool _bState (new state for the flag)
* @return: void
********************/
void CSceneManager::SetOnline(bool _bState) {
	m_bIsOnline = _bState;
}

void CSceneManager::SendPlayerList() {
	m_pServer->SendPlayerList();
}

void CSceneManager::SetPlayerIndex(unsigned int _iIndex) {
	m_uiClientInstance = _iIndex;
	static_cast<CMenu*>(m_pActiveScene.get())->SetPlayerIndex(_iIndex);
}

unsigned int CSceneManager::GetPlayerIndex()const {
	return m_uiClientInstance;
}

std::vector<std::string> CSceneManager::GetPlayerNames()const {
	return m_vecstrPlayerNames;
}

void CSceneManager::DeserialisePlayerPosition(int _iPlayerIndex, std::string _strPosition) {
	m_pActiveScene->DeserialisePlayerPosition(_iPlayerIndex, _strPosition);
}

void CSceneManager::SetPlayerLobbyReadyState(int _iPlayerIndex, bool _bPlayer) {
	// Break if game has started
	if (m_bInProgress) {
		return;
	}
	static_cast<CMenu*>(m_pActiveScene.get())->SetPlayerLobbyReadyState(_iPlayerIndex, _bPlayer);
}

void CSceneManager::UpdateServerPlayerLobbyReadyState(int _iPlayerIndex, bool _bState) {
	m_pClient->AlertServerPlayerLobbyReadyState(_iPlayerIndex, _bState);
}

void CSceneManager::SetPlayerLobbyNameList(std::vector<std::string> _vecstrNames) {
	m_vecstrPlayerNames = _vecstrNames;
	static_cast<CMenu*>(m_pActiveScene.get())->SetPlayerLobbyNames(_vecstrNames);
}

void CSceneManager::SendGameData(char* _cPacketData) {
	if (m_pClient) {
		m_pClient->SendData(_cPacketData);
	}
	else if (m_pServer) {
		m_pServer->SendToAll(_cPacketData);
	}

}

void CSceneManager::DeserialisePlayerVelocity(int _iPlayerIndex, std::string _strVelocity) {
	m_pActiveScene->DeserialisePlayerVelocity(_iPlayerIndex, _strVelocity);
}

void CSceneManager::DeserialisePickupData(unsigned int _iPickupIndex, std::string _strData) {
	m_pActiveScene->DeserialisePickupData(_iPickupIndex, _strData);
}

void CSceneManager::DeserialisePickupPosition(unsigned int _iPickupIndex, std::string _strPosition) {
	m_pActiveScene->DeserialisePickupPosition(_iPickupIndex, _strPosition);
}

void CSceneManager::DeserialiseSoloAgentPosition(unsigned int _iAgentIndex, std::string _strPosition) {
	m_pActiveScene->DeserialiseSoloAgentPosition(_iAgentIndex, _strPosition);
}

void CSceneManager::DeserialiseGroupAgentPosition(unsigned int _iGroupIndex, unsigned int _iAgentIndex, std::string _strPosition) {
	m_pActiveScene->DeserialiseGroupAgentPosition(_iGroupIndex, _iAgentIndex, _strPosition);
}

void CSceneManager::DeserialiseBulletPosition(unsigned int _uiBulletIndex, std::string _strData) {
	m_pActiveScene->DeserialiseBulletPosition(_uiBulletIndex, _strData);
}

void CSceneManager::DeserialiseBulletData(unsigned int _uiBulletIndex, std::string _strData) {
	m_pActiveScene->DeserialiseBulletData(_uiBulletIndex, _strData);
}

void CSceneManager::DeserialiseBulletVelocity(unsigned int _uiBulletIndex, std::string _strData) {
	m_pActiveScene->DeserialiseBulletVelocity(_uiBulletIndex, _strData);
}

void CSceneManager::DeserialiseSoloAgentData(unsigned int _uiAgentIndex, std::string _strData) {
	m_pActiveScene->DeserialiseSoloAgentData(_uiAgentIndex, _strData);
}

void CSceneManager::DeserialiseGroupAgentData(unsigned int _uiGroupIndex, unsigned int _uiAgentIndex, std::string _strData) {
	m_pActiveScene->DeserialiseGroupAgentData(_uiGroupIndex, _uiAgentIndex, _strData);
}

void CSceneManager::AffirmBulletCount(unsigned int _uiBulletCount) {
	m_pActiveScene->AffirmBulletCount(_uiBulletCount);
}

void CSceneManager::RemovePlayer(unsigned int _uiPlayerIndex, unsigned int _uiNewLocalPlayerIndex) {
	if (_uiPlayerIndex > 3 || _uiNewLocalPlayerIndex > 3) {
		return;
	}
	// Erase player from vectors
	m_vecstrPlayerNames.erase(m_vecstrPlayerNames.begin() +  _uiPlayerIndex);
	m_uiClientInstance = _uiNewLocalPlayerIndex;
	COutputLog::GetInstance()->LogMessage(const_cast<char*>((std::string("Local user reference set to: ") + Utility::ToString(_uiNewLocalPlayerIndex)).c_str()));
	if (m_bInProgress) {
		m_pActiveScene->ErasePlayer(_uiPlayerIndex, _uiNewLocalPlayerIndex);
	}
}

void CSceneManager::DeserialiseAndCreateBullet(std::string _strBullet) {
	m_pActiveScene->DeserialiseAndCreateBullet(_strBullet);
}

void CSceneManager::DeserialisePlayerData(unsigned int _uiPlayerIndex, std::string _strData) {
	m_pActiveScene->DeserialisePlayerData(_uiPlayerIndex, _strData);
}

void CSceneManager::LoadMap(unsigned int _uiMapIndex) {
	switch (_uiMapIndex) {
		case 1: {
			m_pActiveScene->LoadMap(MAPS::MAP_1);
			break;
		}

		case 2: {
			m_pActiveScene->LoadMap(MAPS::MAP_2);
			break;
		}

		case 3: {
			m_pActiveScene->LoadMap(MAPS::MAP_3);
			break;
		}

		default:break;
	}
}