#pragma once

#include <stdint.h>
#include <functional>

class TimeNotifier
{
	int64_t _timeout = 0;
	std::function<void()> _func;
	int64_t _lastTick = 0;
public:
	TimeNotifier()
	{
	}

	TimeNotifier(int64_t timeout, std::function<void()> func)
		: _timeout(timeout)
		, _func(func)
	{
	}
public:
	void setArg(int64_t timeout, std::function<void()> func)
	{
		_timeout = timeout;
		_func = func;
	}

	void reset(int64_t currTick)
	{
		_lastTick = currTick;
	}

	void doTimeout(int64_t currTick)
	{
		if(qAbs(currTick - _lastTick) > _timeout)
		{
			if(_func)
				_func();
			_lastTick = currTick;
		}
	}
};
