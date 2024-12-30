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

/* Polynomial�� pTerm�� ���� Exponent�� ������ term�� ���� ��쿡�� ���*/
void Polynomial::Insert(Term* pTerm) {
	/* ����� 0�� ���� �߰� ����*/
	if (pTerm->GetCoeff() == 0)
		return;
	/* Polynomial�� ������� ��*/
	else if (m_pHead == nullptr)	{
		m_pHead = pTerm;
		TermNum++;
		return;
	}
	/* pTerm�� Polynomial�� Head�� �� ���*/
	else if (m_pHead->GetExponent() < pTerm->GetExponent()) {
		pTerm->SetNext(m_pHead);
		m_pHead = pTerm;
		TermNum++;
		return; // Insert �޼ҵ� ����
	}

	Term* pWalker = m_pHead->GetNext();
	Term* pPre = m_pHead;;
	/* pTerm�� ���Ե� ��ġ�� Ž��*/
	while (pWalker) {
		if (pWalker->GetExponent() > pTerm->GetExponent()) {
			pPre = pWalker;
			pWalker = pWalker->GetNext();
		}
		/* pTerm�� exponent�� pPre�� pWalker ���̰��� ��*/
		else
			break; // Ž�� ����
	}
	/* pTerm�� pPre pWalker �����̰ų� �� �������� ��� ����*/
	pTerm->SetNext(pWalker);
	pPre->SetNext(pTerm);
	TermNum++;
}

void Polynomial::PrintList() {
	/* ���� ����� 0�� ��� Ȥ�� ���� �Էµ��� �ʾ��� ���*/
	if (m_pHead == nullptr)
		return;
	Term* pWalker = m_pHead;
	while (pWalker) {
		if (pWalker->GetExponent() != 0) { // ������ ���
			if (pWalker->GetCoeff() > 0)// ����� ����� ������ ���
				cout << " +" << pWalker->GetCoeff() << "x^" << pWalker->GetExponent();
			else if (pWalker->GetCoeff() < 0)// ����� ������ ������ ���
				cout << " " << pWalker->GetCoeff() << "x^" << pWalker->GetExponent();
		}
		else { // ��� ���
			if (pWalker->GetCoeff() > 0)// ����� ����� ������ ���
				cout << " +" << pWalker->GetCoeff();
			else if (pWalker->GetCoeff() < 0)// ����� ������ ������ ���
				cout << " " << pWalker->GetCoeff();
		}
		
		pWalker = pWalker->GetNext();
	}
}

void Polynomial::Add(Polynomial& poly) {
	Term* p1 = m_pHead;
	Term* p2 = poly.m_pHead;
	Term* New; // ���ο� ��� ����
	
	while (p1 && p2) {
		if (p1->GetExponent() > p2->GetExponent())
			p1 = p1->GetNext();
		else if (p1->GetExponent() < p2->GetExponent())	{
			/* p2�� �Ȱ��� �����͸� ������ New ����*/
			New = new Term();
			New->SetCoeff(p2->GetCoeff());
			New->SetExponent(p2->GetExponent());

			Insert(New); // New ����
			p2 = p2->GetNext(); //p2 ���� Term ����
			TermNum++;
		}
		else{ //������ ���� �� ������ ����
			p1->SetCoeff(p1->GetCoeff() + p2->GetCoeff());
			p1 = p1->GetNext();
			p2 = p2->GetNext();
		}
	}
	/* p1�� ���� Term�� p2�� �����ִ� ���*/
	while (p2) {
		/* p2�� �Ȱ��� �����͸� ������ New ����*/
		New = new Term();
		New->SetCoeff(p2->GetCoeff());
		New->SetExponent(p2->GetExponent());
		
		Insert(New); // New ����
		p2 = p2->GetNext(); //p2 ���� Term ����
		TermNum++;
	}

	/* ���� �� ����� 0�̸� Term ����*/
	Term* pPre;
	while (m_pHead->GetCoeff() == 0) { // ù��° Term�� ����� 0�� �� Term ����
		pPre = m_pHead->GetNext();
		delete m_pHead;
		m_pHead = pPre;
		TermNum--;
	}

	pPre = m_pHead;
	p1 = m_pHead->GetNext();
	while (p1) { // Term �˻�
		if (p1->GetCoeff() == 0) { // �߰� Term�� ����� 0�� �� Term ����
			pPre->SetNext(p1->GetNext()); // ���� Term ����
			delete p1; // ��� 0�� Term ����
			p1 = pPre->GetNext(); // p1�� ���� Term�� ����Ŵ
			TermNum--;
			continue;
		}
		pPre = p1;
		p1 = p1->GetNext(); // p1�� ���� Term�� ����Ŵ
	}
}

void Polynomial::Sub(Polynomial& poly) {
	Term* p1 = m_pHead;
	Term* p2 = poly.m_pHead;
	Term* New; // ���ο� ��� ����

	while (p1 && p2) {
		if (p1->GetExponent() > p2->GetExponent())
			p1 = p1->GetNext();
		else if (p1->GetExponent() < p2->GetExponent()) {
			/* p2�� ��ȣ�� �ݴ�� �������� �Ȱ��� �����͸� ������ New ����*/
			New = new Term();
			New->SetCoeff(-(p2->GetCoeff()));
			New->SetExponent(p2->GetExponent());

			Insert(New); // New ����
			p2 = p2->GetNext(); //p2 ���� Term ����
			TermNum++;
		}
		else {
			p1->SetCoeff(p1->GetCoeff() - p2->GetCoeff());
			p1 = p1->GetNext();
			p2 = p2->GetNext();
		}
	}
	/* p1�� ���� Term�� p2�� �����ִ� ���*/
	while (p2) {
		/* p2�� �Ȱ��� �����͸� ������ New ����*/
		New = new Term();
		New->SetCoeff(-(p2->GetCoeff()));
		New->SetExponent(p2->GetExponent());

		Insert(New); // New ����
		p2 = p2->GetNext(); //p2 ���� Term ����
		TermNum++;
	}

	/* ���� �� ����� 0�̸� Term ����*/
	Term* pPre;
	while (m_pHead->GetCoeff() == 0) { // ù��° Term�� ����� 0�� �� Term ����
		pPre = m_pHead->GetNext();
		delete m_pHead;
		m_pHead = pPre;
		TermNum--;
	}

	pPre = m_pHead;
	p1 = m_pHead->GetNext();
	while (p1) { // Term �˻�
		if (p1->GetCoeff() == 0) { // �߰� Term�� ����� 0�� �� Term ����
			pPre->SetNext(p1->GetNext()); // ���� Term ����
			delete p1; // ��� 0�� Term ����
			p1 = pPre->GetNext(); // p1�� ���� Term�� ����Ŵ
			TermNum--;
			continue;
		}
		pPre = p1;
		p1 = p1->GetNext(); // p1�� ���� Term�� ����Ŵ
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