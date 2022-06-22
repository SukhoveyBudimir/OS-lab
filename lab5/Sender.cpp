#include <windows.h>
#include <iostream>
#include <string>
#include <codecvt>
#include <locale>
#include <conio.h>

using namespace std;

struct employee{
    int num; 
    char name[10];
    double hours; 
};

struct message {
    char type;
    int id;
};

int main() {
    HANDLE hPipe;
    DWORD cbRead, dwMode;
    while (1){
        WaitNamedPipe(L"\\\\.\\pipe\\named_pipe", INFINITE);
        hPipe = CreateFile(L"\\\\.\\pipe\\named_pipe", GENERIC_READ |GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);          
        if (hPipe != INVALID_HANDLE_VALUE)
            break;
 
        if (GetLastError() != ERROR_PIPE_BUSY){
            printf("Could not open pipe\n");
            return 0;
        }

        if (!WaitNamedPipe(L"\\\\.\\pipe\\named_pipe", 20000)){
            printf("Could not open pipe\n");
            return 0;
        }
    }

    dwMode = PIPE_READMODE_MESSAGE;
    boolean fSuccess = SetNamedPipeHandleState( hPipe, &dwMode, NULL, NULL);    
    if (!fSuccess){
        printf("SetNamedPipeHandleState failed.\n");
        return 0;
    }

    while (true) {
        cout << "1 - modify an entry\n2 - read the entry\n3 - exit\n";
        int ans;
        cin >> ans;
        if (ans == 1) {
            boolean ok = false;
            while (!ok) {
                cout << "Enter ID\n";
                int id;
                cin >> id;
                HANDLE* mutex = new HANDLE[2];
                mutex[0] = OpenMutex(MUTEX_ALL_ACCESS, true, (LPWSTR)wstring_convert<codecvt_utf8<wchar_t>>().from_bytes("r" + to_string(id)).c_str());
                if (mutex[0] != NULL) {
                    ok = true;
                    mutex[1] = OpenMutex(MUTEX_ALL_ACCESS, false, (LPWSTR)wstring_convert<codecvt_utf8<wchar_t>>().from_bytes("w" + to_string(id)).c_str());
                    cout << "Wait, access to the record may be blocked...\n";
                    WaitForMultipleObjects(2, mutex, true, INFINITE);
                    message msg;
                    msg.id = id;
                    msg.type = 'w';
                    employee emp;
                    TransactNamedPipe(
                        hPipe,                  
                        (char*)&msg,              // message to server
                        sizeof(msg), // message length 
                        (char*)&emp,              // buffer to receive reply
                        sizeof(emp),  // size of read buffer
                        &cbRead,                // bytes read
                        NULL);                  // not overlapped 
                    cout << "Имя сотрудника: " << emp.name << "\nЧисло отработанных часов: " << emp.hours << "\nВведите изменённое имя\n";
                    cin >> emp.name;
                    cout << "\nВведите изменённое число часов\n";
                    cin >> emp.hours;
                    DWORD dwRead;
                    WriteFile(hPipe, (char*)&emp, sizeof(emp), &dwRead, NULL);
                    cout << "\nНажмите любую клавишу, чтобы завершить доступ\n";
                    _getch();
                    ReleaseMutex(mutex[0]);
                    ReleaseMutex(mutex[1]);
                }
                else {
                    cout << "\nСотрудника с таким номером нет\n";
                }
            }
        }
        else if (ans == 2) {
            boolean ok = false;
            while (!ok) {
                cout << "\nВведите ID\n";
                int id;
                cin >> id;
                HANDLE mutex;
                mutex = OpenMutex(MUTEX_ALL_ACCESS, false, (LPWSTR)wstring_convert<codecvt_utf8<wchar_t>>().from_bytes("w" + to_string(id)).c_str());
                if (mutex != NULL) {
                    ok = true;
                    cout << "\nПодождите, доступ к записи может быть заблокирован...\n";
                    WaitForSingleObject(mutex, INFINITE);
                    message msg;
                    msg.id = id;
                    msg.type = 'r';
                    employee emp;
                    TransactNamedPipe( hPipe, (char*)&msg, sizeof(msg), (char*)&emp, sizeof(emp), &cbRead, NULL);                   
                    cout << "Employee's name: " << emp.name << "\nNumber of hours worked: " << emp.hours << "\nPress any key to terminate access\n";
                    _getch();
                    ReleaseMutex(mutex);
                }
                else {
                    cout << "\nThere is no employee with this number\n";
                }
            }
        }
        else {
            return 0;
        }
    }
}