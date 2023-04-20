#include <iostream>
#include <Windows.h> 
using std::endl;
using std::cin;
using std::cout;
int n;
HANDLE* hThread;
int markerCount;
CRITICAL_SECTION criticalSection;
struct numsThread {
	int* arr;
	int num;
	HANDLE start;
	HANDLE stop = CreateEvent(NULL, TRUE, FALSE, NULL);
	HANDLE* event = new HANDLE[2];
};
DWORD WINAPI marker(LPVOID _arrF)
{
	numsThread arrF = *(numsThread*)_arrF;
	srand(arrF.num);
	bool check = false;
	int count = 0;
	while(!check) {
		int temp = rand();
		temp = temp % n;
		EnterCriticalSection(&criticalSection);
		if (arrF.arr[temp] == 0) {
			Sleep(5);
			arrF.arr[temp] = arrF.num;
			Sleep(5);
			count += 1;
		}
		else {
			cout << arrF.num << " " << count << " " << temp <<endl;			
			LeaveCriticalSection(&criticalSection);
			SetEvent(arrF.stop);
			if (WaitForMultipleObjects(markerCount, hThread, FALSE, INFINITE) == WAIT_FAILED) {
				cout << "Error." << endl;
			}
			int k = WaitForMultipleObjects(2, arrF.event, FALSE, INFINITE) - WAIT_OBJECT_0;
			if (k == 0) {
				check = true;
			}
		}
	}
	for (int i = 0; i < n; i++) {
		if (arrF.arr[i] == arrF.num) {
			arrF.arr[i] = 0;
		}
	}
	return 0;
}
int main() {
	InitializeCriticalSection(&criticalSection);
	int* arr;
	DWORD *dwThread;
	cin >> n;
    	arr = new int[n];
	for (int i = 0; i < n; i++) {
		arr[i] = 0;
	}
	cin >> markerCount;
	hThread = new HANDLE[markerCount];
	dwThread = new DWORD[markerCount];
	numsThread* arrF = new numsThread[markerCount];
	bool* check = new bool[markerCount];
	HANDLE st = CreateEvent(NULL, TRUE, FALSE, NULL);
	HANDLE* stop = new HANDLE[markerCount];
	for (int i = 0; i < markerCount; i++) {
		arrF[i].arr = arr;
		arrF[i].num = i + 1;
		stop[i] = arrF[i].stop;
		arrF[i].event[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
		arrF[i].event[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
		hThread[i] = CreateThread(NULL, 0, marker, (LPVOID)(&arrF[i]), 0, &dwThread[i-1]);
		check[i] = false;
	}
		SetEvent(st);
	int end = 0;
	while (end != markerCount) {
		if (WaitForMultipleObjects(markerCount, hThread, TRUE, INFINITE) == WAIT_FAILED) {
			cout << "Error." << endl;
		}		
		for (int i = 0; i < n; i++) {
			cout << arr[i] << " ";
		}
		cout << endl;
		bool term = false;
		while (!term) {
			int index;
			cout << "Enter the number of the thread to be completed: ";
			cin >> index;
			index--;
			if (index >= markerCount || index < 0) {
				cout << "Error input" << endl;
			}
			else if (check[index]) {
				cout << "This thread was ended." << endl;
			}
			else {
				SetEvent(arrF[index].event[0]);
				WaitForSingleObject(hThread[index], INFINITE);
				for (int i = 0; i < n; i++) {
					cout << arr[i] << " ";
				}
				cout << endl;
				check[index] = true;
				term = true;
				end++;
			}
		}
		for (int i = 0; i < markerCount; i++) {
			if (!check[i]) {
				ResetEvent(arrF[i].stop);
				SetEvent(arrF[i].event[1]);
			}
		}
	}
	CloseHandle(stop);
	for (int i = 0; i < markerCount; i++) {
		CloseHandle(hThread[i]);
		CloseHandle(stop[i]);
		CloseHandle(arrF[i].event[0]);
		CloseHandle(arrF[i].event[1]);
	}
	DeleteCriticalSection(&criticalSection);
	return 0;
}