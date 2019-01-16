#include "VGTime.h"

const int fps_limit = 200;

VGTime::VGTime() : start_(0), paused_(0), idle_(0), current_(0), previous_(0), total_(0), delta_(0), is_paused_(false)
{
	long long int frequency;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));

	seconds_per_tick_ = 1.0 / double(frequency);
}


VGTime::~VGTime()
{
}

double VGTime::delta_time() const
{
	return delta_;
}

double VGTime::total_time() const
{
	return total_;
}

void VGTime::start()
{
	if (QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&start_)))
	{
		current_ = start_;
		previous_ = current_;
		paused_ = 0;
		is_paused_ = false;
	}
}

void VGTime::tick()
{
	if (is_paused_)
	{
		delta_ = 0.0;
	}
	else
	{
		if (QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&current_)))
		{

			delta_ = (current_ - previous_) * seconds_per_tick_;
			previous_ = current_;

			if (delta_ < 0.0)
				delta_ = 0.0;

			total_ = (current_ - start_ - idle_)*seconds_per_tick_;
		}
	}
}

void VGTime::pause()
{
	if (QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&paused_)))
	{
		is_paused_ = true;
		total_ = (paused_ - start_ - idle_)*seconds_per_tick_;
	}
}

void VGTime::resume()
{
	if (QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&current_)))
	{
		idle_ += current_ - paused_;
		previous_ = current_;
		paused_ = 0;

		is_paused_ = false;
	}
}

void VGTime::stop()
{
	if (!is_paused_)
	{
		total_ = static_cast<double> (current_ - start_) - idle_;
		is_paused_ = true;
	}
}

void VGTime::reset()
{
	if (QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&current_)))
	{
		start_ = current_;
		previous_ = current_;
		paused_ = 0;
		idle_ = 0;
		total_ = 0;
		is_paused_ = false;
	}
}

int VGTime::get_fps() const
{
	static double time_since_last_frame;
	static int frames;
	frames++;
	
	//if (total_time() - time_since_last_frame >= 1.0f)
	//{
	//	fps_ = frames;
	//	frames = 0;
	//	time_since_last_frame += 1.0;
	//}
	return min((int(1.0 / delta_time())), fps_limit); // +fps_) / 2;
}
