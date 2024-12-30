#include "Node.h"
#include<fstream>

int main()
{
	ifstream fin;
	fin.open("Music_Info.txt");
	
	string artist;
	string song;

	MyMusicManagementList music;
	getline(fin, artist); // 첫 줄 읽어들이기

	while (!fin.eof()) {
		int num = 0;
		getline(fin, artist, '/'); // artist 정보 읽기
		for (int i = artist.length() - 1; artist[i] == ' ' || artist[i] == '\t'; i--) // 공백 및 들여쓰기 개수 파악
			num++;
		artist.erase(artist.length() - num, num); // 공백, 들여쓰기 삭제
		
		num = 0;
		getline(fin, song); // song 정보 읽기
		for (int i = 0; song[i] == ' ' || song[i] == '\t'; i++) // 공백 및 들여쓰기 개수 파악
			num++;
		song.erase(0, num); // 공백, 들여쓰기 삭제
		/* 대문자를 소문자로 변환*/
		for (int i = 0; i < song.length(); i++) {
			if ('A' <= song[i] && song[i] <= 'Z')
				song[i] |= 32; // song[i] -= 32;
		}
		
		music.musicInsert(artist, song);
	}
	music.printMusicInfo();
	
	fin.close();
}