#include <memory>

#include "SystemClass.h"

int WINAPI WinMain (HINSTANCE h_instance, HINSTANCE h_previous_instance,
					PSTR p_string_command_line, int i_command_show)
{
	std::unique_ptr<SystemClass> system = std::make_unique<SystemClass>();

	if (!system)
		return 0;

	bool result = system->initialize();
	if (result)
		system->run();

	system->shutdown();
	return 0;
}