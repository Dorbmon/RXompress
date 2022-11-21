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
    virtual bool ChangeName(void* meta, std::string_view newName) override;
    virtual std::pair<bool, std::string> Save() override;
    virtual std::pair<bool, std::string> Remove(void* meta) override;
    virtual std::tuple<bool, std::string, std::shared_ptr<FileItem>> AddFile(void* dirMeta, std::string_view filePath) override;
    ~Zip();
};
