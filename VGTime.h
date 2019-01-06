#pragma once
#include <Windows.h>
class VGTime
{
public:
	VGTime();
	~VGTime();

	double delta_time() const;
	double total_time() const;

	void start();
	void tick();
	void pause();
	void resume();
	void stop();
	void reset();

	int get_fps();

private:
	//Storage of time
	long long int start_; //startTime
	long long int paused_; //Time when the timer was paused
	long long int idle_; //total time the timer was paused
	long long int current_; //currentTime
	long long int previous_; //time of the previous tick

	double total_; //total time the timer ran(total lifetime - idle)
	double delta_; //time between two frames
	double seconds_per_tick_;

	bool is_paused_;

	int fps_ = 0;

};

