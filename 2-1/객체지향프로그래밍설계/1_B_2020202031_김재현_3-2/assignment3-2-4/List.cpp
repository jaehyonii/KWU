#include "List.h"

Polynomial::Polynomial() {
	m_pHead = nullptr;
	TermNum = 0;
}
Polynomial::~Polynomial() {
	Term* tmp;
	while (m_pHead) {
		tmp = m_pHead->GetNext();
		delete m_pHead;
		m_pHead = tmp;
	}
}

/* Polynomial에 pTerm과 같은 Exponent를 가지는 term이 없을 경우에만 사용*/
void Polynomial::Insert(Term* pTerm) {
	/* 계수가 0일 때는 추가 안함*/
	if (pTerm->GetCoeff() == 0)
		return;
	/* Polynomial이 비어있을 때*/
	else if (m_pHead == nullptr)	{
		m_pHead = pTerm;
		TermNum++;
		return;
	}
	/* pTerm이 Polynomial의 Head가 될 경우*/
	else if (m_pHead->GetExponent() < pTerm->GetExponent()) {
		pTerm->SetNext(m_pHead);
		m_pHead = pTerm;
		TermNum++;
		return; // Insert 메소드 종료
	}

	Term* pWalker = m_pHead->GetNext();
	Term* pPre = m_pHead;;
	/* pTerm이 삽입될 위치를 탐색*/
	while (pWalker) {
		if (pWalker->GetExponent() > pTerm->GetExponent()) {
			pPre = pWalker;
			pWalker = pWalker->GetNext();
		}
		/* pTerm의 exponent가 pPre와 pWalker 사이값일 때*/
		else
			break; // 탐색 종료
	}
	/* pTerm이 pPre pWalker 사이이거나 맨 마지막일 경우 삽입*/
	pTerm->SetNext(pWalker);
	pPre->SetNext(pTerm);
	TermNum++;
}

void Polynomial::PrintList() {
	/* 연산 결과가 0일 경우 혹은 항이 입력되지 않았을 경우*/
	if (m_pHead == nullptr)
		return;
	Term* pWalker = m_pHead;
	while (pWalker) {
		if (pWalker->GetExponent() != 0) { // 미지수 출력
			if (pWalker->GetCoeff() > 0)// 계수가 양수인 미지수 출력
				cout << " +" << pWalker->GetCoeff() << "x^" << pWalker->GetExponent();
			else if (pWalker->GetCoeff() < 0)// 계수가 음수인 미지수 출력
				cout << " " << pWalker->GetCoeff() << "x^" << pWalker->GetExponent();
		}
		else { // 상수 출력
			if (pWalker->GetCoeff() > 0)// 계수가 양수인 미지수 출력
				cout << " +" << pWalker->GetCoeff();
			else if (pWalker->GetCoeff() < 0)// 계수가 음수인 미지수 출력
				cout << " " << pWalker->GetCoeff();
		}
		
		pWalker = pWalker->GetNext();
	}
}

