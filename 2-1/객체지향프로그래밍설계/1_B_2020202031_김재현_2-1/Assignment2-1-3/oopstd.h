#pragma once

namespace oopstd {
	void* memset(void* ptr, int value, size_t num) {
		unsigned char* p = (unsigned char*)ptr; // ptr�� char*������ ĳ����
		unsigned char data = (unsigned char)value; // data�� unsigned char������ ĳ����

		/* num����Ʈ��ŭ �ݺ�. 1����Ʈ���� value������ �ʱ�ȭ*/
		for (int i = 0; i < num; i++) {
			*p = value;
			p++;
		}
		return ptr; // ptr ��ȯ
	}
	void* memcpy(void* destination, const void* source, size_t num) {
		/* ����Ʈ ������ ���縦 �����ϱ� ������ unsigned char* ������ ĳ����*/
		unsigned char* dst = (unsigned char*)destination;
		const unsigned char* src = (const unsigned char*)source;

		for (int i = 0; i < num; i++) {
			dst[i] = src[i];
		}
		return destination;
	}
	
	int strcmp(const char* str1, const char* str2) {
		int idx = 0;

		while (str1[idx] || str2[idx]) {
			/*str1�� str2�� �޶����� ��������*/
			if (str1[idx] != str2[idx])
				return (str1[idx] - str2[idx]); // str1�� ũ�� > 0 ��ȯ, ������ < 0 ��ȯ
			idx++;
		}
		/* str1�� str2�� �Ȱ��� ��*/
		return 0;
	}
	int strncmp(const char* str1, const char* str2, size_t num) {
		int idx = 0;

		while ((str1[idx] || str2[idx]) && idx < num) {
			/*str1�� str2�� �޶����� ��������*/
			if (str1[idx] != str2[idx])
				return (str1[idx] - str2[idx]); // str1�� ũ�� > 0 ��ȯ, ������ < 0 ��ȯ
			idx++;
		}
		/* str1�� str2�� �Ȱ��� ��*/
		return 0;
	}

	char* strcpy(char* destination, const char* source) {
		int idx = 0;

		/* �ι��� ������ ��� ���� ����*/
		while (source[idx]) {
			destination[idx] = source[idx];
			idx++;
		}
		destination[idx] = 0; // �ι��� ����
		
		return destination;
	}
	char* strncpy(char* destination, const char* source, size_t num) {
		int idx = 0;

		/* �ι��� ������ num���� ���� ����*/
		while (source[idx] && idx < num) {
			destination[idx] = source[idx];
			idx++;
		}
		/* num���� ���ڸ� �����ϱ� ���� source ���ڿ��� ������
		���� ������ŭ null�� ä���*/
		while (idx < num) {
			destination[idx] = 0;
			idx++;
		}
		return destination;
	}

	size_t strlen(const char* str) {
		unsigned long long idx = 0;
		
		while (str[idx])
			idx++;

		return idx;
	}

	int atoi(const char* str) {
		int idx = 0;
		int sign = 1; // ��ȣ
		int num = 0;
		
		/* �Ǿ��ʿ� ��ġ�� �������� white space �ǳʶٱ�*/
		while ((9 <= str[idx] && str[idx]<=13) || str[idx] == ' ')
			idx++;
		
		/* ��ȣ ����ϱ�*/
		if(str[idx] == '+' || str[idx] == '-') {
			if (str[idx] == '-')
				sign *= (-1);
			idx++;
		}
		/* ���ڸ� ���ڷ� ��ȯ�Ͽ� num�� ���ϱ�*/
		while ('0' <= str[idx] && str[idx] <= '9') {
			num *= 10;
			num += str[idx] - '0';
			idx++;
		}
		/* ��ȿ�� ���ڰ� ���ų� ���� 0���� ������ �� 0 ��ȯ*/
		if (num == 0)
			return 0;
		/* ��ȣ ����ؼ� sign �����ֱ�*/
		else
			return sign * num;
	}
	double atof(const char* str) {
		int idx = 0;
		int sign = 1; // ��ȣ
		int integer = 0; // �����κ�
		int demical = 0; // �Ҽ��κ�
		int exponent = 0; // �����κ�
		double num = 0; // �Ǽ�

		/* �Ǿ��ʿ� ��ġ�� �������� white space �ǳʶٱ�*/
		while ((9 <= str[idx] && str[idx] <= 13) || str[idx] == ' ')
			idx++;

		/* ��ȣ ����ϱ�*/
		if (str[idx] == '+' || str[idx] == '-') {
			if (str[idx] == '-')
				sign *= (-1);
			idx++;
		}
		
		/* �����κ� ���ϱ�*/
		while ('0' <= str[idx] && str[idx] <= '9') {
			integer *= 10;
			integer += str[idx] - '0';
			idx++;
		}
		
		/* �����κ� ���� '.'�� ��ġ�Ѵٸ� ���� ���� �˻�*/
		if (str[idx] == '.') {
			idx++;
			/* .������ ���ڰ� ������ �Ҽ� demical�� ����*/
			while ('0' <= str[idx] && str[idx] <= '9') {
				demical *= 10;
				demical += str[idx] - '0';
				idx++;
			}
			/* .������ e��E�� ������*/
			if (str[idx] == 'e' || str[idx] == 'E') {
				int sign = 1; // �����κ��� ��ȣ
				idx++;
				if (str[idx] == '-') {
					sign = -sign;
					idx++;
				}
				/* e��E ���� ���ڸ� ���� exponent�� ����*/
				while ('0' <= str[idx] && str[idx] <= '9') {
					exponent *= 10;
					exponent += str[idx] - '0';
					idx++;
				}
				exponent *= sign;
			}
		}
		/* �����κ� ���� e��E�� ���´ٸ�*/
		else if (str[idx] == 'e' || str[idx] == 'E') {
			int sign = 1; // �����κ��� ��ȣ
			idx++;
			if (str[idx] == '-') {
				sign = -sign;
				idx++;
			}
			/* e��E ���� ���ڸ� ���� exponent�� ����*/
			while ('0' <= str[idx] && str[idx] <= '9') {
				exponent *= 10;
				exponent += str[idx] - '0';
				idx++;
			}
			exponent *= sign;
		}
		/* num�� �Ҽ��κ� ���� �����ֱ�*/
		while (demical > 0) {
			num += demical % 10;
			num /= 10;
			demical /= 10;
		}
		/* num�� �����κ� �����ֱ�*/
		num += integer;
		
		/* �����κ��� ����� �����κ� ũ�⸸ŭ 10 �����ֱ�*/
		for (; exponent > 0; exponent--)
			num *= 10;
		/* �����κ��� ������ �����κ� ũ�⸸ŭ 10 �����ֱ�*/
		for (; exponent < 0; exponent++)
			num /= 10;

		return sign * num; // ��ȣ ���
	}
}