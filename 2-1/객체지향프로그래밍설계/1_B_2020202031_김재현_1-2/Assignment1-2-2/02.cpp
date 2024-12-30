// cout 에서 char형이 왜 int형으로 출력되는지 알아보기

#include<iostream>
#include<string.h>
#include<cstdlib>
#include<ctime>

using namespace std;

void XOR(char* copy);
void Sender(char* data);
char* Transmission_Channel(const char* coded_frame);
void Receiver(const char* received_frame);

int main()
{
	char data[16] = { 0 };

	cout << "Data: ";
	cin >> data;

	for (int i = 0; i < 12; i++)
		data[i] -= '0';

	Sender(data);
	char* ptr = Transmission_Channel(data);// Received frame 동적할당
	Receiver(ptr);
	
	delete[] ptr; // 동적할당 메모리 해제

	return 0;
}

void Sender(char* data)
{
	char copy[16];
	char* ptr = copy;
	
	/*copy에 data 복제*/
	for (int i = 0; i < 16; i++)
		copy[i] = data[i];
	
	XOR(ptr);
	/*XOR연산 (ptr이 FCS를 가리키게 됨)*/
	int num = 0;
	while (1) {
		while (!(*ptr)) {
			ptr++;
			num++;
			if (num == 12)
				break;
		}
		if (num == 12)
			break;
		XOR(ptr);
	}

	/*data에 FCS 붙이기*/
	for (int i = 0; i < 4; i++)
		data[i + 12] = ptr[i];

	/*출력*/
	cout << "Coded frame: ";
	for (int i = 0; i < 16; i++)
		cout << (char)(data[i] + '0');
	cout << endl;
}

char* Transmission_Channel(const char* coded_frame)
{
	char* frame = new char[16];
	for (int i = 0; i < 16; i++)
		frame[i] = 0;

	srand((unsigned int)time(NULL));
	for (int i = 0; i < 16; i++) {
		/*0~99까지의 수 중에 0~94까지 나올 확률은 95%다*/
		if (rand() % 100 < 95)
			/*frame이 coded_frame과 같아지도록*/
			frame[i] = frame[i] ^ coded_frame[i];
		else
		{
			/*frame이 coded_frame과 반대가 되도록*/
			if (!coded_frame[i])
				frame[i] = 1;
		}
	}
	
	/*출력*/
	cout << "Received frame: ";
	for (int i = 0; i < 16; i++)
		cout << (char)(frame[i] + '0');
	cout << endl;
	
	return frame;
}

void Receiver(const char* received_frame)
{
	char copy[16];
	char* ptr = copy;

	/*copy에 data 복제*/
	for (int i = 0; i < 16; i++)
		copy[i] = received_frame[i];

	XOR(ptr);
	/*XOR연산 (ptr이 FCS를 가리키게 됨)*/
	int num = 0;
	while (1) {
		while (!(*ptr)) {
			ptr++;
			num++;
			if (num == 12)
				break;
		}
		if (num == 12)
			break;
		XOR(ptr);
	}
	
	/*출력*/
	cout << "Reconstructed data: ";
	for (int i = 0; i < 12; i++)
		cout << (char)(received_frame[i] + '0');
	cout << endl;

	/*에러 확인하기*/
	for (int i = 0; i < 4; i++) {
		if (ptr[i]) {
			cout << "Receiver has detected error";
			return;
		}
	}
	cout << "No detected error";
}

/*앞 5자리 숫자만 XOR연산 진행*/
void XOR(char* ptr)
{
	const int code = 0b11011;
	int num = 0;
	
	/*num에 copy 앞 5자리 저장*/
	for (int i = 0; i < 5; i++) {
		num = num << 1;
		num = num | ptr[i];
	}
	/*XOR 연산*/
	num = num^ code;
	
	/*연산 결과 다시 copy에 저장*/
	for (int i = 4; i >= 0; i--) {
		ptr[i] = num % 2;
		num /= 2;
	}
}