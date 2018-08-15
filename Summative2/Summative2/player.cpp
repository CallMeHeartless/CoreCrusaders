/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	player.cpp
Description :	Player class implementation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/
#include "player.h"
#include "input.h"

CPlayer::CPlayer(){
	// Create sprite
	/*m_pSprite = std::make_unique<CSprite>();
	m_pSprite->Initialise("Resources/Textures/Player.png");

	m_pSprite->SetFrameWidth(m_pSprite->GetWidth() / 3.0f);

	m_pSprite->SetFrameHeight(m_pSprite->GetHeight() / 4.0f);

	m_pSprite->SetIndex(1, 2);*/
}

CPlayer::~CPlayer(){}

/***********************
* Process: Handle the player for the current frame
* @author: Kerry Pellett (2018)
* @parameter: float _fDeltaTick (seconds between frames)
* @return: void
********************/
void CPlayer::Process(float _fDeltaTick) {

}

/***********************
* SetAlive: Set's the player IsAlive flag
* @author: Kerry Pellett (2018)
* @parameter: bool _bState to set flag
* @return: void
********************/
void CPlayer::SetAlive(bool _bState) {
	m_bIsAlive = _bState;
}

/***********************
* IsMoving: Returns the player's IsMoving flag
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool that is m_bIsMoving
********************/
bool CPlayer::IsMoving()const {
	return m_bIsMoving;
}

/***********************
* GetAlive: Returns the player's IsAlive flag
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: bool that is m_bIsAlive
********************/
bool CPlayer::GetAlive()const {
	return m_bIsAlive;
}