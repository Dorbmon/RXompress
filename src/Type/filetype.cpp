#include "filetype.h"
#include "impl/zip.h"
namespace FileType {
    std::unique_ptr<Compress> MakeCompress(const std::string& filePath) {
        auto type = GetFileType(filePath);
        switch (type) {
        case zip:
            return std::make_unique<Zip>(filePath);
        default:
            return nullptr;
        }
    }
    FileType GetFileType(const std::string& filePath) {
        std::ifstream input (filePath, std::ios::binary);
        if (!input.is_open()) {
            return unknown;
        }
        input.seekg(0, std::ios::beg);
        unsigned char magic[4] = {0};
        input.read((char*)magic, sizeof(magic));
        //0x504B0304;
        switch (*(uint32_t*)magic) {
        case 67324752:
            return zip;
        default:
            return unknown;
        }
    }
}