void Polynomial::Add(Polynomial& poly) {
	Term* p1 = m_pHead;
	Term* p2 = poly.m_pHead;
	Term* New; // 새로운 노드 생성
	
	while (p1 && p2) {
		if (p1->GetExponent() > p2->GetExponent())
			p1 = p1->GetNext();
		else if (p1->GetExponent() < p2->GetExponent())	{
			/* p2와 똑같은 데이터를 가지는 New 생성*/
			New = new Term();
			New->SetCoeff(p2->GetCoeff());
			New->SetExponent(p2->GetExponent());

			Insert(New); // New 삽입
			p2 = p2->GetNext(); //p2 다음 Term 참조
			TermNum++;
		}
		else{ //지수가 같을 때 연산을 진행
			p1->SetCoeff(p1->GetCoeff() + p2->GetCoeff());
			p1 = p1->GetNext();
			p2 = p2->GetNext();
		}
	}
	/* p1에 없는 Term이 p2에 남아있는 경우*/
	while (p2) {
		/* p2와 똑같은 데이터를 가지는 New 생성*/
		New = new Term();
		New->SetCoeff(p2->GetCoeff());
		New->SetExponent(p2->GetExponent());
		
		Insert(New); // New 삽입
		p2 = p2->GetNext(); //p2 다음 Term 참조
		TermNum++;
	}

	/* 연산 후 계수가 0이면 Term 삭제*/
	Term* pPre;
	while (m_pHead->GetCoeff() == 0) { // 첫번째 Term의 계수가 0일 때 Term 삭제
		pPre = m_pHead->GetNext();
		delete m_pHead;
		m_pHead = pPre;
		TermNum--;
	}

	pPre = m_pHead;
	p1 = m_pHead->GetNext();
	while (p1) { // Term 검사
		if (p1->GetCoeff() == 0) { // 중간 Term의 계수가 0일 때 Term 삭제
			pPre->SetNext(p1->GetNext()); // 이전 Term 저장
			delete p1; // 계수 0인 Term 삭제
			p1 = pPre->GetNext(); // p1이 다음 Term을 가리킴
			TermNum--;
			continue;
		}
		pPre = p1;
		p1 = p1->GetNext(); // p1이 다음 Term을 가리킴
	}
}

void Polynomial::Sub(Polynomial& poly) {
	Term* p1 = m_pHead;
	Term* p2 = poly.m_pHead;
	Term* New; // 새로운 노드 생성

	while (p1 && p2) {
		if (p1->GetExponent() > p2->GetExponent())
			p1 = p1->GetNext();
		else if (p1->GetExponent() < p2->GetExponent()) {
			/* p2와 부호만 반대고 나머지는 똑같은 데이터를 가지는 New 생성*/
			New = new Term();
			New->SetCoeff(-(p2->GetCoeff()));
			New->SetExponent(p2->GetExponent());

			Insert(New); // New 삽입
			p2 = p2->GetNext(); //p2 다음 Term 참조
			TermNum++;
		}
		else {
			p1->SetCoeff(p1->GetCoeff() - p2->GetCoeff());
			p1 = p1->GetNext();
			p2 = p2->GetNext();
		}
	}
	/* p1에 없는 Term이 p2에 남아있는 경우*/
	while (p2) {
		/* p2와 똑같은 데이터를 가지는 New 생성*/
		New = new Term();
		New->SetCoeff(-(p2->GetCoeff()));
		New->SetExponent(p2->GetExponent());

		Insert(New); // New 삽입
		p2 = p2->GetNext(); //p2 다음 Term 참조
		TermNum++;
	}

	/* 연산 후 계수가 0이면 Term 삭제*/
	Term* pPre;
	while (m_pHead->GetCoeff() == 0) { // 첫번째 Term의 계수가 0일 때 Term 삭제
		pPre = m_pHead->GetNext();
		delete m_pHead;
		m_pHead = pPre;
		TermNum--;
	}

	pPre = m_pHead;
	p1 = m_pHead->GetNext();
	while (p1) { // Term 검사
		if (p1->GetCoeff() == 0) { // 중간 Term의 계수가 0일 때 Term 삭제
			pPre->SetNext(p1->GetNext()); // 이전 Term 저장
			delete p1; // 계수 0인 Term 삭제
			p1 = pPre->GetNext(); // p1이 다음 Term을 가리킴
			TermNum--;
			continue;
		}
		pPre = p1;
		p1 = p1->GetNext(); // p1이 다음 Term을 가리킴
	}
}

Term::Term() {
	m_Coefficient = 0;
	m_Exponent = 0;
	m_pNext = nullptr;
}
Term::~Term(){}

void Term::SetCoeff(int coeff) {
	m_Coefficient = coeff;
}
void Term::SetExponent(int exponent) {
	m_Exponent = exponent;
}
int Term::GetCoeff() {
	return m_Coefficient;
}
int Term::GetExponent() {
	return m_Exponent;
}
void Term::SetNext(Term* pNext) {
	m_pNext = pNext;
}
Term* Term::GetNext() {
	return m_pNext;
}