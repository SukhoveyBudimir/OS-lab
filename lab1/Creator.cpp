#include <windows.h>
#include <fstream>
#include <iostream>
using namespace std;

struct employee
{
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    ofstream out(argv[1], ios::binary | ios::out);
    employee emp;
    for (int i = 0; i < atoi(argv[2]); i++) {
        cout << "������� ����� ����������\n";
        cin >> emp.num;
        cout << "������� ��� ����������\n";
        cin >> emp.name;
        cout << "������� ���������� ������������ �����\n";
        cin >> emp.hours;
        out.write((char*)&emp, sizeof(emp));
    }
    out.close();
    return 0;
}