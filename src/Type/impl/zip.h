#pragma once
#include "../compress.h"
#include <string>
class Zip: public Compress {
private:
    struct zip_t* zip;
    std::string currentPath;
    void reloadFiles();
public:
    Zip(const std::string& fileName);
    ~Zip();
};
