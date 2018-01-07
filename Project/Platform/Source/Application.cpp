
#include "Application.h"
#include "ApplicationPlatform.h"

int Application::width()
{
	return ApplicationPlatform::width();
}

int Application::height()
{
	return ApplicationPlatform::height();
}

float Application::aspect()
{
	return ApplicationPlatform::aspect();
}

double Application::getCurentTime()
{
	return ApplicationPlatform::getCurentTime();
}

void Application::close()
{
	return ApplicationPlatform::close();
}
