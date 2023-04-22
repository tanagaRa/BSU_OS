#include <sstream>
#include "message.h"
#include "winuser.h"
#include <windows.h>
#include <conio.h> 
#include <stdio.h> 
#include <iostream>
#include <fstream>


CRITICAL_SECTION cs;
HANDLE* readyEvents;

template <typename T>
std::string to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

int LaunchSenders(int n_senders, std::string filename) {
    readyEvents = new HANDLE[n_senders];

    for (int i = 0; i < n_senders; ++i) {
        std::string event_name = "sender";
        event_name += to_string(i);
        readyEvents[i] = CreateEvent(NULL, TRUE, FALSE, LPCWSTR("sender"));
        if (readyEvents[i] == NULL) {
            std::cout << "Failed to create the event!\n";
            return GetLastError();
        }
        std::string exe = "Sender.exe";
        exe += " " + filename + " " + event_name;
        LPCTSTR lpszAppName = L"Sender.exe";
        STARTUPINFO* startup_information = new STARTUPINFO[n_senders];
        PROCESS_INFORMATION* process_information = new PROCESS_INFORMATION[n_senders];
        for (int i = 0; i < n_senders; i++)
        {
            ZeroMemory(&startup_information[i], sizeof(STARTUPINFO));
            startup_information[i].cb = sizeof(STARTUPINFO);
            char* lp_arguments = (char*)(exe.c_str());
            if (!CreateProcess(lpszAppName, LPWSTR(lp_arguments), NULL, NULL, FALSE,
                CREATE_NEW_CONSOLE, NULL, NULL, &startup_information[i], &process_information[i])) {
                std::cout << "Process creation error!\n";
                CloseHandle(process_information[i].hProcess);
                CloseHandle(process_information[i].hThread);
            }
        }
    }
    std::cout << "Receiver process created " << n_senders << " senders\n";
}

int main() {
    std::string filename;
    std::ofstream fileStrmOut("file.bin", std::ios::binary);
    std::cout << "Enter binary file name: ";
    std::cin >> filename;
    std::fstream input_file(filename.c_str(), std::ios::binary | std::ios::out);

    int n_senders;
    std::cout << "Enter number of sender processes: ";
    std::cin >> n_senders;

    InitializeCriticalSection(&cs);
    HANDLE start_all = CreateEvent(NULL, TRUE, FALSE, LPCWSTR("START_ALL"));
    HANDLE file_mutex = CreateMutex(NULL, FALSE, LPCWSTR("FILE_ACCESS"));

    if (file_mutex == NULL) {
        std::cout << "Failed to create Mutex!\n";
        return GetLastError();
    }

    HANDLE sender_semaphore = CreateSemaphore(NULL, 0, n_senders, LPCWSTR("MESSAGES_COUNT_SEM"));
    HANDLE read_event = CreateEvent(NULL, FALSE, FALSE, LPCWSTR("MESSAGE_READ"));
    if (sender_semaphore == NULL || read_event == NULL)
        return GetLastError();

    LaunchSenders(n_senders, filename);
    WaitForMultipleObjects(n_senders, readyEvents, TRUE, INFINITE);
    std::cout << "All senders are ready. Starting...\n";
    SetEvent(start_all);
    char buffer[20];
    char message[20];

    while (true) {
        std::cin >> buffer;
        if (std::cin.eof())
            break;
        std::cout << "Waiting for a message...\n";
        WaitForSingleObject(sender_semaphore, INFINITE);
        WaitForSingleObject(file_mutex, INFINITE);
        std::cout << ReadMessage(filename);
        ReleaseMutex(file_mutex);
        SetEvent(read_event);
    }

    input_file.close();
    delete[] readyEvents;
    DeleteCriticalSection(&cs);

    return 0;
}
