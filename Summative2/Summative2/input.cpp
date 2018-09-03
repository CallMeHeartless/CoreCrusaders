/***********************
Bachelor of Software Engineering   Media Design School   Auckland   New Zealand
(c) 2018 Media Design School
File Name   :	input.cpp
Description :	CInput class implementation file
Author      :   Kerry Pellett BE(Hons)
Mail        :   kerry.pel7420@mediadesign.school.nz
********************/

// Local includes
#include "input.h"
#include "camera.h"

// Static instance
CInput* CInput::s_pInstance = nullptr;

CInput::CInput(){}

CInput::~CInput(){}

/***********************
* GetInstance: Returns the singleton input instance, creating it if it does not exist
* @author: Kerry Pellett (2018)
* @parameter: void 
* @return: CInput* (s_pInstance)
********************/
CInput* CInput::GetInstance() {
	if (!s_pInstance) {
		s_pInstance = new CInput();
	}

	return s_pInstance;
}

/***********************
* DestroyInstance: Destroys the singleton instance
* @author: Kerry Pellett (2018)
* @parameter: void
* @return: void
********************/
void CInput::DestroyInstance() {
	delete s_pInstance;
	s_pInstance = nullptr;
}

/***********************
* ProcessMouseButton: Auxilliary function for OpenGL mouse buttons
* @author: Kerry Pellett (2018)
* @parameter:
* @return: void
********************/
void ProcessMouseButton(int _iButton, int _iButtonState, int _iX, int _iY) {
	CInput::GetInstance()->MouseButton(_iButton, _iButtonState, _iX, _iY);
}

void CInput::MouseButton(int _iButton, int _iButtonState, int _iX, int _iY) {
	if (_iButton < 3) {
		if (_iButtonState == GLUT_DOWN) {

			m_uiMouseState[_iButton] = INPUT_FIRST_PRESSED;
		}
		else if (_iButtonState == GLUT_UP) {

			m_uiMouseState[_iButton] = INPUT_FIRST_RELEASED;
		}
	}

	m_vfMousePosition = glm::vec2(_iX, _iY);
}

unsigned int CInput::GetMouseButton(EINPUTMOUSE _eMouseButton)const {
	return m_uiMouseState[_eMouseButton];
}

void ProcessMouseMovement(int _iX, int _iY) {
	CInput::GetInstance()->MouseMovement(_iX, _iY);
}

void CInput::MouseMovement(int _iX, int _iY) {
	m_vfMousePosition = glm::vec2(_iX, _iY);
}

void ProcessKeyDown(unsigned char _cKey, int _iX, int _iY) {
	CInput::GetInstance()->KeyDown(_cKey, _iX, _iY);
}

bool CInput::KeyDown(unsigned char _cKey)
{
	switch (m_uiKeyState[_cKey]) {
	case INPUT_FIRST_PRESSED: {
		return true;
		break;
	}
	case INPUT_PRESSED: {
		return true;
		break;
	}
	default:
		break;
	}
	return false;
}

void CInput::KeyDown(unsigned char _cKey, int _iX, int _iY) {

	switch (m_uiKeyState[_cKey]) {
		case INPUT_FIRST_RELEASED: {
			// Fall through
		}
	
		case INPUT_RELEASED: {
			m_uiKeyState[_cKey] = INPUT_FIRST_PRESSED;
			break;
		}

		case INPUT_FIRST_PRESSED: {
			m_uiKeyState[_cKey] = INPUT_PRESSED;
			break;
		}

		default:break;
	}

	m_vfMousePosition = glm::vec2(_iX, _iY);
}

void ProcessKeyUp(unsigned char _ucKey, int _iX, int _iY) {
	CInput::GetInstance()->KeyUp(_ucKey, _iX, _iY);
}

void CInput::KeyUp(unsigned char _ucKey, int _iX, int _iY) {
	switch (m_uiKeyState[_ucKey]) {
		case INPUT_FIRST_PRESSED: {
			// Fall through
		}
		case INPUT_PRESSED: {
			m_uiKeyState[_ucKey] = INPUT_FIRST_RELEASED;
			break;
		}

		case INPUT_FIRST_RELEASED: {
			m_uiKeyState[_ucKey] = INPUT_RELEASED;
		}
	}

	m_vfMousePosition = glm::vec2(_iX, _iY);
}

void ProcessSpecialKeyDown(int _iKey, int _iX, int _iY) {
	CInput::GetInstance()->SpecialKeyDown(_iKey, _iX, _iY);
}

