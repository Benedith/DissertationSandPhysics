#include "InputClass.h"
#include <windowsx.h>

void InputClass()
{}
void InputClass::initialize()
{
	for (int i = 0; i < 256; i++, m_keys[i] = false);
}

void InputClass::keyDown(unsigned int input)
{
	m_keys[input] = true;
}

void InputClass::keyUp(unsigned int input)
{
	m_keys[input] = false;
}

bool InputClass::getKeyDown(unsigned int key)
{
	return m_keys[key];
}

void InputClass::HandleMouseEvent(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		KeyboardState[Mouse0] = true;
		break;
	case WM_RBUTTONDOWN:
		KeyboardState[Mouse1] = true;
		break;
	case WM_LBUTTONUP:
		KeyboardState[Mouse0] = false;
		break;
	case WM_RBUTTONUP:
		KeyboardState[Mouse1] = false;
		break;
	case WM_MOUSEMOVE:
		mouseMoved = true;
		PreviousMousePosition = MousePosition;
		MousePosition.x = GET_X_LPARAM(lparam);
		MousePosition.y = GET_Y_LPARAM(lparam);
		break;


	}
}
	Vector2 InputClass::GetMousePosition()
	{
		return MousePosition;
	}

	Vector2 InputClass::GetPreviousMousePosition()
	{
		return PreviousMousePosition;
	}

	bool InputClass::MouseMoved()
	{
		if (mouseMoved)
		{
			mouseMoved = false;
			return true;
		}
		return false;
	}

	void InputClass::SetMousePosition(int x, int y)
	{
		MousePosition.x = x;
		MousePosition.y = y;
		PreviousMousePosition.x = x;
		PreviousMousePosition.y = y;
		mouseMoved = false;
	}

