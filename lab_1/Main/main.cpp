#pragma warning(disable:4996)
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
#include <cstring>
#include "..\Reporter\Reporter.h"

void Console_report(char* name) {
	std::ifstream in(name);
	std::string next_line;
	while (getline(in, next_line)) {
		std::cout << next_line << std::endl;
	}
}

char* InputForCreator(std::istream& is, char* binfile_name, int& number) {
	std::cout << "The name of binary file: ";
	char str[50];
	std::cin >> str;
	strcpy(binfile_name, str);

	std::cout << "The number of employees: ";
	std::cin >> number;
	std::string s = std::to_string(number);
	char const* num = s.c_str();

	char* command_line = new char[strlen(binfile_name) + strlen(num) + 2];
	strcpy(command_line, binfile_name);
	strcat(command_line, " ");
	strcat(command_line, num);
	return command_line;
}

char* InputForReporter(std::istream& is, char* binfile_name, char* reportfile_name, double& payment_by_hour) {
	std::cout << "The name of report file: ";
	char str[50];
	std::cin >> str;
	strcpy(reportfile_name, str);

	std::cout << "The payment by hour: ";
	std::cin >> payment_by_hour;
	std::string s = std::to_string(payment_by_hour);
	char const* num = s.c_str();

	char* command_line = new char[strlen(binfile_name) + strlen(reportfile_name) + strlen(num) + 3];
	strcpy(command_line, binfile_name);
	strcat(command_line, " ");
	strcat(command_line, reportfile_name);
	strcat(command_line, " ");
	strcat(command_line, num);
	return command_line;
}

LPWSTR CharToLPWSTR(char* str) {
	wchar_t wtext[50];
	std::mbstowcs(wtext, str, strlen(str) + 1);
	LPWSTR ptr = wtext;
	return ptr;
}

LPWSTR GetIpCommandLine(char* app_name, char* command_line) {
	char* lpszCommandLine = new char[strlen(app_name) + strlen(command_line) + 2];
	strcpy(lpszCommandLine, app_name);
	strcat(lpszCommandLine, " ");
	strcat(lpszCommandLine, command_line);

	return CharToLPWSTR(lpszCommandLine);
}

int main()
{
	char binfile_name[50];
	int number = 0;

	char* creator_command_line = InputForCreator(std::cin, binfile_name, number);
	char creator_app_name[12] = "Creator.exe";

	LPWSTR ip_line_creator = GetIpCommandLine(creator_app_name, creator_command_line);

	STARTUPINFO si1;
	PROCESS_INFORMATION piCom1;
	ZeroMemory(&si1, sizeof(STARTUPINFO));
	si1.cb = sizeof(STARTUPINFO);
	if (!CreateProcess(NULL, ip_line_creator, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si1, &piCom1))
	{
		_cputs("The new process is not created.\n");
		_cputs("Check a name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}
	_cputs("The Creator process is created.\n");
	WaitForSingleObject(piCom1.hProcess, INFINITE);
	CloseHandle(piCom1.hThread);
	CloseHandle(piCom1.hProcess);

	char reportfile_name[50];
	double payment_by_hour = 0;

	char* reporter_command_line = InputForReporter(std::cin, binfile_name, reportfile_name, payment_by_hour);
	char reporter_app_name[13] = "Reporter.exe";

	LPWSTR ip_line_reporter = GetIpCommandLine(reporter_app_name, reporter_command_line);

	STARTUPINFO si2;
	PROCESS_INFORMATION piCom2;
	ZeroMemory(&si2, sizeof(STARTUPINFO));
	si2.cb = sizeof(STARTUPINFO);
	if (!CreateProcess(NULL, ip_line_reporter, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si2, &piCom2))
	{
		_cputs("The new process is not created.\n");
		_cputs("Check a name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}
	_cputs("The Reporter process is created.\n");
	WaitForSingleObject(piCom2.hProcess, INFINITE);
	CloseHandle(piCom2.hThread);
	CloseHandle(piCom2.hProcess);
	Console_report(reportfile_name);
	return 0;
}
