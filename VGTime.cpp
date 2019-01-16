#include "VGTime.h"

const int fps_limit = 200;

VGTime::VGTime() : start_(0), paused_(0), idle_(0), current_(0), previous_(0), total_(0), delta_(0), is_paused_(false)
{
	long long int frequency;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency)); // get frequency of the processor

	seconds_per_tick_ = 1.0 / double(frequency); // calculate the seconds per ticks the processor can do
}


VGTime::~VGTime()
{
}

// return delta time
double VGTime::delta_time() const
{
	return delta_;
}

// return total elapsed time
double VGTime::total_time() const
{
	return total_;
}

// start timer
void VGTime::start()
{
	if (QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&start_)))
	{
		current_ = start_; // set current time to start time
		previous_ = current_; // store current time to have the previous time
		paused_ = 0; // paused time is 0
		is_paused_ = false; // timer is not paused
	}
}

// tick timer
void VGTime::tick()
{
	if (is_paused_)
	{
		delta_ = 0.0; // not delta time, if timer is paused
	}
	else
	{
		// if system can get current performance counter, store it in current
		if (QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&current_)))
		{
			// calculate delta time using the difference between two time states 
			delta_ = (current_ - previous_) * seconds_per_tick_; 
			previous_ = current_; // store current to previous for the next tick

			if (delta_ < 0.0) // ensure that delta is not negative
				delta_ = 0.0;

			total_ = (current_ - start_ - idle_)*seconds_per_tick_; // calculate total time
		}
	}
}

// pause timer
void VGTime::pause()
{
	if (QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&paused_)))
	{
		is_paused_ = true; // pause timer
		total_ = (paused_ - start_ - idle_)*seconds_per_tick_; // pause also total time value
	}
}

// resume timer
void VGTime::resume()
{
	if (QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&current_)))
	{
		idle_ += current_ - paused_; // accumulate idle time that cane from pauses
		previous_ = current_; // store current in previous for the next tick
		paused_ = 0; // paused time is 0

		is_paused_ = false; // timer not paused anymore
	}
}

// stop timer
void VGTime::stop()
{
	// can only stop, when timer is not running
	if (!is_paused_)
	{
		total_ = static_cast<double> (current_ - start_) - idle_; // get total time
		is_paused_ = true; // timer is paused
	}
}

// reset timer
void VGTime::reset()
{
	if (QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&current_)))
	{
		start_ = current_; // reset start
		previous_ = current_; // store current in previous for the next tick
		paused_ = 0; // reset paused time
		idle_ = 0; // reset idle time
		total_ = 0; // reset total time
		is_paused_ = false; // timer is not paused
	}
}

// return frames per second
int VGTime::get_fps() const
{	
	return min((int(1.0 / delta_time())), fps_limit); // get minimum of ticks per second and fps_limit to get the average frames per second
}
