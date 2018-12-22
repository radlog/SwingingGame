#include "VGTime.h"


VGTime::VGTime() : _start(0), _paused(0), _idle(0), _total(0), _current(0), _previous(0)
{
	long long int frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	_seconds_per_tick = 1.0 / (double)frequency;
}


VGTime::~VGTime()
{
}

double VGTime::deltaTime() const
{
	return _delta;
}

double VGTime::totalTime() const
{
	return _total;
}

void VGTime::start()
{
	if (QueryPerformanceCounter((LARGE_INTEGER*)&_start))
	{
		_current = _start;
		_previous = _current;
		_paused = 0;
		_is_paused = false;
	}
	else {
		//return std::runtime_error("Could not get the current time from query performance counter!");
	}


}

void VGTime::tick()
{
	if (_is_paused)
	{
		_delta = 0.0;
	}
	else
	{
		if (QueryPerformanceCounter((LARGE_INTEGER*)&_current))
		{

			_delta = (_current - _previous) * _seconds_per_tick;
			_previous = _current;

			if (_delta < 0.0)
				_delta = 0.0;

			_total = (_current - _start - _idle)*_seconds_per_tick;
		}
		else {
			//return std::runtime_error("Could not get the current time from query performance counter!");
		}
	}


}

void VGTime::pause()
{
	if (QueryPerformanceCounter((LARGE_INTEGER*)&_paused))
	{
		_is_paused = true;
		_total = (_paused - _start - _idle)*_seconds_per_tick;
	}
	else
	{
		//return std::runtime_error("Could not get the current time from query performance counter!");
	}


}

void VGTime::resume()
{
	if (QueryPerformanceCounter((LARGE_INTEGER*)&_current))
	{
		_idle += _current - _paused;
		_previous = _current;
		_paused = 0;

		_is_paused = false;
	}
	else {
		//return std::runtime_error("Could not get the current time from query performance counter!");
	}


}

void VGTime::stop()
{
	if (!_is_paused)
	{
		_total = static_cast<double> (_current - _start) - _idle;
		_is_paused = true;
	}
	else {
		//return std::runtime_error("timer is already stopped!");
	}

}

void VGTime::reset()
{
	if (QueryPerformanceCounter((LARGE_INTEGER*)&_current))
	{
		_start = _current;
		_previous = _current;
		_paused = 0;
		_idle = 0;
		_total = 0;
		_is_paused = false;
	}
	else {
		//return std::runtime_error("Could not get the current time from query performance counter!");
	}


}

int VGTime::getFPS()
{
	static double time_since_last_frame;
	static int frames;
	frames++;
	
	if (totalTime() - time_since_last_frame >= 1.0f)
	{
		fps = frames;
		frames = 0;
		time_since_last_frame += 1.0;
	}
	return ((int)(1.0 / deltaTime()) + fps) / 2;
	//return fps;
}
