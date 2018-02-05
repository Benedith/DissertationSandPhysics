#pragma once


class InputClass
{
public:
	InputClass() = default;
	~InputClass() = default;

	void initialize();

	void keyDown(unsigned int);
	void keyUp(unsigned int);

	bool getKeyDown(unsigned int);

private:
	bool m_keys[256];
};