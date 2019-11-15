#pragma once

const unsigned int MAX_KEYS = 256;

class InputClass
{
public:
	void Initialize();
	void Shutdown();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

	InputClass();
	InputClass(const InputClass&);
	~InputClass();

private:
	bool m_keys[MAX_KEYS];
};