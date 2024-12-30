#pragma once
#include<iostream>

using namespace std;

class Clock {
private:
	int hour;
	int minute;
	int second;

	void setHour(int hour);
	void setMinute(int minute);
	void setSecond(int second);

	
public:
	Clock();
	~Clock();
	
	void increaseHour();
	void increaseMinute();
	void increaseSecond();

	void setTime(int second);
};