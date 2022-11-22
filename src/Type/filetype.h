#pragma once

#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <memory>
#include <ostream>
#include "compress.h"
namespace FileType {
enum FileType {
    zip,
    gz,
    unknown
};

FileType GetFileType(const std::string& filePath);
std::unique_ptr<Compress> MakeCompress(const std::string& filePath);
}