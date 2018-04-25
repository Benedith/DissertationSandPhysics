#include "SystemClass.h"
#include "Constants.h"
#include <AntTweakBar.h>
class thread;
bool SystemClass::initialize()
{
	int screen_width = 0;
	int screen_height = 0;

	
	initWindows(screen_width, screen_height);

	if (!m_input_class)
		return false;

	m_input_class->initialize();

	if (!m_graphics_class)
		return false;

	if(!m_graphics_class->initialize(screen_width, screen_height, m_h_window))
		return false;
	

	return true;
}



void SystemClass::shutdown()
{
	shutdownWindows();
}



void SystemClass::run()
{
	MSG msg;
	bool done = false;
	bool result = false;
	
	ZeroMemory(&msg, sizeof(MSG));

	while (!done)
	{
		
		
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			if (!frame())
			{
				done = true;
			}
		}
	}
}



LRESULT SystemClass::messageHandler(HWND h_window, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		InputClass::HandleMouseEvent(message, wparam, lparam);
		break;
		return 0;
	case WM_KEYDOWN:
		m_input_class->keyDown((unsigned int)wparam);
		break;

	case WM_KEYUP:
		m_input_class->keyUp((unsigned int)wparam);
		break;

	default:
		return DefWindowProc(h_window, message, wparam, lparam);
		break;
	}

	return 0;
}



bool SystemClass::frame()
{
	if (m_input_class->getKeyDown(VK_ESCAPE))
		return false;

	if (m_input_class->getKeyDown(VK_SPACE))
	{
		moveCamera(0.0f, 0.5f);
	}
	if (m_input_class->getKeyDown(VK_NUMPAD2))
	{
		moveCamera(0.0f, -0.5f);
	}
	if (m_input_class->getKeyDown(VK_LEFT))
	{
		moveCamera(-0.5f);
	}
	if (m_input_class->getKeyDown(VK_RIGHT))
	{
		moveCamera(0.5f);
	}
	if (m_input_class->getKeyDown(VK_NUMPAD1))
	{
		rotateCamera(1.0f, 0.0f);
	}
	if (m_input_class->getKeyDown(VK_NUMPAD3))
	{
		rotateCamera(0.0f, -1.0f);
	}
	if ((m_input_class->getKeyDown(VK_UP)))
	{
		moveCamera(0.0f, 0.0f, 0.5f);
	}
	if ((m_input_class->getKeyDown(VK_DOWN)))
	{
		moveCamera(0.0f, 0.0f, -0.5f);
	}
	if(!m_graphics_class->frame())
		return false;

	return true;
}



void SystemClass::moveCamera(float translation_x, float translation_y, float translation_z)
{
	Camera* temp_cam = m_graphics_class->getCamera();

	DirectX::XMFLOAT3 cam_position = temp_cam->getPosition();
	DirectX::XMFLOAT3 new_cam_position = DirectX::XMFLOAT3(cam_position.x + translation_x,
															cam_position.y + translation_y,
															cam_position.z + translation_z);

	m_graphics_class->getCamera()->setPosition(new_cam_position);

}



void SystemClass::rotateCamera(float translation_x, float translation_y, float translation_z)
{
	/*Camera* temp_cam = m_graphics_class->getCamera();*/
	DirectX::XMFLOAT3 cam_rotation = m_graphics_class->getCamera()->getRotation();
	DirectX::XMFLOAT3 new_cam_rotation = DirectX::XMFLOAT3(cam_rotation.x + translation_x,
															cam_rotation.y + translation_y,
															cam_rotation.z + translation_z);

	m_graphics_class->getCamera()->setRotation(new_cam_rotation);
}



void SystemClass::initWindows(int& screen_width, int& screen_height)
{
	WNDCLASSEX wnd_cls;
	DEVMODE dev_mode_screen_settings;
	int pos_x;
	int pos_y;

	ApplicationHandle = this;
	
	m_h_instance = GetModuleHandle(NULL);
	m_application_name = "Sand Simulation";

	wnd_cls.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wnd_cls.lpfnWndProc = wndProc;
	wnd_cls.cbClsExtra = 0;
	wnd_cls.cbWndExtra = 0;
	wnd_cls.hInstance = m_h_instance;
	wnd_cls.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wnd_cls.hIconSm = wnd_cls.hIcon;
	wnd_cls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd_cls.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wnd_cls.lpszMenuName = NULL;
	wnd_cls.lpszClassName = m_application_name;
	wnd_cls.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wnd_cls);

	screen_width = GetSystemMetrics(SM_CXSCREEN);
	screen_height = GetSystemMetrics(SM_CYSCREEN);

	if (Constant::FULL_SCREEN)
	{
		memset(&dev_mode_screen_settings, 0, sizeof(dev_mode_screen_settings));
		dev_mode_screen_settings.dmSize = sizeof(dev_mode_screen_settings);
		dev_mode_screen_settings.dmPelsWidth = (unsigned long)screen_width;
		dev_mode_screen_settings.dmPelsHeight = (unsigned long)screen_height;
		dev_mode_screen_settings.dmBitsPerPel = 32;
		dev_mode_screen_settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dev_mode_screen_settings, CDS_FULLSCREEN);
		
		pos_x = 0;
		pos_y = 0;
	}
	else
	{
		screen_width = 800;
		screen_height = 600;

		pos_x = (GetSystemMetrics(SM_CXSCREEN) - screen_width) / 2;
		pos_y = (GetSystemMetrics(SM_CYSCREEN) - screen_height) / 2;
	}

	m_h_window = CreateWindowEx(WS_EX_APPWINDOW, m_application_name, m_application_name,
								WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
								pos_x, pos_y, screen_width, screen_height, NULL, NULL,
								m_h_instance, NULL);

	ShowWindow(m_h_window, SW_SHOW);
	SetForegroundWindow(m_h_window);
	SetFocus(m_h_window);

	ShowCursor(true);
}



void SystemClass::shutdownWindows()
{
	ShowCursor(true);

	if (Constant::FULL_SCREEN)
		ChangeDisplaySettings(NULL, 0);

	DestroyWindow(m_h_window);
	m_h_window = NULL;

	UnregisterClass(m_application_name, m_h_instance);
	m_h_instance = NULL;


	if (ApplicationHandle)
		delete ApplicationHandle;

	ApplicationHandle = nullptr;
}



LRESULT CALLBACK wndProc(HWND h_window, UINT message, WPARAM wparam, 
				LPARAM lparam)
{
	if (TwEventWin(h_window, message, wparam, lparam))
		return 0; 
	switch (message)
	{
	case WM_MOUSEMOVE:
		InputClass::HandleMouseEvent(message, wparam, lparam);
		break;
		return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		default:
			return ApplicationHandle->messageHandler(h_window, message, wparam, lparam);
			break;
	}

	return 0;
}

