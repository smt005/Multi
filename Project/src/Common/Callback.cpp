
#include "Callback.h"
#include "IncludesMatem.h"
#include "Control.h"
#include "Application.h"

#ifdef WIN32
#include <windows.h>
#endif

namespace ñallback
{
	SYSTEMTIME _systemTime;
	double _currentTime = 0;
	bool _wasLongTap = false;
};

Callback *Callback::_hintObject = 0;
bool Callback::_tap = false;
double Callback::_time = 0;
glm::vec2 Callback::_pos;
glm::vec2 Callback::_vector;

void Callback::setCallback(const EventCallback &event, const CallbackFunction &function)
{
	switch (event)
	{
	case TAP_DOWN:
		_tap_down = function;
		break;
	case TAP_PINCH:
		_tap_pinch = function;
		break;
	case TAP_UP:
		_tap_up = function;
		break;
	case TAP_LONG:
		_tap_long = function;
		break;
	case TAP_DOUBLE:
		_tap_double = function;
		break;
	case MOVE:
		_move = function;
		break;
	case HINT:
		_hint = function;
	}

	_hasCallback = true;
}

void Callback::tap_down()
{
	if (_tap) return;
	_tap = true;
	_vector.x = 0.0f;
	_vector.y = 0.0f;
	ñallback::_wasLongTap = false;

	GetLocalTime(&ñallback::_systemTime);
	int currentTime = (ñallback::_systemTime.wMinute * 60000) + (ñallback::_systemTime.wSecond * 1000) + ñallback::_systemTime.wMilliseconds;
	_time = static_cast<double>(currentTime) - ñallback::_currentTime;
	_time /= 1000;

	if (_time < 0.3) tap_double();

	ñallback::_currentTime = currentTime;
	_time = 0;

	//Log("%s", "tap_bown");

	if (_hintObject && _hintObject->_tap_down)
	{
		_hintObject->_tap_down(_hintObject);
	}
}

void Callback::tap_pinch()
{
	if (!_tap) return;

	GetLocalTime(&ñallback::_systemTime);
	int currentTime = (ñallback::_systemTime.wMinute * 60000) + (ñallback::_systemTime.wSecond * 1000) + ñallback::_systemTime.wMilliseconds;
	_time = static_cast<double>(currentTime) - ñallback::_currentTime;
	_time /= 1000;

	if (_hintObject && _hintObject->_tap_pinch)
	{
		//Log("%s%l", "tap_pinch _time = ", _time);
		_hintObject->_tap_pinch(_hintObject);
	}

	if (!ñallback::_wasLongTap && _time > 1.0) tap_long();
}

void Callback::tap_up()
{
	if (!_tap) return;
	_tap = false;

	GetLocalTime(&ñallback::_systemTime);
	int currentTime = (ñallback::_systemTime.wMinute * 60000) + (ñallback::_systemTime.wSecond * 1000) + ñallback::_systemTime.wMilliseconds;
	_time = static_cast<double>(currentTime) - ñallback::_currentTime;
	_time /= 1000;

	//Log("%s%l", "tap_up _time = ", _time);
	
	if (_hintObject && _hintObject->_tap_up)
	{
		_hintObject->_tap_up(_hintObject);
	}

	_time = 0;
}

void Callback::tap_long()
{
	ñallback::_wasLongTap = true;
	//Log("%s%l", "tap_long_tap _time = ", _time);

	if (_hintObject && _hintObject->_tap_long)
	{
		_hintObject->_tap_long(_hintObject);
	}
}

void Callback::tap_double()
{
	//Log("%s", "\ttap_double");

	if (_hintObject && _hintObject->_tap_double)
	{
		_hintObject->_tap_double(_hintObject);
	}
}

void Callback::move(const float *pos)
{
	if (_pos.x == pos[0] && _pos.y == pos[1])
	{
		_vector.x = 0.0;
		_vector.y = 0.0;
		return;
	}

	_vector.x = (float)(_pos.x - pos[0]);
	_vector.y = (float)(_pos.y - pos[1]);
	_pos = glm::vec2(pos[0], pos[1]);

	if (_hintObject && _hintObject->_move)
	{
		//LOG_LINE _vector.x << '\t' << _vector.y <<  '\t' << lenght LOG_END
		_hintObject->_move(_hintObject);
	}
}

void Callback::hint()
{
	/*_hintObject = dynamic_cast<Callback*>(UiObject::hint());

	if (_hintObject && _hintObject->_hint)
	{
		_hintObject->_hint(_hintObject);
	}*/
}
