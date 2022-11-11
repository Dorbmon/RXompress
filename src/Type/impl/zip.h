#pragma once
#include "../compress.h"
#include <string>
#include <zip.h>
class Zip: public Compress {
private:
    zip_t* zip;
    std::string currentPath;
    void reloadFiles();
public:
    Zip(const std::string& fileName);
    ~Zip();
};
