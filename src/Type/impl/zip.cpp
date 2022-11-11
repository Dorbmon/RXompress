#include "zip.h"
#include <iostream>
#include <zip.h>
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
        if (name.back() == '/') {
            name.pop_back();
        }
        Compress::fileTree->InsertFile(
            name, 
            std::make_shared<FileItem>(name, false, stat.size));
    }
}

Zip::Zip(const std::string& fileName) {
    this->zip = zip_open(fileName.c_str(), ZIP_CREATE, nullptr);
    this->currentPath = "/";
    this->reloadFiles();
}

Zip::~Zip() {
    zip_close(this->zip);
}