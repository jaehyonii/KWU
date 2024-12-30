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

/* Artist ��ü�� Song ��� �߰��ϱ�*/
void ArtistNode::SongInsert(SongNode* song) {
	/* Song ����Ÿ�� ������� ��*/
	if (s_Head == nullptr) {
		s_Head = song;
		return;
	}
	/* song ��尡 ����Ʈ�� Head�� �ž� �� ��*/
	else if (s_Head->getSong() > song->getSong()) {
		song->setNext(s_Head);
		s_Head = song;
		return;
	}

	SongNode* pWalker = s_Head;
	SongNode* pPre = s_Head;
	/* song ��尡 ����Ʈ �߰��� ��ġ�ؾ� �� ��*/
	while (pWalker) { // song�� ��ġ�� ������ ��ġ Ž��
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
	/* Song ����Ʈ�� ������� ��*/
	if (s_Head == nullptr) {
		return;
	}

	/* Song ����Ʈ ��ü ���*/
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
		/* Head�� ���� ������ ���ʷ� ����*/
		ArtistNode* pWalker = a_Head->getNext();
		while (pWalker != a_Head) {
			a_Head->setNext(pWalker->getNext()); // pWalker�� ����Ʈ���� ����
			delete pWalker; // pWalker ����
			pWalker = a_Head->getNext(); // pWalker�� ����Ű�� ��� ����
		}
		delete a_Head; // ������ ���� ��� ����
	}
}

void MyMusicManagementList::musicInsert(string& artist, string& song) {
	/* Music�� ���� ������ ���� ��*/
	if (a_Head == nullptr) {
		/* Song ��� ����*/
		SongNode* s = new SongNode();
		s->setSong(song);
		/* Artist ��� ����*/
		ArtistNode* a = new ArtistNode();
		a->setArtist(artist);
		a->SongInsert(s); // Artist�� Song �߰�

		/* ���� ���Ḯ��Ʈ ����*/
		a_Head = a;
		a->setNext(a_Head);
		return;
	}

	/* �̹� ���Ḯ��Ʈ�� ������ ��*/
	ArtistNode* pWalker = a_Head;
	do {
		if (pWalker->getArtist() == artist) { // �̹� ���� ������ music�� �����Ѵٸ�
			/* Song ��� ����*/
			SongNode* s = new SongNode();
			s->setSong(song);
			pWalker->SongInsert(s); // �ش� ���� ��忡 song �߰�
			return;
		}
		pWalker = pWalker->getNext();
	} while (pWalker != a_Head);
	
	/* ���� ������ �������� �ʴ´ٸ� */
	/* Song ��� ����*/
	SongNode* s = new SongNode();
	s->setSong(song);
	/* Artist ��� ����*/
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