#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <cstdlib>
#include "oopstd.h"

using namespace std;

struct {
	char name[40];
}person, copy1, copy2;

int main()
{
	/* memset 실험*/
	cout << "=========================memset==============================" << endl;
	char str1[] = "almost every programmer should know memset!";
	char arr1[] = "almost every programmer should know memset!";
	cout << "before memset: " << str1 << endl;

	memset(str1, '-', 6);
	cout << "after std::memset: " << str1 << endl;
	oopstd::memset(arr1, '-', 6);
	cout << "after oopstd::memset: " << arr1 << endl << endl;;
	

	/* 배열 대상 memcpy 실험*/
	cout << "=========================memcpy==============================" << endl;
	char str2[100] = "Hello";
	char arr2[100] = "Hello";
	char str3[] = "HiHi";
	cout << "before memcpy: " << str2 << endl;

	memcpy(str2 + 5, str3, strlen(str3) + 1);
	cout << "after std::memcpy: " << str2 << endl;
	oopstd::memcpy(arr2 + 5, str3, strlen(str3) + 1);
	cout << "after oopstd::memcpy: " << arr2 << endl <<endl;


	/* 구조체 대상 memcpy 실험*/
	char myname[] = "Pierre de Fermat";
	memcpy(person.name, myname, strlen(myname) + 1);
	cout << "before memcpy: " << person.name << endl;

	memcpy(&copy1, &person, sizeof(person));
	cout << "after std::memcpy: " << copy1.name << endl;
	oopstd::memcpy(&copy2, &person, sizeof(person));
	cout << "after oopstd::memcpy: " << copy2.name << endl << endl;


	/* strcmp 실험*/
	cout << "=========================strcmp==============================" << endl;
	char key1[] = "orange";
	char key2[] = "orange";
	char key3[] = "apple";
	char key4[] = "watermelon";
	
	cout << "std::strcmp(\"orange\", \"orange\"): " << strcmp(key1, key2) << endl;
	cout << "oopstd::strcmp(\"orange\", \"orange\"): " << oopstd::strcmp(key1, key2) << endl;
	cout << "std::strcmp(\"orange\", \"apple\"): " << strcmp(key1, key3) << endl;
	cout << "oopstd::strcmp(\"orange\", \"apple\"): " << oopstd::strcmp(key1, key3) << endl;
	cout << "std::strcmp(\"orange\", \"watermelon\"): " << strcmp(key1, key4) << endl;
	cout << "oopstd::strcmp(\"orange\", \"watermelon\"): " << oopstd::strcmp(key1, key4) << endl << endl;


	/* strncmp 실험*/
	cout << "=========================strncmp==============================" << endl;
	char key5[] = "apple";
	char key6[] = "applemango";
	
	cout << "std::strncmp(\"apple\", \"applemango\", 5): " << strncmp(key5, key6,5) << endl;
	cout << "oopstd::strncmp(\"apple\", \"applemango\", 5): " << oopstd::strncmp(key5, key6, 5) << endl;
	cout << "std::strncmp(\"apple\", \"applemango\", 8): " << strncmp(key5, key6, 8) << endl;
	cout << "oopstd::strncmp(\"apple\", \"applemango\", 8): " << oopstd::strncmp(key5, key6, 8) << endl;
	cout << "std::strncmp(\"apple\", \"applemango\", 3): " << strncmp(key5, key6, 3) << endl;
	cout << "oopstd::strncmp(\"apple\", \"applemango\", 3): " << oopstd::strncmp(key5, key6, 3) << endl << endl;

	
	/* strcpy 실험*/
	cout << "=========================strcpy==============================" << endl;
	char str4[] = "Sample string";
	char str5[14];
	char str6[14];

	strcpy(str5, str4);
	cout << "std::strcpy(str5, \"Sample string\"): " << str5 << endl;
	oopstd::strcpy(str6, str4);
	cout << "oopstd::strcpy(str6, \"Sample string\"): " << str6 << endl << endl;

	
	/* strncpy 실험*/
	cout << "=========================strncpy==============================" << endl;
	char str7[40];
	char str8[40];

	strncpy(str7, str4, 6);
	cout << "std::strncpy(str5, \"Sample string\", 6): " << str7 << endl;
	oopstd::strncpy(str8, str4, 6);
	cout << "oopstd::strncpy(str6, \"Sample string\", 6): " << str8 << endl;

	strncpy(str7, str4, 15);
	cout << "std::strncpy(str5, \"Sample string\", 15): " << str7 << endl;
	cout << "str7[14]: " << str7[14] << endl;
	oopstd::strncpy(str8, str4, 15);
	cout << "oopstd::strncpy(str6, \"Sample string\", 15): " << str8 << endl << endl;
	cout << "str8[14]: " << str8[14] << endl;
	

	/* strlen 실험*/
	cout << "=========================strlen==============================" << endl;
	char str9[] = "HelloMyName";
	
	cout << "str: HelloMyName" << endl;
	cout << "std::strlen(str): " << strlen(str9) << endl;
	cout << "oopstd::strlen(str): " << oopstd::strlen(str9) << endl << endl;


	/* atoi 실험*/
	cout << "=========================atoi==============================" << endl;
	char a[] = "     \t\n\v\f\r +-12345";
	cout << "arr:     \\t\\n\\v\\f\\r +-12345" << endl;
	
	cout << "std::atoi(arr): " << atoi(a) << endl;
	cout << "oopstd::atoi(arr): " << oopstd::atoi(a) << endl << endl;
	

	char b[] = "     \t\n\v\f\r -12345abc";
	cout << "arr:     \\t\\n\\v\\f\\r -12345abc" << endl;
	
	cout << "std::atoi(arr): " << atoi(b) << endl;
	cout << "oopstd::atoi(arr): " << oopstd::atoi(b) << endl << endl;

	
	/* atof 실험*/

	cout << "=========================atof==============================" << endl;
	char c[] = "   \t  \n  1234.5987e10";
	cout << "arr:    \\t  \\n  1234.5987e10" << endl;
	cout << "std::atof(arr): " << atof(c) << endl;
	cout << "oopstd::atof(arr): " << oopstd::atof(c) << endl;
	
	return 0;
}