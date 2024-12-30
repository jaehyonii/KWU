#include "Clock.h"

/* public*/
/* 객체가 선언되면 생성자를 통해 hour, minute, second를 0으로 초기화*/
Clock::Clock() {
	cout << "Clock turns on" << endl;
	hour = 0;
	minute = 0;
	second = 0;
}
/* 프로그램이 종료될 때 소멸자를 통해 현재 시간을 출력*/
Clock::~Clock() {
	cout << "It's " << hour << ":" << minute << ":" << second << " now" << endl;
}
/* hour를 1증가시키고 hour가 24가 되면 0으로 되돌림 // 현재 시간 출력*/
void Clock::increaseHour() {
	hour++;
	if (hour >= 24) {
		hour -= 24;
	}
	cout << "It's " << hour << ":" << minute << ":" << second << " now" << endl;
}
/* minute을 1증가시키고 minute이 60이 되면 0으로 되돌리고 increaseHour호출 // 현재 시간 출력*/
void Clock::increaseMinute() {
	minute++;
	if (minute >= 60) {
		minute -= 60;
		increaseHour();
		return;
	}
	cout << "It's " << hour << ":" << minute << ":" << second << " now" << endl;
}
/* second을 1증가시키고 second가 60이 되면 0으로 되돌리고 increaseMinute호출 // 현재 시간 출력*/
void Clock::increaseSecond() {
	second++;
	if (second >= 60) {
		second -= 60;
		increaseMinute();
		return;
	}
	cout << "It's " << hour << ":" << minute << ":" << second << " now" << endl;
}
/* 초단위로 입력 받은 시간을 초, 분, 시간 단위로 각각 변환하여 set메소드들에 전달*/
void Clock::setTime(int second) {
	setSecond(second % 60);
	second /= 60; // 초를 분단위로 변환
	setMinute(second % 60);
	second /= 60; // 분을 시간단위로 변환
	setHour(second);
	cout << "It's " << hour << ":" << minute << ":" << this->second << " now" << endl;
}


/* private*/
/* hour이 24이상이면 -24해주고 24미만이 되면 그 값으로 초기화*/
void Clock::setHour(int hour) {
	while (hour >= 24)
		hour -= 24;
	this->hour = hour;
}

/* setMinute과 setSecond는 인자로 60이상을 받지 않으므로 인자값 그대로 초기화*/
void Clock::setMinute(int minute) {
	this->minute = minute;
}
void Clock::setSecond(int second) {
	this->second = second;
}
