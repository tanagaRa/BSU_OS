#include <iostream>
#include "Windows.h"


CRITICAL_SECTION workingWithAnArray;
CRITICAL_SECTION userInput;
HANDLE *hStartThread;
HANDLE *hThreads;
HANDLE *hExitThread;
HANDLE *hImpossibleToWork;
int *arr;
int size{0};
const int delay{5};
const int tread_flag_quantity{2};


DWORD WINAPI marker(int _threadNum) {
    WaitForSingleObject(hStartThread[(int) _threadNum], INFINITE);

    int threadNum{(int) _threadNum};
    int randomNum{0};
    int markedElements{0};
    HANDLE hThreadFlags[]{hStartThread[threadNum], hExitThread[threadNum]};
    DWORD dFlagNum{0};


    while (true) {
        randomNum = rand() % size;
        if (arr[randomNum] == 0) {
            EnterCriticalSection(&workingWithAnArray);
            Sleep(delay);
            arr[randomNum] = threadNum + 1;
            markedElements++;
            Sleep(delay);
            LeaveCriticalSection(&workingWithAnArray);
        } else {
            EnterCriticalSection(&workingWithAnArray);
            std::cout << "Thread number: " << threadNum + 1 << std::endl;
            std::cout << "Number of marked elements: " << markedElements << std::endl;
            std::cout << "Index impossible to mark: " << randomNum << std::endl;
            LeaveCriticalSection(&workingWithAnArray);
            SetEvent(hImpossibleToWork[threadNum]);
            ResetEvent(hStartThread[threadNum]);
            dFlagNum = WaitForMultipleObjects(tread_flag_quantity, hThreadFlags, FALSE, INFINITE);
            if (dFlagNum == WAIT_OBJECT_0 + 1) {
                EnterCriticalSection(&workingWithAnArray);
                for (std::size_t i = 0; i < size; i++)
                    if (arr[i] == threadNum + 1)
                        arr[i] = 0;
                LeaveCriticalSection(&workingWithAnArray);
                ExitThread(0);
            } else {
                ResetEvent(hImpossibleToWork[threadNum]);
                continue;
            }
        }
    }
}


int main() {
    int threadsQuantity{0};
    int threadToTerminate{0};
    int terminatedThreads{0};
    bool *isTerminated{nullptr};

    InitializeCriticalSection(&workingWithAnArray);
    InitializeCriticalSection(&userInput);

    std::cout << "Input size of array: ";
    std::cin >> size;
    arr = new int[size]{};
    std::cout << "Input number of threads: ";
    std::cin >> threadsQuantity;

    hThreads = new HANDLE[threadsQuantity];
    hExitThread = new HANDLE[threadsQuantity];
    hImpossibleToWork = new HANDLE[threadsQuantity];
    hStartThread = new HANDLE[threadsQuantity];
    isTerminated = new bool[threadsQuantity]{};

    for (std::size_t i = 0; i < threadsQuantity; i++) {
        hThreads[i] = CreateThread(NULL, 1, reinterpret_cast<LPTHREAD_START_ROUTINE>(marker), (LPVOID) (i), NULL, NULL);
        hStartThread[i] = CreateEvent(NULL, TRUE, FALSE,NULL);
        hExitThread[i] = CreateEvent(NULL, TRUE, FALSE,NULL);
        hImpossibleToWork[i] = CreateEvent(NULL, TRUE, FALSE,NULL);
    }

    while (terminatedThreads != threadsQuantity) {

        for (std::size_t i = 0; i < threadsQuantity; i++)
            if (!isTerminated[i]) {
                ResetEvent(hImpossibleToWork[i]);
                SetEvent(hStartThread[i]);
            }

        WaitForMultipleObjects(threadsQuantity, hImpossibleToWork, TRUE, INFINITE);

        EnterCriticalSection(&workingWithAnArray);
        std::cout << "ARRAY: ";
        for (std::size_t i = 0; i < size; i++)
            std::cout << arr[i] << ' ';
        std::cout << std::endl;
        LeaveCriticalSection(&workingWithAnArray);

        std::cout << "thread to terminate: ";
        std::cin >> threadToTerminate;
        if (isTerminated[threadToTerminate - 1])
            std::cout << "THIS THREAD HAS BEEN TERMINATED." << std::endl;
        else {
            isTerminated[threadToTerminate - 1] = true;
            terminatedThreads++;
            SetEvent(hExitThread[threadToTerminate - 1]);
            WaitForSingleObject(hThreads[threadToTerminate - 1], INFINITE);
            CloseHandle(hThreads[threadToTerminate - 1]);
            CloseHandle(hExitThread[threadToTerminate - 1]);
            CloseHandle(hStartThread[threadToTerminate - 1]);
        }

        EnterCriticalSection(&workingWithAnArray);
        std::cout << "Array: ";
        for (std::size_t i = 0; i < size; i++)
            std::cout << arr[i] << ' ';
        std::cout << std::endl;
        LeaveCriticalSection(&workingWithAnArray);
    }

    delete arr;
    arr = nullptr;
    delete isTerminated;
    isTerminated = nullptr;
    for (std::size_t i = 0; i < threadsQuantity; i++)
        CloseHandle(hImpossibleToWork[i]);

    std::cout << "ALL THREADS HAS BEEN TERMINATED." << std::endl;
}
