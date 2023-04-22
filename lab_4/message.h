#pragma once

#include <iostream>
#include <fstream>
#include <Windows.h>

const int messageSize = 20;

std::string ReadMessage(std::string filename);

void CustomSendMessage(std::ofstream& out, std::string message, char* filename);

#endif