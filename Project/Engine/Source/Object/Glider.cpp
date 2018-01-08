
#include "Glider.h"

GliderTemplate GliderTemplate::_template;

Glider::Glider()
{
	_template = &GliderTemplate::_template;

	_speedHeight = _template->_speedHeight;
	_height = (_template->_minHeight + _template->_maxHeight) * 0.5f;
}

Glider::~Glider()
{

}

void Glider::action()
{
	if (_height > _template->_maxHeight)
	{
		_speedHeight = -_template->_speedHeight;
	}

	if (_height < _template->_minHeight)
	{
		_speedHeight = _template->_speedHeight;
	}

	_height += _speedHeight;
	setHeight(_height);
}