void CInput::SpecialKeyDown(int _iKey, int _iX, int _iY) {
	// Determine index based on arrow key
	int iIndex;
	switch (_iKey) {
		case GLUT_KEY_UP: {
			iIndex = 0;
			break;
		}

		case GLUT_KEY_LEFT: {
			iIndex = 1;
			break;
		}

		case GLUT_KEY_DOWN: {
			iIndex = 2;
			break;
		}

		case GLUT_KEY_RIGHT: {
			iIndex = 3;
			break;
		}

		default:return;
	}

	// Process key down
	switch (m_uiArrowKeyState[iIndex]) {
		case INPUT_FIRST_RELEASED: {
			// Fall through
		}

		case INPUT_RELEASED: {
			m_uiArrowKeyState[iIndex] = INPUT_FIRST_PRESSED;
			break;
		}

		case INPUT_FIRST_PRESSED: {
			m_uiArrowKeyState[iIndex] = INPUT_PRESSED;
			break;
		}

		default:break;
	}

	m_vfMousePosition = glm::vec2(_iX, _iY);

}

void ProcessSpecialKeyUp(int _iKey, int _iX, int _iY) {
	CInput::GetInstance()->SpecialKeyUp(_iKey, _iX, _iY);
}

void CInput::SpecialKeyUp(int _iKey, int _iX, int _iY) {
	// Determine index based on arrow key
	int iIndex;
	switch (_iKey) {
		case GLUT_KEY_UP: {
			iIndex = 0;
			break;
		}

		case GLUT_KEY_LEFT: {
			iIndex = 1;
			break;
		}

		case GLUT_KEY_DOWN: {
			iIndex = 2;
			break;
		}

		case GLUT_KEY_RIGHT: {
			iIndex = 3;
			break;
		}

		default:
			return;
			break;
	}

	// Process key down
	switch (m_uiArrowKeyState[iIndex]) {
		case INPUT_FIRST_PRESSED: {
			// Fall through
		}

		case INPUT_PRESSED: {
			m_uiArrowKeyState[iIndex] = INPUT_FIRST_RELEASED;
			break;
		}

		case INPUT_FIRST_RELEASED: {
			m_uiArrowKeyState[iIndex] = INPUT_RELEASED;
			break;
		}

		default:break;
	}

	m_vfMousePosition = glm::vec2(_iX, _iY);
}

unsigned int CInput::GetKeyState(unsigned char _ucKey)const {
	return m_uiKeyState[_ucKey];
}

void CInput::Initialise() {
	// Mouse 
	glutMouseFunc(ProcessMouseButton);
	glutPassiveMotionFunc(ProcessMouseMovement);
	// Keyboard buttons
	glutKeyboardFunc(ProcessKeyDown);
	glutKeyboardUpFunc(ProcessKeyUp);
	glutSpecialFunc(ProcessSpecialKeyDown);
	glutSpecialUpFunc(ProcessSpecialKeyUp);

	glutIgnoreKeyRepeat(1);
}

std::vector<unsigned int> CInput::GetPlayerMovement(bool _bIsPlayer1) {
	std::vector<unsigned int> vecPlayerMovement(4);
	if (_bIsPlayer1) {
		vecPlayerMovement[0] = ((m_uiKeyState['W'] == INPUT_PRESSED || 
								 m_uiKeyState['w'] == INPUT_PRESSED) 
								 ? INPUT_PRESSED : INPUT_RELEASED);
		vecPlayerMovement[1] = ((m_uiKeyState['a'] == INPUT_PRESSED ||
								m_uiKeyState['A'] == INPUT_PRESSED)
								? INPUT_PRESSED : INPUT_RELEASED);
		vecPlayerMovement[2] = ((m_uiKeyState['s'] == INPUT_PRESSED ||
								m_uiKeyState['S'] == INPUT_PRESSED)
								? INPUT_PRESSED : INPUT_RELEASED);
		vecPlayerMovement[3] = ((m_uiKeyState['d'] == INPUT_PRESSED ||
								m_uiKeyState['D'] == INPUT_PRESSED)
								? INPUT_PRESSED : INPUT_RELEASED);
	}
	else {
		vecPlayerMovement[0] = m_uiArrowKeyState[0];
		vecPlayerMovement[1] = m_uiArrowKeyState[1];
		vecPlayerMovement[2] = m_uiArrowKeyState[2];
		vecPlayerMovement[3] = m_uiArrowKeyState[3];
	}
	return vecPlayerMovement;
}

glm::vec2 CInput::GetMousePosition()const {
	return m_vfMousePosition;
}

