/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	input.h
Description :	CInput class header file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/

#ifndef __INPUT_H__
#define __INPUT_H__

// Library includes
#include <vector>

// Local includes
#include "utility.h"

// Forward Declaration
class CCamera;

// Defines the state of a button or key
enum EINPUTSTATE {
	INPUT_FIRST_RELEASED,
	INPUT_RELEASED,
	INPUT_FIRST_PRESSED,
	INPUT_PRESSED
};

enum EINPUTMOUSE {
	MOUSE_LEFT,
	MOUSE_MIDDLE,
	MOUSE_RIGHT
};

class CInput {
	private:
		CInput();
		CInput(const CInput&) = delete;
		CInput& operator=(const CInput&) = delete;
		static CInput* s_pInstance;

		unsigned int m_uiKeyState[255] = { INPUT_RELEASED };
		unsigned int m_uiMouseState[3] = { INPUT_RELEASED };
		unsigned int m_uiArrowKeyState[4] = { INPUT_RELEASED };
		glm::vec2 m_vfMousePosition;

	public:
		~CInput();

		static CInput* GetInstance();
		void DestroyInstance();

		void Initialise();
		
		// Mouse
		void MouseButton(int _iButton, int _iButtonState, int _iX, int _iY);
		void MouseMovement(int _iX, int _iY);
		glm::vec2 GetMousePosition()const;
		unsigned int GetMouseButton(EINPUTMOUSE _eMouseButton)const;
		glm::vec3 MouseRay(const CCamera* const _kpCamera);
		glm::vec3 MouseIntersectionWithPlane(glm::vec3 _vfMouseRay, glm::vec3 _vfCameraPosition, glm::vec3 _vfPlaneNormal, glm::vec3 _vfPlanePoint);

		// Keyboard
		void KeyDown(unsigned char _cKey, int _iX, int _iY);
		void KeyUp(unsigned char _cKey, int _iX, int _iY);
		unsigned int GetKeyState(unsigned char _ucKey)const;
		std::vector<unsigned int> GetPlayerMovement(bool _bPlayer1);
		void SpecialKeyDown(int _iKey, int _iX, int _iY);
		void SpecialKeyUp(int _iKey, int _iX, int _iY);

		void MakePressedOrReleased();


};

#endif // !__INPUT_H__
