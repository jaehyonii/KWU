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
	/* Tail부분부터 차례로 삭제*/
	while (m_pTail != m_pHead) {
		m_pTail = m_pTail->GetPrev();
		delete m_pTail->GetNext();
	}
	delete m_pTail; // 마지막 남은 노드 삭제
}

void StudentScoreList::Insert(Score* pScore) {
	if (!m_pHead) {
		m_pHead = pScore;
		m_pTail = pScore;
	}
	else {
		Score* pWalker = m_pHead;
		/* pScore의 적절한 위치 탐색*/
		while (pWalker!=nullptr) {
			if (pWalker->GetAvr() >= pScore->GetAvr())
				break;
			pWalker = pWalker->GetNext();
		}
		/* pScore의 평균점수가 가장 크면 pScore가 pTail이 된다.*/
		if (pWalker == nullptr) {
			m_pTail->SetNext(pScore);
			pScore->SetPrev(m_pTail);
			m_pTail = pScore;
		}
		/* pScore의 평균점수가 가장 작으면 pScore가 pHead가 된다.*/
		else if (pWalker == m_pHead) {
			m_pHead->SetPrev(pScore);
			pScore->SetNext(m_pHead);
			m_pHead = pScore;
		}
		/* pScore의 평균점수가 중간값일 때*/
		else {
			pScore->SetNext(pWalker);
			pWalker->GetPrev()->SetNext(pScore);
			pScore->SetPrev(pWalker->GetPrev());
			m_pTail->SetPrev(pScore);
		}
	}
}
void StudentScoreList::PrintList(bool isAscending) {
	/* 점수평균 오름차순 출력*/
	if (isAscending == true) {
		Score* pWalker = m_pHead; // pHead부터
		while (pWalker != m_pTail) { // pTail 직전까지
			cout << pWalker->GetAvr() << " "; // 출력
			pWalker = pWalker->GetNext();
		}
		cout << m_pTail->GetAvr() << endl; // pTail 출력
	}
	/* 점수평균 내림차순 출력*/
	else {
		Score* pWalker = m_pTail; // pTail부터
		while (pWalker != m_pHead) { // pHead 직전까지
			cout << pWalker->GetAvr() << " "; // 출력
			pWalker = pWalker->GetPrev();
		}
		cout << m_pHead->GetAvr() << endl; // pHead 출력
	}
}