void CInput::MakePressedOrReleased() {
	// Mouse
	for (unsigned int i = 0; i < 3; ++i) {
		switch (m_uiMouseState[i]) {
			case INPUT_FIRST_PRESSED:{
				m_uiMouseState[i] = INPUT_PRESSED;
				break;
			}

			case INPUT_FIRST_RELEASED: {
				m_uiMouseState[i] = INPUT_RELEASED;
				break;
			}

			default:break;
		}
	}

	// WASD
	switch (m_uiKeyState['w']) {
		case INPUT_FIRST_PRESSED: {
			m_uiKeyState['w'] = INPUT_PRESSED;
			break;
		}

		case INPUT_FIRST_RELEASED: {
			m_uiKeyState['w'] = INPUT_RELEASED;
			break;
		}

		default:break;
	}
	switch (m_uiKeyState['W']) {
	case INPUT_FIRST_PRESSED: {
		m_uiKeyState['W'] = INPUT_PRESSED;
		break;
	}

	case INPUT_FIRST_RELEASED: {
		m_uiKeyState['W'] = INPUT_RELEASED;
		break;
	}

	default:break;
	}
	switch (m_uiKeyState['a']) {
		case INPUT_FIRST_PRESSED: {
			m_uiKeyState['a'] = INPUT_PRESSED;
			break;
		}

		case INPUT_FIRST_RELEASED: {
			m_uiKeyState['a'] = INPUT_RELEASED;
			break;
		}

		default:break;
	}
	switch (m_uiKeyState['A']) {
	case INPUT_FIRST_PRESSED: {
		m_uiKeyState['A'] = INPUT_PRESSED;
		break;
	}

	case INPUT_FIRST_RELEASED: {
		m_uiKeyState['A'] = INPUT_RELEASED;
		break;
	}

	default:break;
	}
	switch (m_uiKeyState['s']) {
		case INPUT_FIRST_PRESSED: {
			m_uiKeyState['s'] = INPUT_PRESSED;
			break;
		}

		case INPUT_FIRST_RELEASED: {
			m_uiKeyState['s'] = INPUT_RELEASED;
			break;
		}

		default:break;
	}
	switch (m_uiKeyState['S']) {
	case INPUT_FIRST_PRESSED: {
		m_uiKeyState['S'] = INPUT_PRESSED;
		break;
	}

	case INPUT_FIRST_RELEASED: {
		m_uiKeyState['S'] = INPUT_RELEASED;
		break;
	}

	default:break;
	}
	switch (m_uiKeyState['d']) {
		case INPUT_FIRST_PRESSED: {
			m_uiKeyState['d'] = INPUT_PRESSED;
			break;
		}

		case INPUT_FIRST_RELEASED: {
			m_uiKeyState['d'] = INPUT_RELEASED;
			break;
		}

		default:break;
	}
	switch (m_uiKeyState['D']) {
	case INPUT_FIRST_PRESSED: {
		m_uiKeyState['D'] = INPUT_PRESSED;
		break;
	}

	case INPUT_FIRST_RELEASED: {
		m_uiKeyState['D'] = INPUT_RELEASED;
		break;
	}

	default:break;
	}
	// Arrow keys
	for (unsigned int i = 0; i < 4; ++i) {
		switch (m_uiArrowKeyState[i]) {
			case INPUT_FIRST_PRESSED: {
				m_uiArrowKeyState[i] = INPUT_PRESSED;
				break;
			}
			case INPUT_FIRST_RELEASED: {
				m_uiArrowKeyState[i] = INPUT_RELEASED;
				break;
			}
			default:break;
		}
	}

}

// Projects from camera to mouse position in world space
glm::vec3 CInput::MouseRay(const CCamera* const _kpCamera) {
	// Obtain coordinates of mouse
	glm::vec2 vfMouse = CInput::GetInstance()->GetMousePosition();
	//vfMouse.y = Utility::SCR_HEIGHT - vfMouse.y;

	// Normalise device coordinates ray
	float fX = (2.0f * vfMouse.x) / (float)Utility::SCR_WIDTH - 1.0f;
	float fY = 1.0f - (2.0f * vfMouse.y) / (float)Utility::SCR_HEIGHT;
	float fZ = 1.0f;
	glm::vec3 vfRay = glm::vec3(fX, fY, fZ);

	// Convert to homongeneous coordinates
	glm::vec4 vfRayClip = glm::vec4(vfRay.x, vfRay.y, -1.0f, 1.0f);

	// Multiply by inverse projection matrix
	glm::vec4 vfRayEye = glm::inverse(_kpCamera->GetProjection()) * vfRayClip;
	// Default values for z and w, as only x and y are unprojected
	vfRayEye.z = -1.0f;
	vfRayEye.w = 0.0f;

	// World coordinates
	glm::vec4 vfRayWorld = (glm::inverse(_kpCamera->GetView()) * vfRayEye);

	// Obtain normalised vector
	vfRay = glm::normalize(glm::vec3(vfRayWorld.x, vfRayWorld.y, vfRayWorld.z));
	return vfRay;
}

glm::vec3 CInput::MouseIntersectionWithPlane(glm::vec3 _vfMouseRay, glm::vec3 _vfCameraPosition, glm::vec3 _vfPlaneNormal, glm::vec3 _vfPlanePoint) {
	float fPlaneDot = glm::dot(_vfPlaneNormal, _vfPlanePoint);
	float fPlaneOffset = (fPlaneDot == 0 ? 0.0f : -fPlaneDot);

	float ft = -((glm::dot(_vfCameraPosition, _vfPlaneNormal) + fPlaneOffset)/ glm::dot(_vfMouseRay, _vfPlaneNormal));

	return _vfMouseRay * ft + _vfCameraPosition;
}