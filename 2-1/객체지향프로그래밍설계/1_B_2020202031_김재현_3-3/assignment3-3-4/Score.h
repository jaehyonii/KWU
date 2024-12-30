#pragma once
#include <iostream>

using namespace std;

class Score 
{
private:
	Score* m_pNext;
	Score* m_pPrev;
	double m_Avg;

public:
	Score();
	~Score();

	void SetAvg(double avg);
	void SetNext(Score* pNext);
	void SetPrev(Score* pPrev);
	double GetAvr();
	Score* GetNext();
	Score* GetPrev();
};

class StudentScoreList 
{
private:
	Score* m_pHead;
	Score* m_pTail;

public:
	StudentScoreList();
	~StudentScoreList();

	void Insert(Score* pScore);
	void PrintList(bool isAscending);
};