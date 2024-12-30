#include<iostream>

using namespace std;

char* my_strtok(char* str) {
	static char* pstr = nullptr;
	static int idx = 0; // 문자열 str의 인덱스

	/* str에 문자열 배열이 입력되면 문자열의 주소값을 저장*/
	if (str){
		pstr = str;
	}
	
	int pre = idx;
	while (1) {
		if (pstr[idx] == '@' || pstr[idx] == '.') {
			pstr[idx] = NULL; // '@', '.'를 NULL로 대체
			idx++; // 다음 함수 호출에서 NULL 다음 문자를 가리킬 수 있도록 함
			break;
		}
		else if (pstr[idx] == NULL) {
			break; //문자열의 끝이면 idx가 NULL을 가리키게 됨
		}
		idx++;
	}
	
	/* 문자열의 끝이면 idx에 덧셈연산이 진행되지 않아 pre와 같다*/
	if (pre == idx)
		return NULL;
	else
		return pstr + pre;
}

int main()
{
	char arr[100];

	cin >> arr;
	char* ptr = my_strtok(arr);
	
	while (ptr) {
		cout << ptr << endl;
		ptr = my_strtok(NULL);
	}

	return 0;
}