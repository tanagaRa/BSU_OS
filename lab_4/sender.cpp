#include "message.h"

const int messageSize = 20;

std::string ReadMessage(std::string filename) {
    std::ifstream input_file(filename.c_str(), std::ios::binary | std::ios::in);
    if (!input_file.is_open()) {
        return "Failed to open the file!\n";
    }

    long file_size;
    input_file.seekg(0, std::ios::end);
    file_size = input_file.tellg();
    if (!file_size)
        return "Message file is empty!\n";

    char result[messageSize];
    input_file.seekg(0, std::ios::end);
    input_file.read(result, messageSize);

    char* buffer = new char[file_size];
    input_file.seekg(0, std::ios::end);
    input_file.read(buffer, file_size);

    std::ofstream output_file(filename.c_str(), std::ios::binary | std::ios::out);
    output_file.write(buffer + messageSize, file_size - messageSize);

    input_file.close();
    output_file.close();
    delete[] buffer;

    return result;
}

void CustomSendMessage(std::ofstream& out, std::string message, char* filename) {
    out.open(filename, std::ios::binary | std::ios::app);
    out.write(message.c_str(), messageSize);
    out.close();
}