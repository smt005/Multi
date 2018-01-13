
#include "ApplicationPlatform.h"

int WINAPI WinMain(HINSTANCE applicationInstance, HINSTANCE previousInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!ApplicationPlatform::execution(applicationInstance))
	{
		return -1;
	}

	return 0;
}
