#pragma once

#include "Engine/Source/Common/IncludesMatem.h"
#include <functional>

#define CallbackFunction std::function<bool(void *data)>
#define UiFunction(metod) [this](void *data) { return this->metod(data); }

enum EventCallback
{
	TAP_DOWN,
	TAP_PINCH,
	TAP_UP,
	TAP_LONG,
	TAP_DOUBLE,
	MOVE,
	HINT,

	BUTTON_DOWN,
	BUTTON_UP
};

class Callback
{
private:
	bool _hasCallback = false;

	CallbackFunction _tap_down = 0;
	CallbackFunction _tap_pinch = 0;
	CallbackFunction _tap_up = 0;
	CallbackFunction _tap_long = 0;
	CallbackFunction _tap_double = 0;
	CallbackFunction _move = 0;
	CallbackFunction _hint = 0;

	CallbackFunction _buttonDown = 0;
	CallbackFunction _buttonUp = 0;

	static bool _wasLongTap;
	static double _currentTime;

public:
	static Callback *_hintObject;
	static bool _tap;
	
	static double _time;
	static glm::vec2 _pos;
	static glm::vec2 _vector;

	static char _charButtonDown;
	static char _charButtonUp;

public:
	const bool &hasCallback() { return _hasCallback; };
	void setCallback(const EventCallback &event, const CallbackFunction &function);

public:
	static bool tap() { return _tap; }
	static double time() { return _time; }

	static void tap_down();
	static void tap_pinch();
	static void tap_up();
	static void tap_long();
	static void tap_double();
	static void move(const float *pos);
	static void hint();

	static void buttonDown(char charVar);
	static void buttonUp(char charVar);
};