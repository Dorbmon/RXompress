#include "compress.h"
#include <memory>


Compress::Compress() {
    this->fileTree = std::make_unique<FileTree>();
}

std::vector<std::shared_ptr<FileTreeNode>> Compress::GetFiles() {
    return this->fileTree->GetCurrentDirFiles();
}
FileTree::FileTree() {
    this->root = std::make_shared<FileTreeNode>(std::make_shared<FileItem>("/", true, 0, nullptr), nullptr);
    this->root->self->isDir = true;
    this->currentNode = this->root;
}

std::string FileTree::InsertFile(std::string_view fullEntryName, std::shared_ptr<FileItem> fileInfo) {
    std::vector<std::string> res = split(fullEntryName, "/");
    auto currentNode = this->root;
    for (auto ptr = res.begin();ptr != res.end(); ptr ++) {
        // 一层一层下去
        if (currentNode->map.find(*ptr) == currentNode->map.end()) {
            currentNode->map.insert({(*ptr), std::make_shared<FileTreeNode>(
                std::make_shared<FileItem>(*ptr, true, 0, nullptr),
                currentNode
            )});
        }
        currentNode = currentNode->map [*ptr];
    }
    currentNode->self = fileInfo;
    fileInfo->fileName = res.back();
    return res.back();
}
std::vector<std::shared_ptr<FileTreeNode>> FileTree::GetCurrentDirFiles() {
    std::vector<std::shared_ptr<FileTreeNode>> ret;
    for (auto ptr = this->currentNode->map.begin();ptr != this->currentNode->map.end(); ptr ++) {
        ret.push_back(ptr->second);
    }
    return ret;
}
std::vector<std::string> split(std::string_view s, std::string_view c) {
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