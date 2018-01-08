
#include "Application.h"
#include "ApplicationPlatform.h"

int App::width()
{
	return ApplicationPlatform::width();
}

int App::height()
{
	return ApplicationPlatform::height();
}

float App::aspect()
{
	return ApplicationPlatform::aspect();
}

double App::getCurentTime()
{
	return ApplicationPlatform::getCurentTime();
}

void App::close()
{
	return ApplicationPlatform::close();
}
