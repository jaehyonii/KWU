#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>

using namespace std;

class SongNode
{
private:
	string song;
	SongNode* pNext;
public:
	SongNode();
	~SongNode();

	void setSong(string song);
	void setNext(SongNode* next);
	string getSong();
	SongNode* getNext();
};

class ArtistNode
{
private:
	string artist;
	ArtistNode* pNext;
	SongNode* s_Head;
public:
	ArtistNode();
	~ArtistNode();

	/* Artist management*/
	void setArtist(string artist);
	void setNext(ArtistNode* next);
	string getArtist();
	ArtistNode* getNext();

	/* Song management*/
	void SongInsert(SongNode* song);
	void printSong();
};

class MyMusicManagementList
{
private:
	ArtistNode* a_Head;
public:
	MyMusicManagementList();
	~MyMusicManagementList();
	
	void musicInsert(string& artist, string& song);
	void printMusicInfo();
};
