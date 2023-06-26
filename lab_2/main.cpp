#include <iostream>
#include <Windows.h>

static int n;
volatile int min;
volatile int max;
volatile int arithmeticMean;

// поток для поиска max и min чисел
DWORD WINAPI min_max(void* iArr)
{
	int* arr = (int*)iArr;
	min = arr[0];
	max = arr[0];
	for (int i = 0; i < n; i++) {
		if (min > arr[i]) {
			min = arr[i];
		}
		Sleep(7);
		if (max < arr[i]) {
			max = arr[i];
		}
		Sleep(7);
	}
	std::cout << "Min: " << min << " Max: " << max << std::endl;
	return 0;
}

// поток для подсчета арифметического среднего всех чисел
DWORD WINAPI average(void* iArr)
{
	int* arr = (int*)iArr;
	int sum = 0;
	for (int i = 0; i < n; i++) {
		sum += arr[i];
		Sleep(12);
	}
	arithmeticMean = sum / n;
	std::cout << "Arithmetic mean: " << arithmeticMean << std::endl;
	return 0;
}

int main() {
	HANDLE hThread1;
	DWORD IDThread1;
	HANDLE hThread2;
	DWORD IDThread2;
	
	std::cout << "Input size of the array: " << std::endl;
	std::cin >> n;
	int* arr = new int[n];
	
	std::cout << "Input the elements of the array:" << std::endl;
	for (int i = 0; i < n; i++) {
		std::cin >> arr[i];
	}
	// обработка исключений
	hThread1 = CreateThread(NULL, 0, min_max, arr, 0, &IDThread1);
	if (hThread1 == NULL) {
		delete[] arr;
		return GetLastError();
	}
	hThread2 = CreateThread(NULL, 0, average, arr, 0, &IDThread2);
	if (hThread2 == NULL) {
		delete[] arr;
		return GetLastError();
	}
	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);

	// max и min приравниваем к среднему
	for (int i = 0; i < n; i++) {
		if (arr[i] == min || arr[i] == max) {
			arr[i] = arithmeticMean;
		}
	}

	// выводим изменённый массив
	std::cout << "Changed array: " << std::endl;
	for (int i = 0; i < n; i++) {
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;

	// очищение памяти
	delete[] arr;
	return 0;
}
