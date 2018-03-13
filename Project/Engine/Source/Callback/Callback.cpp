
#include "Platform/Source/Application.h"
#include "Callback/Callback.h"
#include "Common/IncludesMatem.h"

Callback *Callback::_hintObject = 0;

bool Callback::_tap = false;
bool Callback::_wasLongTap = false;
double Callback::_time = 0;
double Callback::_currentTime = 0;
glm::vec2 Callback::_pos;
glm::vec2 Callback::_vector;

bool Callback::_key[256];
int Callback::_pressButtons = 0;
char Callback::_charButtonDown;
char Callback::_charButtonUp;

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
		break;

	case BUTTON_DOWN:
		_buttonDown = function;
		break;
	case BUTTON_PINCH:
		_buttonPinch = function;
		break;
	case BUTTON_UP:
		_buttonUp = function;
		
	}

	_hasCallback = true;
}

void Callback::reset()
{
	for (int i = 0; i < 256; ++i)
	{
		_key[i] = false;
	}

	_charButtonDown = '\0';
	_charButtonUp = '\0';

	buttonUp('\0');
}

void Callback::tap_down()
{
	if (_tap) return;
	_tap = true;
	_vector.x = 0.0f;
	_vector.y = 0.0f;
	_wasLongTap = false;

	double currentTime = App::getCurentTime();
	_time = currentTime - _currentTime;
	_time /= 1000;

	if (_time < 0.3) tap_double();

	_currentTime = currentTime;
	_time = 0;

	if (_hintObject && _hintObject->_tap_down)
	{
		_hintObject->_tap_down(_hintObject);
	}
}

void Callback::tap_pinch()
{
	if (!_tap) return;

	double currentTime = App::getCurentTime();
	_time = currentTime - _currentTime;
	_time /= 1000;

	if (_hintObject && _hintObject->_tap_pinch)
	{
		_hintObject->_tap_pinch(_hintObject);
	}

	if (!_wasLongTap && _time > 1.0) tap_long();
}

void Callback::tap_up()
{
	if (!_tap) return;
	_tap = false;

	double currentTime = App::getCurentTime();
	_time = currentTime - _currentTime;
	_time /= 1000;

	if (_hintObject && _hintObject->_tap_up)
	{
		_hintObject->_tap_up(_hintObject);
	}

	_time = 0;
}

void Callback::tap_long()
{
	_wasLongTap = true;

	if (_hintObject && _hintObject->_tap_long)
	{
		_hintObject->_tap_long(_hintObject);
	}
}

void Callback::tap_double()
{
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

void Callback::buttonDown(char charVar)
{
	if (_key[charVar] == true) return;
	Callback::_key[charVar] = true;
	_charButtonDown = charVar;
	++_pressButtons;

	if (_hintObject && _hintObject->_buttonDown)
	{
		_hintObject->_buttonDown(_hintObject);
	}

	_charButtonDown = '\0';
}

void Callback::buttonPinch()
{
	if (_pressButtons > 0 && _hintObject && _hintObject->_buttonPinch)
	{
		_hintObject->_buttonPinch(_hintObject);
	}
}

void Callback::buttonUp(char charVar)
{
	if (_key[charVar] == false) return;
	Callback::_key[charVar] = false;
	_charButtonUp = charVar;
	--_pressButtons;

	if (_hintObject && _hintObject->_buttonUp)
	{
		_hintObject->_buttonUp(_hintObject);
	}

	_charButtonUp = '\0';
}
