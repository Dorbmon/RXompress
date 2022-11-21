#pragma once
#include "gtkmm/treemodel.h"
#include <string>
#include <memory>
#include <vector>
#include <map>
class FileItem {
public:
    std::string fileName;
    bool isDir;
    size_t size;
    void* metaData;
    FileItem(std::string_view fileName, bool isDir, size_t size, void* metaData): fileName(fileName), isDir(isDir), size(size), metaData(metaData) {}
};

std::vector<std::string> split(std::string_view s, std::string_view c);

class Compress {
private:
    
public:
    Compress();
    virtual std::vector<std::shared_ptr<class FileTreeNode>> GetFiles();    // Get All Files under current folder
    virtual bool ChangeName(void* meta, std::string_view newName)=0;
    virtual std::pair<bool, std::string> Save() = 0;
    virtual std::pair<bool, std::string> Remove(void* meta) = 0;
    virtual std::tuple<bool, std::string, std::shared_ptr<FileItem>> AddFile(void* dirMeta, std::string_view filePath) = 0;
    std::unique_ptr<class FileTree> fileTree;
};

class FileTreeNode {
public:
    std::weak_ptr<FileTreeNode> pre;
    std::shared_ptr<FileItem> self;
    std::map<std::string, std::shared_ptr<FileTreeNode>> map;
    Gtk::TreeModel::iterator dirListColumn;
    Gtk::TreeModel::iterator fileListColumn;
    FileTreeNode(std::shared_ptr<FileItem> self, std::shared_ptr<FileTreeNode> pre) :  pre(pre), self(self) {}
};
class FileTree {
public:
    std::shared_ptr<FileTreeNode> currentNode;
    std::shared_ptr<FileTreeNode> root;
    FileTree();
    std::string InsertFile(std::string_view fullEntryName, std::shared_ptr<FileItem> fileInfo);
    std::vector<std::shared_ptr<FileTreeNode>> GetCurrentDirFiles();
};
/*
https://www.freesion.com/article/4648229939/
*/