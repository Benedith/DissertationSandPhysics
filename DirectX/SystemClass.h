#pragma once

//Definitions
#define WIN32_LEAN_AND_MEAN

//System includes
#include <Windows.h>
#include <memory>

//Custom Includes
#include "InputClass.h"
#include "GraphicsClass.h"
#include <time.h>

class SystemClass
{
public:
	SystemClass() = default;
	~SystemClass() = default;

	bool initialize();
	void shutdown();
	void run();
	LRESULT CALLBACK messageHandler(HWND h_window, UINT u_int, 
									WPARAM w_parameter, LPARAM l_parameter);

private:
	bool frame();
	void moveCamera(float translation_x = 0.0f, float translation_y = 0.0f, float translation_z = 0.0f);
	void rotateCamera(float translation_x = 0.0f, float translation_y = 0.0f, float translation_z = 0.0f);
	void initWindows(int& width, int& height);
	void shutdownWindows();

	LPSTR m_application_name;
	HINSTANCE m_h_instance;
	HWND m_h_window;

	std::unique_ptr<InputClass> m_input_class = std::make_unique<InputClass>();
	std::unique_ptr<GraphicsClass> m_graphics_class = std::make_unique<GraphicsClass>();
	void CreateConsole();
};

static LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;