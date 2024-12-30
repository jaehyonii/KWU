#include "Score.h"

Score::Score() {
	m_pNext = nullptr;
	m_pPrev = nullptr;
	m_Avg = 0;
}
Score::~Score(){}

void Score::SetAvg(double avg) {
	m_Avg = avg;
}
void Score::SetNext(Score* pNext) {
	m_pNext = pNext;
}
void Score::SetPrev(Score* pPrev) {
	m_pPrev = pPrev;
}
double Score::GetAvr() {
	return m_Avg;
}
Score* Score::GetNext() {
	return m_pNext;
}
Score* Score::GetPrev() {
	return m_pPrev;
}


StudentScoreList::StudentScoreList() {
	m_pHead = nullptr;
	m_pTail = nullptr;
}
StudentScoreList::~StudentScoreList() {
	/* Tail�κк��� ���ʷ� ����*/
	while (m_pTail != m_pHead) {
		m_pTail = m_pTail->GetPrev();
		delete m_pTail->GetNext();
	}
	delete m_pTail; // ������ ���� ��� ����
}

void StudentScoreList::Insert(Score* pScore) {
	if (!m_pHead) {
		m_pHead = pScore;
		m_pTail = pScore;
	}
	else {
		Score* pWalker = m_pHead;
		/* pScore�� ������ ��ġ Ž��*/
		while (pWalker!=nullptr) {
			if (pWalker->GetAvr() >= pScore->GetAvr())
				break;
			pWalker = pWalker->GetNext();
		}
		/* pScore�� ��������� ���� ũ�� pScore�� pTail�� �ȴ�.*/
		if (pWalker == nullptr) {
			m_pTail->SetNext(pScore);
			pScore->SetPrev(m_pTail);
			m_pTail = pScore;
		}
		/* pScore�� ��������� ���� ������ pScore�� pHead�� �ȴ�.*/
		else if (pWalker == m_pHead) {
			m_pHead->SetPrev(pScore);
			pScore->SetNext(m_pHead);
			m_pHead = pScore;
		}
		/* pScore�� ��������� �߰����� ��*/
		else {
			pScore->SetNext(pWalker);
			pWalker->GetPrev()->SetNext(pScore);
			pScore->SetPrev(pWalker->GetPrev());
			m_pTail->SetPrev(pScore);
		}
	}
}
void StudentScoreList::PrintList(bool isAscending) {
	/* ������� �������� ���*/
	if (isAscending == true) {
		Score* pWalker = m_pHead; // pHead����
		while (pWalker != m_pTail) { // pTail ��������
			cout << pWalker->GetAvr() << " "; // ���
			pWalker = pWalker->GetNext();
		}
		cout << m_pTail->GetAvr() << endl; // pTail ���
	}
	/* ������� �������� ���*/
	else {
		Score* pWalker = m_pTail; // pTail����
		while (pWalker != m_pHead) { // pHead ��������
			cout << pWalker->GetAvr() << " "; // ���
			pWalker = pWalker->GetPrev();
		}
		cout << m_pHead->GetAvr() << endl; // pHead ���
	}
}