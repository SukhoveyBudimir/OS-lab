#include <Windows.h>
#include <iostream>

struct massive {
	int min = 0;
	int max = 0;
	int size;
	int* arr = NULL;
	int average = 0;
};

DWORD WINAPI max_min(LPVOID a) {
	printf("Stream max_min start: \n");

	massive* mas = (massive*)a;

	mas->min = mas->arr[0];
	mas->max = mas->arr[0];

	for (int i = 0; i < mas->size; i++) {
		if (mas->max < mas->arr[i]) {
			mas->max = mas->arr[i];
		} Sleep(7);
		if (mas->min > mas->arr[i]) {
			mas->min = mas->arr[i];
		} Sleep(7);
	}

	printf("Min: %d %s", mas->min, "\n");
	printf("Max: %d %s", mas->max, "\n");
	printf("Stream min_max close\n");
	return 0;
}

DWORD WINAPI average(LPVOID a) {
	printf("Stream average start: \n");

	massive* mas = (massive*)a;

	for (int i = 0; i < mas->size; i++) {
		mas->average += mas->arr[i];
	}
	mas->average /= mas->size;

	printf("average: %d %s", mas->average, "\n");
	printf("Stream average close\n");
	return 0;
}

int main() {
	int size;
	printf("Input suze of your array: ");
	scanf_s("%d", &size);
	int* array = new int[size];
	printf("Input elements of your array: ");
	for (int i = 0; i < size; i++) {
		scanf_s("%d", &array[i]);
	}

	struct massive* mas = new massive;
	mas->size = size;
	mas->arr = array;

	LPTHREAD_START_ROUTINE mm = (LPTHREAD_START_ROUTINE)max_min;
	HANDLE hThread_mm;
	DWORD IDThread_mm;
	hThread_mm = CreateThread(NULL, 0, mm, (LPVOID)mas, 0, &IDThread_mm);
	if (hThread_mm == NULL)
		return GetLastError();
	LPTHREAD_START_ROUTINE av = (LPTHREAD_START_ROUTINE)average;
	HANDLE hThread_av;
	DWORD IDThread_av;
	hThread_av = CreateThread(NULL, 0, av, (LPVOID)mas, 0, &IDThread_av);
	if (hThread_av == NULL)
		return GetLastError();

	WaitForSingleObject(hThread_mm, INFINITE);
	WaitForSingleObject(hThread_av, INFINITE);

	CloseHandle(hThread_mm);
	CloseHandle(hThread_av);

	for (int i = 0; i < size; i++) {
		if (array[i] == mas->max) {
			array[i] = mas->average;
		}
		if (array[i] == mas->min) {
			array[i] = mas->average;
		}
	}

	printf("New array: ");

	for (int i = 0; i < size; i++) {
		std::cout << array[i] << " ";
	}

	delete[] array;
	delete mas;
	system("pause");
	return 0;
}