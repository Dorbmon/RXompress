#include "zip.h"
#include "../compress.h"
#include "zip/zip.h"
#include <iostream>
void Zip::reloadFiles() {
    auto n = zip_entries_total(this->zip);
    for (ssize_t i = 0; i < n; ++i) {
        zip_entry_openbyindex(this->zip, i);
        auto name = std::string(zip_entry_name(zip));
        int isdir = zip_entry_isdir(zip);
        unsigned long long size = zip_entry_size(zip);
        //std::cout << name << " " << name.back() << std::endl;
        if (name.back() == '/') {
            name.pop_back();
        }
        Compress::fileTree->InsertFile(
            name, 
            std::make_shared<FileItem>(name, isdir, size));
    }
}

Zip::Zip(const std::string& fileName) {
    this->zip = zip_open(fileName.c_str(), 0, 'r');
    this->currentPath = "/";
    this->reloadFiles();
}

Zip::~Zip() {
    zip_close(this->zip);
}