#pragma once
class VGTime
{
public:
	VGTime();
	~VGTime();

	double deltaTime() const;
	double totalTime() const;

	void start();
	void tick();
	void pause();
	void resume();
	void stop();
	void reset();

private:
	//Storage of time
	long long int _start; //startTime
	long long int _paused; //Time when the timer was paused
	long long int _idle; //total time the timer was paused
	long long int _total; //total time the timer ran(total lifetime - idle)
	long long int _current; //currentTime
	long long int _previous; //time of the previous tick

	double _delta; //time between two frames
	double _seconds_per_tick;

	bool _is_paused;
};

