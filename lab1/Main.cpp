#include<windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <codecvt>
#include <locale>
using namespace std;

struct employee
{
	int num;
	char name[10];
	double hours;
};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	STARTUPINFO si1;
	PROCESS_INFORMATION pi1;
	ZeroMemory(&si1, sizeof(STARTUPINFO));
	si1.cb = sizeof(STARTUPINFO);
	string str1 = "C:\\Users\\budim\\source\\repos\\os1\\Debug\\os1.exe ";

	STARTUPINFO si2;
	PROCESS_INFORMATION pi2;
	ZeroMemory(&si2, sizeof(STARTUPINFO));
	si2.cb = sizeof(STARTUPINFO);
	string str2 = "C:\\Users\\budim\\source\\repos\\os1Reporter\\Debug\\os1Reporter.exe ";

	char fileName1[20];
	int recordsNum;
	cout << "¬ведите им€ бинарного файла\n";
	cin >> fileName1;
	cout << "¬ведите количество записей\n";
	cin >> recordsNum;
	CreateProcess(NULL, (LPWSTR)wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(str1 + fileName1 + " " + to_string(recordsNum)).c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si1, &pi1);
	WaitForSingleObject(pi1.hProcess, INFINITE);
	CloseHandle(pi1.hThread);
	CloseHandle(pi1.hProcess);

	ifstream fin(fileName1, ios::binary | ios::in);
	employee emp;
	while (!fin.eof()) {
		fin.read((char*)&emp, sizeof(emp));
		if (!fin.eof())
			cout << emp.num << ", " << emp.name << ", " << emp.hours << '\n';
	}

	char fileName2[20];
	cout << "¬ведите им€ файла отчЄта\n";
	cin >> fileName2;
	int payment;
	cout << "¬ведите оплату за час работы\n";
	cin >> payment;
	CreateProcess(NULL, (LPWSTR)wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(str2 + fileName1 + " " + fileName2 + " " + to_string(payment)).c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si2, &pi2);
	WaitForSingleObject(pi2.hProcess, INFINITE);
	CloseHandle(pi2.hThread);
	CloseHandle(pi2.hProcess);
	fin.close();
	ifstream fin1(fileName2);
	char ch;
	while (!fin1.eof()) {
		fin1.read(&ch, 1);
		if (!fin1.eof()) {
			cout << ch;
		}
	}
	fin1.close();
	return 0;
}