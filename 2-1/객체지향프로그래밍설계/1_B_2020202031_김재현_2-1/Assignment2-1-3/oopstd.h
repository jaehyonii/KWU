#pragma once

namespace oopstd {
	void* memset(void* ptr, int value, size_t num) {
		unsigned char* p = (unsigned char*)ptr; // ptr을 char*형으로 캐스팅
		unsigned char data = (unsigned char)value; // data를 unsigned char형으로 캐스팅

		/* num바이트만큼 반복. 1바이트마다 value값으로 초기화*/
		for (int i = 0; i < num; i++) {
			*p = value;
			p++;
		}
		return ptr; // ptr 반환
	}
	void* memcpy(void* destination, const void* source, size_t num) {
		/* 바이트 단위로 복사를 진행하기 때문에 unsigned char* 형으로 캐스팅*/
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
			/*str1과 str2가 달라지는 시점에서*/
			if (str1[idx] != str2[idx])
				return (str1[idx] - str2[idx]); // str1이 크면 > 0 반환, 작으면 < 0 반환
			idx++;
		}
		/* str1과 str2가 똑같을 때*/
		return 0;
	}
	int strncmp(const char* str1, const char* str2, size_t num) {
		int idx = 0;

		while ((str1[idx] || str2[idx]) && idx < num) {
			/*str1과 str2가 달라지는 시점에서*/
			if (str1[idx] != str2[idx])
				return (str1[idx] - str2[idx]); // str1이 크면 > 0 반환, 작으면 < 0 반환
			idx++;
		}
		/* str1과 str2가 똑같을 때*/
		return 0;
	}

	char* strcpy(char* destination, const char* source) {
		int idx = 0;

		/* 널문자 전까지 모든 문자 복사*/
		while (source[idx]) {
			destination[idx] = source[idx];
			idx++;
		}
		destination[idx] = 0; // 널문자 삽입
		
		return destination;
	}
	char* strncpy(char* destination, const char* source, size_t num) {
		int idx = 0;

		/* 널문자 전까지 num개의 문자 복사*/
		while (source[idx] && idx < num) {
			destination[idx] = source[idx];
			idx++;
		}
		/* num개의 문자를 복사하기 전에 source 문자열이 끝나면
		남은 개수만큼 null로 채우기*/
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
		int sign = 1; // 부호
		int num = 0;
		
		/* 맨앞쪽에 위치한 연속적인 white space 건너뛰기*/
		while ((9 <= str[idx] && str[idx]<=13) || str[idx] == ' ')
			idx++;
		
		/* 부호 고려하기*/
		if(str[idx] == '+' || str[idx] == '-') {
			if (str[idx] == '-')
				sign *= (-1);
			idx++;
		}
		/* 문자를 숫자로 변환하여 num에 더하기*/
		while ('0' <= str[idx] && str[idx] <= '9') {
			num *= 10;
			num += str[idx] - '0';
			idx++;
		}
		/* 유효한 문자가 없거나 문자 0만이 존재할 때 0 반환*/
		if (num == 0)
			return 0;
		/* 부호 고려해서 sign 곱해주기*/
		else
			return sign * num;
	}
	double atof(const char* str) {
		int idx = 0;
		int sign = 1; // 부호
		int integer = 0; // 정수부분
		int demical = 0; // 소수부분
		int exponent = 0; // 지수부분
		double num = 0; // 실수

		/* 맨앞쪽에 위치한 연속적인 white space 건너뛰기*/
		while ((9 <= str[idx] && str[idx] <= 13) || str[idx] == ' ')
			idx++;

		/* 부호 고려하기*/
		if (str[idx] == '+' || str[idx] == '-') {
			if (str[idx] == '-')
				sign *= (-1);
			idx++;
		}
		
		/* 정수부분 구하기*/
		while ('0' <= str[idx] && str[idx] <= '9') {
			integer *= 10;
			integer += str[idx] - '0';
			idx++;
		}
		
		/* 정수부분 다음 '.'이 위치한다면 다음 문자 검사*/
		if (str[idx] == '.') {
			idx++;
			/* .다음에 숫자가 나오면 소수 demical에 저장*/
			while ('0' <= str[idx] && str[idx] <= '9') {
				demical *= 10;
				demical += str[idx] - '0';
				idx++;
			}
			/* .다음에 e나E가 나오면*/
			if (str[idx] == 'e' || str[idx] == 'E') {
				int sign = 1; // 지수부분의 부호
				idx++;
				if (str[idx] == '-') {
					sign = -sign;
					idx++;
				}
				/* e나E 다음 숫자를 지수 exponent에 저장*/
				while ('0' <= str[idx] && str[idx] <= '9') {
					exponent *= 10;
					exponent += str[idx] - '0';
					idx++;
				}
				exponent *= sign;
			}
		}
		/* 정수부분 다음 e나E가 나온다면*/
		else if (str[idx] == 'e' || str[idx] == 'E') {
			int sign = 1; // 지수부분의 부호
			idx++;
			if (str[idx] == '-') {
				sign = -sign;
				idx++;
			}
			/* e나E 다음 숫자를 지수 exponent에 저장*/
			while ('0' <= str[idx] && str[idx] <= '9') {
				exponent *= 10;
				exponent += str[idx] - '0';
				idx++;
			}
			exponent *= sign;
		}
		/* num에 소수부분 먼저 더해주기*/
		while (demical > 0) {
			num += demical % 10;
			num /= 10;
			demical /= 10;
		}
		/* num에 정수부분 더해주기*/
		num += integer;
		
		/* 지수부분이 양수면 지수부분 크기만큼 10 곱해주기*/
		for (; exponent > 0; exponent--)
			num *= 10;
		/* 지수부분이 음수면 지수부분 크기만큼 10 나눠주기*/
		for (; exponent < 0; exponent++)
			num /= 10;

		return sign * num; // 부호 고려
	}
}