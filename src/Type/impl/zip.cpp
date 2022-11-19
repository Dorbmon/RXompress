#include "zip.h"
#include "glibmm/ustring.h"
#include <fmt/format.h>
#include <iostream>
#include <utility>
#include <zip.h>
#include <zipconf.h>
void Zip::reloadFiles() {
    //auto n = (this->zip);
    auto n = zip_get_num_entries(this->zip, 0);
    for (zip_int64_t i = 0; i < n; ++i) {
        //zip_entry_openbyindex(this->zip, i);
        //zip_open();
        auto file = zip_fopen_index(this->zip, i, 0);
        auto name = std::string(zip_get_name(this->zip, i, 0));
        //int isdir = zip_entry_isdir(zip);
        //unsigned long long size = zip_entry_size(zip);
        //std::cout << name << " " << name.back() << std::endl;
        zip_stat_t stat;
        zip_stat_index(this->zip, i, 0, &stat);
        bool isDir = false;
        //std::cout << name << std::endl;
        if (name.back() == '/') {
            isDir = true;
            name.pop_back();
        }
        
        Compress::fileTree->InsertFile(
            name, 
            std::make_shared<FileItem>(name, isDir, stat.size, (void*)i));
    }
}

Zip::Zip(const std::string& fileName) : fileName(fileName) {
    this->zip = zip_open(fileName.c_str(), ZIP_CREATE, nullptr);
    this->currentPath = "/";
    this->reloadFiles();
}

Zip::~Zip() {
    zip_close(this->zip);
}
bool Zip::ChangeName(void *meta, std::string &newName) {
    auto index = (zip_uint64_t)meta;
    auto name = std::string(zip_get_name(this->zip, index, 0));
    size_t end = 0;
    bool isDir = false;
    if (name.ends_with("/")) {
        isDir = true;
    }
    auto pos = name.find_last_of("/", name.length() - 2);
    if (pos != std::string::npos) {
        end = pos;
    } else {
        end = -1;
    }
    auto prefix = name.substr(0, end + 1);
    std::cout << index << " prefix of :" << name << " is " << prefix << std::endl;
    if (isDir) {
        name = fmt::format("{}{}/", prefix, newName);
    } else {
        name = fmt::format("{}{}", prefix, newName);
    }
    std::cout << "set new name " << name << std::endl;
    int ret;
    if (isDir) {
        ret = zip_file_rename(this->zip, index, name.c_str(), 0);
    } else {
        ret = zip_rename(this->zip, index, name.c_str());
    }
    return ret == 0;
}
std::pair<bool, std::string> Zip::Save() {
    zip_close(this->zip);
    int error;
    this->zip = zip_open(this->fileName.c_str(), ZIP_CREATE, &error);
    if (this->zip == nullptr) {
        return std::make_pair(false, fmt::format("open file error, {}", error));
    }
    return std::make_pair(true, "");
}