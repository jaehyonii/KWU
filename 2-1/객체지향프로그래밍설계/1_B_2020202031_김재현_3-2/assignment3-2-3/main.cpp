#include "Node.h"
#include<fstream>

int main()
{
	ifstream fin;
	fin.open("Music_Info.txt");
	
	string artist;
	string song;

	MyMusicManagementList music;
	getline(fin, artist); // ù �� �о���̱�

	while (!fin.eof()) {
		int num = 0;
		getline(fin, artist, '/'); // artist ���� �б�
		for (int i = artist.length() - 1; artist[i] == ' ' || artist[i] == '\t'; i--) // ���� �� �鿩���� ���� �ľ�
			num++;
		artist.erase(artist.length() - num, num); // ����, �鿩���� ����
		
		num = 0;
		getline(fin, song); // song ���� �б�
		for (int i = 0; song[i] == ' ' || song[i] == '\t'; i++) // ���� �� �鿩���� ���� �ľ�
			num++;
		song.erase(0, num); // ����, �鿩���� ����
		/* �빮�ڸ� �ҹ��ڷ� ��ȯ*/
		for (int i = 0; i < song.length(); i++) {
			if ('A' <= song[i] && song[i] <= 'Z')
				song[i] |= 32; // song[i] -= 32;
		}
		
		music.musicInsert(artist, song);
	}
	music.printMusicInfo();
	
	fin.close();
}