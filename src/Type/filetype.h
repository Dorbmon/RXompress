#pragma once

#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
namespace FileType {
enum FileType {
    zip,
    gz
};

static FileType GetFileType(const std::string& filePath) {
    std::ifstream input (filePath, std::ios::binary);
    input.seekg(0, std::ios::beg);
    unsigned char magic[4] = {0};
    input.read((char*)magic, sizeof(magic));
    //0x504B0304;
    std::cout << *(uint32_t*)magic << std::endl;
}
}