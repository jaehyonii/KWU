#include "Node.h"

/* SongNode*/
SongNode::SongNode() {
	pNext = nullptr;
}
SongNode::~SongNode() {}

void SongNode::setNext(SongNode* next) {
	pNext = next;
}
SongNode* SongNode::getNext() {
	return pNext;
}
void SongNode::setSong(string song) {
	this->song = song;
}
string SongNode::getSong() {
	return song;
}



/* ArtistNode*/
ArtistNode::ArtistNode() {
	pNext = nullptr;
	s_Head = nullptr;
}
ArtistNode::~ArtistNode() {
	SongNode* tmp;
	while (s_Head) {
		tmp = s_Head->getNext();
		delete s_Head;
		s_Head = tmp;
	}
}

void ArtistNode::setNext(ArtistNode* next) {
	pNext = next;
}
ArtistNode* ArtistNode::getNext() {
	return pNext;
}
void ArtistNode::setArtist(string artist) {
	this->artist = artist;
}
string ArtistNode::getArtist() {
	return artist;
}

/* Artist 객체에 Song 노드 추가하기*/
void ArtistNode::SongInsert(SongNode* song) {
	/* Song 리스타가 비어있을 때*/
	if (s_Head == nullptr) {
		s_Head = song;
		return;
	}
	/* song 노드가 리스트의 Head가 돼야 할 때*/
	else if (s_Head->getSong() > song->getSong()) {
		song->setNext(s_Head);
		s_Head = song;
		return;
	}

	SongNode* pWalker = s_Head;
	SongNode* pPre = s_Head;
	/* song 노드가 리스트 중간에 위치해야 할 때*/
	while (pWalker) { // song이 위치할 적절한 위치 탐색
		if (pWalker->getSong() < song->getSong()) {
			pPre = pWalker;
			pWalker = pWalker->getNext();
		}
		else {
			break;
		}
	}
	pPre->setNext(song);
	song->setNext(pWalker);
}
void ArtistNode::printSong() {
	/* Song 리스트가 비어있을 때*/
	if (s_Head == nullptr) {
		return;
	}

	/* Song 리스트 전체 출력*/
	SongNode* pWalker = s_Head;
	while (pWalker) {
		cout << "\t/" << pWalker->getSong();
		pWalker = pWalker->getNext();
	}
}



/* MyMusicManagementList*/
MyMusicManagementList::MyMusicManagementList() {
	a_Head = nullptr;
}

MyMusicManagementList::~MyMusicManagementList() {
	if(a_Head!=nullptr)	{
		/* Head의 다음 노드부터 차례로 삭제*/
		ArtistNode* pWalker = a_Head->getNext();
		while (pWalker != a_Head) {
			a_Head->setNext(pWalker->getNext()); // pWalker를 리스트에서 제외
			delete pWalker; // pWalker 삭제
			pWalker = a_Head->getNext(); // pWalker가 가리키는 대상 변경
		}
		delete a_Head; // 마지막 남은 노드 삭제
	}
}

void MyMusicManagementList::musicInsert(string& artist, string& song) {
	/* Music에 대한 정보가 없을 때*/
	if (a_Head == nullptr) {
		/* Song 노드 생성*/
		SongNode* s = new SongNode();
		s->setSong(song);
		/* Artist 노드 생성*/
		ArtistNode* a = new ArtistNode();
		a->setArtist(artist);
		a->SongInsert(s); // Artist에 Song 추가

		/* 원형 연결리스트 생성*/
		a_Head = a;
		a->setNext(a_Head);
		return;
	}

	/* 이미 연결리스트가 존재할 때*/
	ArtistNode* pWalker = a_Head;
	do {
		if (pWalker->getArtist() == artist) { // 이미 동일 가수의 music이 존재한다면
			/* Song 노드 생성*/
			SongNode* s = new SongNode();
			s->setSong(song);
			pWalker->SongInsert(s); // 해당 가수 노드에 song 추가
			return;
		}
		pWalker = pWalker->getNext();
	} while (pWalker != a_Head);
	
	/* 동일 가수가 존재하지 않는다면 */
	/* Song 노드 생성*/
	SongNode* s = new SongNode();
	s->setSong(song);
	/* Artist 노드 생성*/
	ArtistNode* a = new ArtistNode();
	a->setArtist(artist);
	a->SongInsert(s);
	
	a->setNext(a_Head->getNext());
	a_Head->setNext(a);
	a_Head = a;
}
void MyMusicManagementList::printMusicInfo() {
	if (a_Head == nullptr)
		return;

	ArtistNode* artist = a_Head->getNext();
	while(artist != a_Head) {
		cout << artist->getArtist() << '\t';
		artist->printSong();
		cout << endl;
		
		artist = artist->getNext();
	}
	cout << a_Head->getArtist() << '\t';
	a_Head->printSong();	
}