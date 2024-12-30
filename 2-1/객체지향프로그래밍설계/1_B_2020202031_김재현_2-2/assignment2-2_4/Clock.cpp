#include "Clock.h"

/* public*/
/* ��ü�� ����Ǹ� �����ڸ� ���� hour, minute, second�� 0���� �ʱ�ȭ*/
Clock::Clock() {
	cout << "Clock turns on" << endl;
	hour = 0;
	minute = 0;
	second = 0;
}
/* ���α׷��� ����� �� �Ҹ��ڸ� ���� ���� �ð��� ���*/
Clock::~Clock() {
	cout << "It's " << hour << ":" << minute << ":" << second << " now" << endl;
}
/* hour�� 1������Ű�� hour�� 24�� �Ǹ� 0���� �ǵ��� // ���� �ð� ���*/
void Clock::increaseHour() {
	hour++;
	if (hour >= 24) {
		hour -= 24;
	}
	cout << "It's " << hour << ":" << minute << ":" << second << " now" << endl;
}
/* minute�� 1������Ű�� minute�� 60�� �Ǹ� 0���� �ǵ����� increaseHourȣ�� // ���� �ð� ���*/
void Clock::increaseMinute() {
	minute++;
	if (minute >= 60) {
		minute -= 60;
		increaseHour();
		return;
	}
	cout << "It's " << hour << ":" << minute << ":" << second << " now" << endl;
}
/* second�� 1������Ű�� second�� 60�� �Ǹ� 0���� �ǵ����� increaseMinuteȣ�� // ���� �ð� ���*/
void Clock::increaseSecond() {
	second++;
	if (second >= 60) {
		second -= 60;
		increaseMinute();
		return;
	}
	cout << "It's " << hour << ":" << minute << ":" << second << " now" << endl;
}
/* �ʴ����� �Է� ���� �ð��� ��, ��, �ð� ������ ���� ��ȯ�Ͽ� set�޼ҵ�鿡 ����*/
void Clock::setTime(int second) {
	setSecond(second % 60);
	second /= 60; // �ʸ� �д����� ��ȯ
	setMinute(second % 60);
	second /= 60; // ���� �ð������� ��ȯ
	setHour(second);
	cout << "It's " << hour << ":" << minute << ":" << this->second << " now" << endl;
}


/* private*/
/* hour�� 24�̻��̸� -24���ְ� 24�̸��� �Ǹ� �� ������ �ʱ�ȭ*/
void Clock::setHour(int hour) {
	while (hour >= 24)
		hour -= 24;
	this->hour = hour;
}

/* setMinute�� setSecond�� ���ڷ� 60�̻��� ���� �����Ƿ� ���ڰ� �״�� �ʱ�ȭ*/
void Clock::setMinute(int minute) {
	this->minute = minute;
}
void Clock::setSecond(int second) {
	this->second = second;
}
