#include <windows.h>
#include <fstream>
#include <iostream>
#include <iomanip>
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

    ifstream in(argv[1], ios::binary | ios::in);
    ofstream out(argv[2]);
    out << "����� �� ����� �" << argv[1] << "�\n";
    out << "����� ����������, ��� ����������, ����, ��������\n";
    employee emp;
    while (!in.eof()) {
        in.read((char*)&emp, sizeof(emp));
        if (!in.eof())
            out << emp.num << ", " << emp.name << ", " << emp.hours << ", " << emp.hours * atoi(argv[3]) << '\n';
    }
    out.close();
    in.close();
    return 0;
}