#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
class FileItem {
public:
    std::string fileName;
    bool isDir;
    unsigned long long size;
    FileItem(std::string_view fileName, bool isDir, unsigned long long size): fileName(fileName), isDir(isDir), size(size) {}
};

static std::vector<std::string> split(std::string_view s, std::string_view c) {
    std::vector<std::string> v;
	std::string::size_type pos1, pos2;
	size_t len = s.length();
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2) {
		v.emplace_back(s.substr(pos1, pos2 - pos1));
 
		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != len)
		v.emplace_back(s.substr(pos1));
    return v;
}
class Compress {
private:
    
public:
    Compress();
    virtual std::vector<std::shared_ptr<class FileTreeNode>> GetFiles();    // 获取当前目录下所有的文件
    std::unique_ptr<class FileTree> fileTree;
};

class FileTreeNode {
public:
    std::weak_ptr<FileTreeNode> pre;
    std::shared_ptr<FileItem> self;
    std::map<std::string, std::shared_ptr<FileTreeNode>> map;
    FileTreeNode(std::shared_ptr<FileItem> self, std::shared_ptr<FileTreeNode> pre) : self(self), pre(pre) {}
};
class FileTree {
public:
    std::shared_ptr<FileTreeNode> currentNode;
    std::shared_ptr<FileTreeNode> root;
    FileTree();
    std::string InsertFile(std::string& fullEntryName, std::shared_ptr<FileItem> fileInfo);
    std::vector<std::shared_ptr<FileTreeNode>> GetCurrentDirFiles();
};
/*
https://www.freesion.com/article/4648229939/
*/