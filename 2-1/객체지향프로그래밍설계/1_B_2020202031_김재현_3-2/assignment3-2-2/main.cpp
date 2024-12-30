#define _CRT_SECURE_NO_WARNINGS
#include<iostream>

using namespace std;

class myMusic
{
private:
	char m_title[32];
	char m_singer[32];
	char m_album[32];
	int m_track_no;
	int m_year;

public:
	myMusic() {
		m_track_no = 0;
		m_year = 0;
	}
	~myMusic() {}

	void setTitle(char* title) {
		strcpy(m_title, title);
	}
	void setSinger(char* singer) {
		strcpy(m_singer, singer);
	}
	void setAlbum(char* album) {
		strcpy(m_album, album);
	}
	void setYear(int year) {
		m_year = year;
	}
	void setTrack(int track_no) {
		m_track_no = track_no;
	}

	char* getTitle() {
		return m_title;
	}
	char* getSinger() {
		return m_singer;
	}
	char* getAlbum() {
		return m_album;
	}
	int getYear() {
		return m_year;
	}
	int getTrackNo() {
		return m_track_no;
	}
};

int main() {
	myMusic music[3];
	
	char input[100];
	int track_no = 0;
	int year = 0;
	
	cout << "Input by following order.\nSinger,Album,Title,Year,Track number." << endl << endl;
	for(int i = 0; i < 3; i++)
	{
		switch (i) {
		case 0:
			cout << "Input 1st Music: ";
			break;
		case 1:
			cout << "Input 2nd Music: ";
			break;
		case 2:
			cout << "Input 3nd Music: ";
			break;
		}
		
		cin.getline(input, 100,','); // Singer 입력 
		music[i].setSinger(input); // 세팅
		cin.getline(input, 100, ','); // Album 입력
		music[i].setAlbum(input); // 세팅
		cin.getline(input, 100, ','); // Title 입력
		music[i].setTitle(input); // 세팅
		cin.getline(input, 100, ','); // Year 입력
		year = 0;
		for (int i = 0; input[i]; i++) { // Year 정수로 변환
			year *= 10;
			year += input[i] - '0';
		}
		music[i].setYear(year); // 세팅
		cin.getline(input, 100); // Track number 입력
		track_no = 0;
		for (int i = 0; input[i]; i++) { // Track number 정수로 변환
			track_no *= 10;
			track_no += input[i] - '0';
		}
		music[i].setTrack(track_no); // 세팅
	}
	cout << endl;
	
	/* myMusic 배열들을 모두 출력*/
	for (int i = 0; i < 3; i++) {
		switch (i) {
		case 0:
			cout << "1st Music" << endl;
			break;
		case 1:
			cout << "2nd Music" << endl;
			break;
		case 2:
			cout << "3nd Music" << endl;
			break;
		}
		cout << "singer: " << music[i].getSinger() << endl; // 가수 이름 출력
		cout << "album: " << music[i].getAlbum() << endl; // 앨범 이름 출력
		cout << "title: " << music[i].getTitle() << endl; // 타이틀곡 제목 출력
		cout << "year: " << music[i].getYear() << endl; // 발매 년도 출력
		cout << "track number: " << music[i].getTrackNo() << endl; // 트랙 수 출력
		cout << endl;
	}
}