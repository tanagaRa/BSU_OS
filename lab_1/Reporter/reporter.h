#pragma once
#include "../Creator/creator.h"
#include <iomanip>

void read_from_bin_file(char* bin_filename, char* report_name, int amount) {
    employee temp;

    std::ifstream fin(bin_filename, std::ios_base::binary);
    std::ofstream fout(report_name);

    fout << std::setw(30) << " "
         << "Report based on " << bin_filename << ".\n";
    fout << std::setw(20) << "Employees' num " 
         << std::setw(20) << "Name "
         << std::setw(20) << "hours " 
         << std::setw(20) << "salary\n";

    while (fin.read((char*)&temp,sizeof(employee))) {
        fout << std::setw(20) << temp.num 
             << std::setw(20) << temp.name 
             << std::setw(20) << temp.hours 
             << std::setw(20) << temp.hours * amount << std::endl;
    }     
}