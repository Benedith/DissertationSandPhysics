#include "InputClass.h"

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
