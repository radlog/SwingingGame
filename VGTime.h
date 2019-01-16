#pragma once
extern const int fps_limit;

class VGTime
{

	

public:
	VGTime();
	~VGTime();

	double delta_time() const;
	double total_time() const;

	void start(); // start the timer
	void tick(); // run the timer and add a tick to total time
	void pause(); // pause the timer
	void resume(); // resume the timer
	void stop(); // stop the timer
	void reset(); // reset the timer

	int get_fps() const;

private:
	//Storage of time
	long long int start_; //startTime
	long long int paused_; //Time when the timer was paused
	long long int idle_; //total time the timer was paused
	long long int current_; //currentTime
	long long int previous_; //time of the previous tick

	double total_; //total time the timer ran(total lifetime - idle)
	double delta_; //time between two frames
	double seconds_per_tick_; // value in seconds per tick

	bool is_paused_; // is timer paused or not

	//int fps_ = 0; // average frames per second
	
};

