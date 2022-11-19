#pragma once
#include "../compress.h"
#include <string>
#include <zip.h>
class Zip: public Compress {
private:
    zip_t* zip;
    std::string currentPath;
    void reloadFiles();
    std::string fileName;
public:
    Zip(const std::string& fileName);
    virtual bool ChangeName(void* meta, std::string& newName) override;
    virtual std::pair<bool, std::string> Save() override;
    ~Zip();
};
