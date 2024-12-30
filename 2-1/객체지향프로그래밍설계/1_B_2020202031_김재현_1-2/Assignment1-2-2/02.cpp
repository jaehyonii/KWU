// cout ���� char���� �� int������ ��µǴ��� �˾ƺ���

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
	char* ptr = Transmission_Channel(data);// Received frame �����Ҵ�
	Receiver(ptr);
	
	delete[] ptr; // �����Ҵ� �޸� ����

	return 0;
}

void Sender(char* data)
{
	char copy[16];
	char* ptr = copy;
	
	/*copy�� data ����*/
	for (int i = 0; i < 16; i++)
		copy[i] = data[i];
	
	XOR(ptr);
	/*XOR���� (ptr�� FCS�� ����Ű�� ��)*/
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

	/*data�� FCS ���̱�*/
	for (int i = 0; i < 4; i++)
		data[i + 12] = ptr[i];

	/*���*/
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
		/*0~99������ �� �߿� 0~94���� ���� Ȯ���� 95%��*/
		if (rand() % 100 < 95)
			/*frame�� coded_frame�� ����������*/
			frame[i] = frame[i] ^ coded_frame[i];
		else
		{
			/*frame�� coded_frame�� �ݴ밡 �ǵ���*/
			if (!coded_frame[i])
				frame[i] = 1;
		}
	}
	
	/*���*/
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

	/*copy�� data ����*/
	for (int i = 0; i < 16; i++)
		copy[i] = received_frame[i];

	XOR(ptr);
	/*XOR���� (ptr�� FCS�� ����Ű�� ��)*/
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
	
	/*���*/
	cout << "Reconstructed data: ";
	for (int i = 0; i < 12; i++)
		cout << (char)(received_frame[i] + '0');
	cout << endl;

	/*���� Ȯ���ϱ�*/
	for (int i = 0; i < 4; i++) {
		if (ptr[i]) {
			cout << "Receiver has detected error";
			return;
		}
	}
	cout << "No detected error";
}

/*�� 5�ڸ� ���ڸ� XOR���� ����*/
void XOR(char* ptr)
{
	const int code = 0b11011;
	int num = 0;
	
	/*num�� copy �� 5�ڸ� ����*/
	for (int i = 0; i < 5; i++) {
		num = num << 1;
		num = num | ptr[i];
	}
	/*XOR ����*/
	num = num^ code;
	
	/*���� ��� �ٽ� copy�� ����*/
	for (int i = 4; i >= 0; i--) {
		ptr[i] = num % 2;
		num /= 2;
	}
}