#pragma once
#include <Windows.h>
#include "Bmath.h"
using namespace BMath;
enum KeyCode
{
	//None = 0,
	Mouse0 = VK_LBUTTON,
	Mouse1 = VK_RBUTTON,
	Backspace = VK_BACK,
	Delete = VK_DELETE,
	Tab = VK_TAB,
	Clear = VK_CLEAR,
	Return = VK_RETURN,
	LeftShift = VK_SHIFT,
	Pause = VK_PAUSE,
	EscapeKey = VK_ESCAPE,
	Space = VK_SPACE,
	upArrow = VK_UP,
	downArrow = VK_DOWN,

	A = 0x41,
	B = 0x42,
	C = 0x43,
	D = 0x44,
	E = 0x45,
	F = 0x46,
	G = 0x47,
	H = 0x48,
	I = 0x49,
	J = 0x4A,
	K = 0x4B,
	L = 0x4C,
	M = 0x4D,
	N = 0x4E,
	O = 0x4F,
	P = 0x50,
	Q = 0x51,
	R = 0x52,
	S = 0x53,
	T = 0x54,
	U = 0x55,
	V = 0x56,
	W = 0x57,
	X = 0x58,
	Y = 0x59,
	Z = 0x5A,

	END_KEYS
};
static bool KeyboardState[KeyCode::END_KEYS];
static bool PreviousKeyboardState[KeyCode::END_KEYS];

static Vector2 MousePosition;
static Vector2 PreviousMousePosition;

static bool mouseMoved = false;
class InputClass
{
public:
	InputClass() = default;
	~InputClass() = default;

	void initialize();

	void keyDown(unsigned int);
	void keyUp(unsigned int);

	bool getKeyDown(unsigned int);
	static void HandleMouseEvent(UINT msg, WPARAM wparam, LPARAM lparam);

	static bool MouseMoved();

	static Vector2 GetMousePosition();
	static Vector2 GetPreviousMousePosition();

	static void SetMousePosition(int x, int y);

private:
	bool m_keys[256];
};